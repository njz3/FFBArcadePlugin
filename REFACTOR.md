# Plan de refactorisation — FFB Arcade Plugin

Ce document suit la factorisation du plugin (`FFBArcadePlugin`) et de sa GUI (`FFBPluginGUI`) : ce qui est fait, ce qui reste à faire, dans quel ordre et comment le valider.

## Principes

- **Ne pas changer le comportement.** Le retour de force ne peut pas être testé sans les jeux et le matériel : chaque étape doit être une transformation dont on peut prouver l'équivalence (même adresses mémoire, mêmes clés `.ini` et valeurs par défaut, mêmes calculs, mêmes types et conversions).
- **Les corrections de bugs sont séparées** des factorisations, dans leurs propres commits, et validées en jeu.
- **Une famille de jeux par commit**, pour pouvoir bisecter et revenir en arrière facilement.
- **Ne pas renommer les clés `.ini`**, même incohérentes (ex. `PersistentAlternativeMaxForceLeftDirtDevils`) : elles sont déjà dans les fichiers des utilisateurs et dans la GUI.

## Validation de chaque étape

1. Compiler x64 et x86 en dehors des dossiers `Release.*`, sans les scripts post-build :

   ```
   "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" FFBArcadePlugin\Dinput8Wrapper.sln -nologo -nr:false -m:8 "-p:Platform=x64;Configuration=Release;PostBuildEventUseInBuild=false;OutDir=<dossier>\;IntDir=<dossier>\obj\"
   ```

   `-nr:false` est important : sans lui, des processus `cl.exe` orphelins peuvent garder des fichiers sources verrouillés en écriture.
2. Aucun nouvel avertissement par rapport à la compilation précédente.
3. Table d'exports de `dinput8.dll` identique (`dumpbin /exports`), pour x64 et x86.
4. Relecture du diff : comparaison mécanique des adresses, clés `.ini` et valeurs par défaut (script), puis relecture humaine des parties réécrites.
5. Pour les étapes à risque, test en jeu des jeux concernés (liste donnée dans chaque étape).
6. Test différentiel (voir la refonte orientée objet ci-dessous) : l'ancien et le nouveau code, compilés dans un programme de test avec des simulations, doivent demander exactement les mêmes effets, écritures mémoire, crochets et réglages.

## Refonte orientée objet — fait (novembre 2026)

Commits 85926e2 à a71f024 sur `dev/mame-racer`. Le code des jeux est passé d'une fonction `FFBLoop(constants, helpers, triggers)` par jeu, avec des variables statiques, des pointeurs `myTriggers`/`myHelpers` et ~60 globales `extern`, à des classes :

| Avant | Après |
|---|---|
| `EffectTriggers` (pointeurs de fonctions), `EffectConstants` | `ffb::Wheel` et `ffb::Device` (méthodes virtuelles), `ffb::Direction` |
| `Helpers` (`ReadFloat32(0x1234, true)`...) | `ffb::Memory` avec des adresses typées : `GameValue<float>{ Base::Module, 0x1234 }`, `PointerChain`, `WriteNop`, `Patch`, `FindPattern` |
| `MH_CreateHook((LPVOID)a, d, (LPVOID*)&o)` | `ffb::Hooks::Create(a, d, &o)` (typé) |
| `GetSettingInt` dans des statiques lues au chargement | `ffb::Settings`, membres des classes ; `ffb::Config` pour les réglages de `DllMain.cpp` |
| ~60 globales `extern` dans 30 fichiers | `ffb::Config` (réglages) et `Core/PluginState.h` (état partagé, déclaré une fois) |
| `MAMESupermodel.cpp` : 5 084 lignes, 350 chaînes de roms, 867 réglages lus au chargement, 78 blocs de copie | `GameFiles/EmulatorGames.cpp` (une ligne par jeu), `Core/ForceProfile`, 30 classes de cartes dans `Hardware/` |
| versions d'un même jeu recopiées | classes de base avec une structure d'adresses par version (Initial D 4/5, 6/7/8, 0 ; Daytona 3/NSE ; Out Run 2 ; Let's Go Island ; WMMT ; Gaelco ; FNF ; Hummer ; Mario Kart Real ; Dead Heat/WMMT 3) |
| `PlayInitialDFFB` | `Hardware/SegaInitialD` : la drive board Initial D, partagée par les jeux TeknoParrot et Flycast |
| `CreateGame` dans `DllMain.cpp` | `Core/GameFactory.cpp` |
| macros `VK_A`..`VK_Z`, copies de `SDL_HAT_*`, `PERSISTENT_KEYS` | supprimées ; restent les X-macros des exports de DLL (`WrapperExports.h`), le seul moyen de les générer |

Les fichiers d'entrées de Demul et M2Emulator sont devenus des classes (`DemulInputs`, `M2EmulatorInputs`), convertis mécaniquement : leur logique n'est pas encore factorisée (étape 3c).

**Validation.** Un programme de test compile l'ancien code (5b650e8) et le nouveau avec des simulations déterministes : mémoire du jeu (valeurs typées, changeantes), mémoire partagée TeknoParrot, événements joystick, sorties MAME aléatoires, appels des crochets avec des arguments aléatoires (x86), fenêtres des jeux de Demul et M2Emulator, threads des jeux, temps virtuel. Il enregistre les effets, écritures mémoire, crochets, threads et réglages appliqués, et compare les deux versions :

- 1 460 exécutions, en 32 bits et en 64 bits : tous les jeux et toutes les roms MAME/Supermodel/Flycast, avec deux FFBPlugin.ini (valeurs par défaut, et une valeur pour chaque clé) et deux graines, 3 000 boucles chacune ;
- 186 exécutions de Demul, M2Emulator et Road Fighters 3D avec la fenêtre de chaque jeu, leurs threads et les entrées du volant activées.

Toutes identiques à chaque commit. Les jeux peu couverts (crochets d'API, threads) ont été relus deux fois, ligne à ligne, et certains testés avec des valeurs orientées. Quatre jeux MAME ont été lancés dans MAME. Rien n'a encore été testé sur les bornes.

## Bugs repérés pendant la refonte orientée objet, conservés

Le comportement a été gardé à l'identique ; ces points sont à corriger un par un, en jeu.

| Jeu | Problème |
|---|---|
| GRID Custom | plante si la première boucle voit un changement de vitesse (son pointeur d'effets n'est posé qu'à la fin de la boucle) |
| Scud Race, Le Mans, Dirt Devils, ECA (Supermodel) | les séquences de la drive board ne jouent jamais : `(début - maintenant) > durée` en non signé est vrai dès la première boucle |
| Mario Kart GP DX Custom 1.10/1.18/USA | rumble de l'eau ~128 fois trop fort (priorité des opérateurs) ; `Sleep(50)` dans la boucle FFB ; la petite bosse annulée aussitôt ; force du volant infinie avec `ConstantEffectForSteeringStrength` à 0 |
| Mario Kart GP DX 1.00 Custom | forces de rumble 3,5 et 3,0 (> 1) ; durée partagée entre deux effets |
| Storm Racer G | avec `MinFFBConstantStrength` à 0, toute force est maximale |
| GRID Real, Showdown, Sega Rally 3 Other | `(31 - data)` en non signé : une valeur > 31 donne une force énorme |
| Sega Rally 3 | une valeur > 31 donne une force négative |
| Ford Racing / Ford Racing Other | forces jusqu'à 1,67 / 1,1 |
| Initial D 4 | pas d'amortisseur, contrairement à 4 Japan et 5 |
| Initial D 0 v2.11 | `EnableOutputs == 1` au lieu de « différent de 0 » dans les autres versions |
| Sega Race TV | écriture « enable I/O » relative au module alors que la force est lue en absolu ; moteurs de rumble inversés |
| Crazy Taxi | `springStrength / 100` en division entière : ressort nul sous 100 |
| SWDC 2018 | amortisseur joué deux fois par boucle |
| Wasteland Racers 2071 | le rumble prend la position brute du volant (bien au-delà de 1) |
| Hummer | ressort = octet brut (0 à 255) ; Hummer Extreme : clé `ViosityStrength` mal orthographiée (gardée, les .ini l'utilisent) |
| Golden Gun | le drapeau « plus de vie » du joueur 2 n'est jamais remis à zéro |
| Out Run 2 Real | le saut est réécrit à chaque événement joystick ; la vitesse est lue sans être utilisée ; la commande 0x7D (« inutilisée ») joue le ressort |
| Out Run 2 Fake | la force du volant est jouée deux fois quand les valeurs course et menu valent 1 |
| BG4 JP | lectures de 4 octets qui se chevauchent pour lire des octets |
| Dead Heat / WMMT 3 | le retour de force « par la gauche » est 0x40–0xFF dans l'un et négatif dans l'autre |
| Mach Storm | réglage `DomeFix` lu mais inutilisé |
| Chase HQ 2 | le masque `16384 + 1` n'enlève le bit 0 qu'avec le bit 14 |
| Demul | `LoadLibrary("padDemul.dll")` et les NOP refaits à chaque boucle |
| M2Emulator | clés `.ini` avec une espace finale (`InputDeviceWheelSteeringAxisDevice2 `) |
| TestGame | l'ID `-8` (rumble des gâchettes) n'est jamais créé par la fabrique |

## Étape 0 — Fait (octobre 2026)

Bilan : 109 fichiers modifiés, environ −9 700 / +2 800 lignes, plus ~1 500 lignes de nouveaux fichiers partagés. Comportement préservé, validé par compilation x64/x86, exports identiques et quatre relectures indépendantes. Rien n'a été testé en jeu.

| Zone | Avant | Après |
|---|---|---|
| `DllMain.cpp` | 3 634 lignes ; 19 effets avec le calcul de force recopié ; Sine/Rumble copiés pour 3 périphériques ; 2 boucles de réglage de force identiques ; ~1 300 lignes de déclarations et `GetProcAddress` | ~1 500 lignes ; `ScaleForce`/`ScaleCoefficient`/`RunSineEffect`/`RunRumble` ; une seule boucle de réglage ; table `wrappedLibraries` ; X-macros dans [WrapperExports.h](FFBArcadePlugin/WrapperExports.h) |
| `Config/PersistentValues.h` | 930 lignes de `if (romname == …)` dans un header | [PersistentValues.cpp](FFBArcadePlugin/Config/PersistentValues.cpp), table de ~50 entrées |
| Identifiants de jeux | constantes dans `DllMain.cpp`, nombres magiques ailleurs | [GameIds.h](FFBArcadePlugin/Common%20Files/GameIds.h) |
| Lecture des réglages | 1 647 appels `GetPrivateProfileInt(TEXT("Settings"), …, settingsFilename)` et ~50 copies de `settingsFilename` | `GetSettingInt()` dans [Game.h](FFBArcadePlugin/Common%20Files/Game.h) |
| Lecture/écriture mémoire | une fonction par type dans `Helpers` | templates `Read<T>`/`Write<T>` |
| Amortisseur | 63 blocs `if (EnableDamper) …->Damper(DamperStrength / 100.0)` | `ApplyDamper()` |
| Modes de rumble des jeux de tir | 44 chaînes `if (HowtoRumble… == 0/1/2)` | `RumbleWithMode()` |
| 2ᵉ/3ᵉ périphérique | boucle de recherche par GUID copiée dans 19 fichiers | [Devices.cpp](FFBArcadePlugin/Common%20Files/Devices.cpp) |
| TeknoParrot | lectures de la mémoire partagée dupliquées | [TeknoParrotGame.cpp](FFBArcadePlugin/Common%20Files/TeknoParrotGame.cpp) simplifié |
| WMMT 5/5DX/5DX+/6/6R/6RR | 6 fichiers de ~320 lignes quasi identiques | base [WMMT.cpp](FFBArcadePlugin/Game%20Files/WMMT.cpp) + table d'adresses par version |
| Protocole Initial D (`0x80/0x84/0x85/0x86`) | recopié dans 13 jeux | [InitialDFFB.cpp](FFBArcadePlugin/Common%20Files/InitialDFFB.cpp) |
| Ring Riders / Tokyo Cop / Tuning Race | même hook copié 3 fois | base [Gaelco.cpp](FFBArcadePlugin/Game%20Files/Gaelco.cpp) |
| Code mort | — | 131 déclarations inutilisées supprimées |

Changements de comportement volontaires :

- `WritePersistentMaxForce` n'écrit plus avec une clé `NULL`, ce qui supprimait toute la section `[Settings]` de `FFBPlugin.ini` pour un jeu d'émulateur absent de la table des clés persistantes.
- La boucle principale fait toujours `Sleep(16)`, même sans jeu reconnu (elle tournait à 100 % CPU).
- `TriggerRampEffect` : la valeur de fin utilise son propre seuil de force minimale (effet utilisé par aucun jeu).
- `if (EnableDamper == 1)` unifié en `if (EnableDamper)` dans 21 fichiers ; identique tant que la valeur vaut 0 ou 1, ce que la GUI garantit.
- Lectures sûres : mémoire partagée TeknoParrot non mappée, nom de rom `NULL`, lectures mémoire en échec (retournent 0).

## Étape 1 — Profils de réglages par jeu des émulateurs

Gain élevé, risque faible, entièrement vérifiable par script.

**Constat.** `MAMESupermodel.cpp`, `M2Emulator.cpp` et `Demul.cpp` déclarent 707 variables `static int` qui lisent les mêmes 12 réglages suffixés par le nom du jeu (`MinForceDaytona2`, `MaxForceDaytona2`, …, `ForceSpringStrengthDaytona2`). Puis 73 blocs de 12 affectations les recopient dans les globales de `DllMain.cpp` quand le jeu est détecté (61 dans MAME/Supermodel/Flycast, 6 dans M2, 6 dans Demul).

**Plan.**

1. Créer une struct `ForceProfile` (les 12 champs) et une fonction `LoadForceProfile(L"Daytona2")` qui lit les clés `<Réglage><Suffixe>` avec les mêmes valeurs par défaut.
2. Ajouter `ApplyForceProfile(const ForceProfile&)` qui copie dans les globales, dans le même ordre.
3. Remplacer chaque bloc de détection par une entrée de table `{ roms…, suffixe, RunningFFB }`. Les groupes de roms sont en grande partie les mêmes que dans `PersistentValues.cpp` : partager une seule table des jeux d'émulateurs.
4. Attention aux suffixes de périphériques (`RevXDevice2`, `LGhostDevice3`…), qui alimentent les réglages `…Device2/3`.

**Validation.** Script qui compare, pour chaque jeu, la liste (clé, valeur par défaut, globale cible) avant/après. Tests en jeu : un jeu MAME, un Supermodel, un M2, un Demul.

Gain estimé : ~1 500 lignes.

## Étape 2 — Bugs repérés pendant l'étape 0

Petits correctifs, un commit chacun, à valider en jeu.

| Problème | Emplacement | Correction proposée |
|---|---|---|
| L'ID de test `-8` (rumble des gâchettes) n'est pas créé par la fabrique : `TestGame` le gère mais n'est jamais instancié pour lui | [DllMain.cpp:1221](FFBArcadePlugin/DllMain.cpp:1221) | ajouter `case TEST_GAME_RUMBLE_TRIGGER:` |
| L'effet `LeftRight` n'est jamais créé : `effect_leftright_id` vaut 0 et met à jour l'effet constant | [DllMain.cpp:784](FFBArcadePlugin/DllMain.cpp:784) | créer l'effet dans `Initialize()` (vérifier avec Mario Kart GP DX USA Custom, seul utilisateur) |
| Clés `.ini` avec un espace final, donc probablement jamais lues | [M2EmulatorDaytonaInputs.cpp:44](FFBArcadePlugin/Game%20Files/M2EmulatorDaytonaInputs.cpp:44) | retirer l'espace (comparer avec Indy 500 / Sega Rally) |
| Sans `Device2GUID`, le dernier joystick sert de 2ᵉ périphérique | [RoadFighters3D.cpp:279](FFBArcadePlugin/Game%20Files/RoadFighters3D.cpp:279) | décider si c'est voulu ; sinon supprimer le repli |
| Conversions pointeur ↔ entier signalées en x64 | [OutRun2Real.cpp:177](FFBArcadePlugin/Game%20Files/OutRun2Real.cpp:177), [GRIDReal.cpp:67](FFBArcadePlugin/Game%20Files/GRIDReal.cpp:67), [MAMESupermodel.cpp:2137](FFBArcadePlugin/Game%20Files/MAMESupermodel.cpp:2137) | utiliser `INT_PTR`/`uintptr_t` ; vérifier que les jeux concernés sont bien 32 bits |
| `%08X` reçoit un `double` (log seulement) | [Showdown.cpp:71](FFBArcadePlugin/Game%20Files/Showdown.cpp:71) | format `%f` |

## Étape 3 — Fichiers d'entrées Demul, M2Emulator et Road Fighters 3D

Gain très élevé, risque élevé : ne pas commencer sans le harnais de l'étape 3a.

**Constat.** 6 fichiers `Demul*Inputs.cpp` (~5 500 lignes), 3 fichiers `M2Emulator*Inputs.cpp` (~4 800 lignes) et `RoadFighters3D.cpp` (1 445 lignes), similaires à 70–96 %. Ils ont tous la même structure : réglages des boutons pour les périphériques 1 et 2, croix directionnelle et clavier associés par nom d'action, conversion des axes (zone morte, axe inversé, pédales combinées), affichage des numéros de boutons et d'axes pour la configuration. Les calculs d'axes diffèrent subtilement d'un jeu à l'autre.

**Plan.**

- **3a. Harnais « golden master » d'abord.** Petit projet de test (console) qui compile les fichiers d'entrées avec des adresses pointant vers un tampon mémoire, rejoue des séquences scriptées de `SDL_Event` (axes, boutons, croix, clavier) et enregistre les octets écrits. On exécute ce harnais sur l'ancien et le nouveau code : les résultats doivent être identiques.
- **3b.** Extraire les blocs strictement identiques : l'affichage `ShowAxisForSetup` / `ShowButtonNumbersForSetup` est le même dans 7 fichiers.
- **3c.** Remplacer les chaînes de `if` par une table d'actions `{ réglage bouton, réglage périphérique 2, réglage clavier, adresse, masque, logique inversée }`, traitée par un moteur commun. Conserver le motif actuel « valeur lue une fois puis `-=`/`+=` » (`servicetestread -= 0x40`), qui a un effet sur l'état.
- **3d.** Garder la conversion des axes propre à chaque jeu, sous forme de petites fonctions.

**Tests en jeu.** Un jeu par fichier (Nascar, Initial D, Smashing Drive, Maximum Speed, Faster Than Speed, ATV Track, Daytona USA, Indy 500, Sega Rally, Road Fighters 3D), avec un volant seul puis avec un 2ᵉ périphérique.

## Étape 4 — Variantes de jeux encore dupliquées

Gain moyen, risque moyen. Même méthode que pour WMMT : une base commune avec une table d'adresses, ou un helper partagé.

| Groupe | Fichiers | Similarité |
|---|---|---|
| Mario Kart GP DX | `MarioKartGPDX*Custom/Real.cpp` (7 fichiers, 42 à 476 lignes) | variantes Custom/Real par version |
| OutRun 2 | `OutRun2Fake.cpp`, `OutRun2Real.cpp` | 63 % |
| Let's Go Island | `LGI.cpp`, `LGI3D.cpp` | 87 % |
| Fast & Furious | `FnF.cpp`, `FnFDrift.cpp`, `FnFSuperCars.cpp` | 84 % |
| Hummer | `Hummer.cpp`, `HummerExtreme.cpp` | 79 % |
| Dead Heat / WMMT3 (TeknoParrot) | `DeadHeat.cpp`, `DeadHeatRiders.cpp`, `WMMT3.cpp` | 73–85 % |
| Ford Racing | `FordRacing.cpp`, `FordRacingOther.cpp` | — |
| Sega Rally 3 | `SegaRally3.cpp`, `SegaRally3Other.cpp` | — |
| Daytona 3 | `Daytona3.cpp`, `Daytona3NSE.cpp` | 66 % |
| Motif TeknoParrot `(31 - ff) / 15.0` | `ChaseHQ2.cpp`, `Daytona3.cpp`, `SegaRally3.cpp`, `WackyRaces.cpp` | bloc identique |

## Étape 5 — Architecture du cœur

Gain qualitatif, à faire une fois les étapes 1 à 4 terminées.

- Regrouper les ~60 globales de réglages de `DllMain.cpp` dans une struct `Settings`, avec une struct par périphérique au lieu de trois séries `…`, `…Device2`, `…Device3`.
- Remplacer les `extern` dispersés dans les fichiers de jeux par un header commun.
- Découper `MAMESupermodel.cpp` (~4 500 lignes) :
  - réception des sorties MAME (fenêtre `WinMain`/`WndProc`, `mame_*`) ;
  - détection de la rom ;
  - effets par jeu (`FFBGameEffects`, une seule fonction géante).
- Remplacer le `switch` de `CreateGame` par une table `{ GameId, fabrique }` partagée avec la GUI.

## Étape 6 — GUI (`FFBPluginGUI`)

**Constat.** `Helper.h` fait 2 138 lignes ; les écrans `*Select.h` en totalisent 4 946 (MAME 1 707, Flycast 785, Supermodel 744, M2 743, Demul 716) ; la GUI compte 655 appels `AutoAddComponent`. Les formulaires par jeu sont générés par le designer WinForms (C++/CLI).

**Plan.** Décrire la liste des jeux (ID, nom, réglages affichés) dans une table unique partagée avec `GameIds.h`, et générer les écrans de sélection à partir de cette table. Ne pas toucher au code généré par le designer à la main.

## Étape 7 — Build et outillage

- Supprimer les restes de l'ancien script post-build Node.js, remplacé par `build.ps1` et appelé nulle part : `Post-Build/build.js`, `package.json`, `package-lock.json`, `node_modules/`.
- Ajouter une intégration continue (GitHub Actions, `windows-latest`) : compilation x64/x86 et comparaison des exports.
- Ajouter un `.editorconfig`. Les fichiers mélangent aujourd'hui UTF-8 avec et sans BOM ; garder CRLF et les tabulations.
- Corriger les avertissements existants (étape 2), puis passer en `/WX` (avertissements traités comme des erreurs).

## Suivi

- [x] Étape 0 — cœur, helpers partagés, WMMT, Initial D, Gaelco, périphériques, code mort
- [x] Étape 1 — profils de réglages par jeu des émulateurs (`ForceProfile`, table `EmulatorGames.cpp`, Demul, M2Emulator)
- [ ] Étape 2 — correctifs des bugs repérés (liste complétée par la refonte orientée objet)
- [x] Étape 3a — harnais « golden master » : le test différentiel de la refonte orientée objet (hors dépôt pour l'instant)
- [ ] Étape 3b à 3d — fichiers d'entrées Demul / M2 / Road Fighters 3D : devenus des classes, logique pas encore factorisée
- [x] Étape 4 — variantes de jeux dupliquées (classes de base avec une structure d'adresses par version)
- [x] Étape 5 — architecture du cœur (`Core/`, `Hardware/`, `ffb::Config`, `MAMESupermodel.cpp` découpé ; la fabrique n'est pas encore partagée avec la GUI)
- [ ] Étape 6 — GUI
- [ ] Étape 7 — build et outillage
- [ ] Le moteur des effets (`DllMain.cpp` : effets SDL, effets entretenus, réglage de la force en jeu) en classes de `Core/`
