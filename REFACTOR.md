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
- [ ] Étape 1 — profils de réglages par jeu des émulateurs
- [ ] Étape 2 — correctifs des bugs repérés
- [ ] Étape 3a — harnais « golden master » des entrées
- [ ] Étape 3b à 3d — fichiers d'entrées Demul / M2 / Road Fighters 3D
- [ ] Étape 4 — variantes de jeux dupliquées
- [ ] Étape 5 — architecture du cœur
- [ ] Étape 6 — GUI
- [ ] Étape 7 — build et outillage
