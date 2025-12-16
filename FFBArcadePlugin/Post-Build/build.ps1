<#
.SYNOPSIS
    Générateur de configurations FFBPlugin (Portage PowerShell)
#>

param (
    [string]$Filename = "$PSScriptRoot\FFBPlugin.ini",
    [string]$SourceDir = "$PSScriptRoot",
    [string]$OutputDir = "$PSScriptRoot"
)

# --- Fonction Helper pour parser un fichier INI ---
function Get-IniContent {
    param([string]$FilePath)
    $ini = @{}
    $currentSection = "Global"
    
    if (-not (Test-Path $FilePath)) { return $null }

    Get-Content $FilePath | ForEach-Object {
        $line = $_.Trim()
        if ($line -eq "" -or $line.StartsWith(";")) { return } # Ignorer vides et commentaires

        if ($line -match "^\[(.*)\]$") {
            $currentSection = $matches[1]
            $ini[$currentSection] = @{}
        } elseif ($line -match "^(.*?)=(.*)$") {
            $key = $matches[1].Trim()
            $value = $matches[2].Trim()
            # Supprimer les guillemets si présents (comportement standard INI)
            if ($value -match "^['`"](.*)['`"]$") { $value = $matches[1] }
            if ($null -ne $ini[$currentSection]) {
                $ini[$currentSection][$key] = $value
            }
        }
    }
    return $ini
}

# --- Début du script ---

# 1. Lecture du fichier INI
Write-Host "Reading INI file: $Filename"
$o = Get-IniContent -FilePath $Filename

if ($null -eq $o) {
    Write-Error "Fichier INI introuvable ou vide."
    exit
}

# 2. Vérification de la DLL
if (Test-Path "$OutputDir\dinput8.dll") {
    Write-Host "dinput8.dll found, build is ok" -ForegroundColor Green
} else {
    Write-Host "dinput8.dll NOT FOUND, ERROR IN BUILD!" -ForegroundColor Yellow
}

# 3. Récupération des paramètres globaux
$globalSettings = @{}
if ($o.ContainsKey("Settings")) {
    $globalSettings = $o["Settings"]
}

# 4. Boucle sur les sections
foreach ($key in $o.Keys) {
    # On ignore la section "Settings" elle-même pour ne traiter que les sous-profils
    if ($key -eq "Settings") { continue }

    $folderName = $key
    
    # Fusionner Global + Spécifique (Spécifique écrase Global)
    # On clone d'abord les settings globaux
    $realOutput = $globalSettings.Clone()
    
    # On applique les settings spécifiques
    $specificSettings = $o[$key]
    foreach ($settingKey in $specificSettings.Keys) {
        $realOutput[$settingKey] = $specificSettings[$settingKey]
    }

    # Création du dossier s'il n'existe pas
    $targetPath = Join-Path -Path $OutputDir -ChildPath $folderName
    if (-not (Test-Path $targetPath)) {
        New-Item -ItemType Directory -Force -Path $targetPath | Out-Null
    }

    Write-Host "Exporting settings for $folderName" -ForegroundColor Cyan

    # Construction du contenu du fichier de sortie
    $headerText = " *** FFB Settings for $folderName ***"
    $stars = "*" * $headerText.Length
    
    $sb = [System.Text.StringBuilder]::new()
    $sb.AppendLine("; $stars") | Out-Null
    $sb.AppendLine("; $headerText") | Out-Null
    $sb.AppendLine("; $stars") | Out-Null
    $sb.AppendLine("[Settings]") | Out-Null

    foreach ($k in $realOutput.Keys) {
        $sb.AppendLine("$k=$($realOutput[$k])") | Out-Null
    }

    # Écriture du fichier
    $outFilePath = Join-Path -Path $targetPath -ChildPath "FFBPlugin.ini"
    Set-Content -Path $outFilePath -Value $sb.ToString() -Encoding Ascii

    # Partie commentée dans le script original (Copie de DLLs)
    <#
    try {
        Copy-Item "$SourceDir\SDL2.dll" "$targetPath\SDL2.dll" -ErrorAction Stop
        Copy-Item "$SourceDir\dinput8.dll" "$targetPath\dinput8.dll" -ErrorAction Stop
    } catch {
        # oh well
    }
    #>
}
