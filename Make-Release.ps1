<#
.SYNOPSIS
    Builds the plugin and the GUI, then packs the x64 MAME release zip and its .sha256.

.DESCRIPTION
    Builds both solutions in Release, x86 then x64 (the x86 build's post-build step writes the
    "MAME 64bit Outputs" profile of FFBPlugin.ini), then puts in
    <OutDir>\FFBArcadePlugin-<version>-njz3-windows-x64\ and in the zip of the same name:
        dinput8.dll, SDL2.dll                  FFBArcadePlugin\Release.x64
        MAME64.dll                             FFBArcadePlugin
        FFBPlugin.ini                          FFBArcadePlugin\Release.Win32\MAME 64bit Outputs (GameId=22)
        FFBPluginReadme.md                    FFBArcadePlugin
        FFBPluginGUI.exe, MetroFramework*.dll  FFBPluginGUI\x64\Release
    <version> comes from Version.h (Write-Version.ps1): "2.1.0" on the tagged commit,
    "2.1.0.3-g22f99dd" between two tags. It refuses to run with changes that are not committed,
    unless -AllowDirty, so that a zip always matches a commit.

.EXAMPLE
    powershell -ExecutionPolicy Bypass -File Make-Release.ps1 -OutDir ..\releases
#>
param(
    [string]$OutDir = (Join-Path $PSScriptRoot 'dist'),
    [switch]$AllowDirty,
    [switch]$NoBuild,
    [int]$MaxCpu = 1
)

$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot

function Invoke-Git { $ErrorActionPreference = 'Continue'; $out = & git.exe -C $root @args 2>$null; if ($LASTEXITCODE -ne 0) { return $null }; $out }

if (-not $AllowDirty -and (Invoke-Git status --porcelain --untracked-files=no)) {
    throw 'Changes are not committed: commit them, or pass -AllowDirty for a test build.'
}

if (-not $NoBuild) {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    $msbuild = & $vswhere -latest -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
    if (-not $msbuild) { throw 'MSBuild not found' }
    foreach ($sln in 'FFBArcadePlugin\Dinput8Wrapper.sln', 'FFBPluginGUI\FFBPluginGUI.sln') {
        foreach ($platform in 'x86', 'x64') {
            & $msbuild (Join-Path $root $sln) -t:restore "-p:Platform=$platform;Configuration=Release" -p:RestorePackagesConfig=true -nologo -v:m
            if ($LASTEXITCODE -ne 0) { throw "restore failed: $sln $platform" }
            & $msbuild (Join-Path $root $sln) "/m:$MaxCpu" "-p:Platform=$platform;Configuration=Release" -nologo -v:m
            if ($LASTEXITCODE -ne 0) { throw "build failed: $sln $platform" }
        }
    }
}

$header = Get-Content -LiteralPath (Join-Path $root 'Version.h') -Raw
function Get-VersionDefine([string]$name) { if ($header -notmatch "#define $name `"([^`"]*)`"") { throw "Version.h: no $name" }; $Matches[1] }
$version = Get-VersionDefine 'FFB_VERSION_STRING'
$fileVersion = Get-VersionDefine 'FFB_VERSION_FILE'

$plugin = Join-Path $root 'FFBArcadePlugin'
$gui = Join-Path $root 'FFBPluginGUI\x64\Release'
$files = [ordered]@{
    'dinput8.dll'               = "$plugin\Release.x64\dinput8.dll"
    'SDL2.dll'                  = "$plugin\Release.x64\SDL2.dll"
    'MAME64.dll'                = "$plugin\MAME64.dll"
    'FFBPlugin.ini'             = "$plugin\Release.Win32\MAME 64bit Outputs\FFBPlugin.ini"
    'FFBPluginReadme.md'       = "$plugin\FFBPluginReadme.md"
    'FFBPluginGUI.exe'          = "$gui\FFBPluginGUI.exe"
    'MetroFramework.dll'        = "$gui\MetroFramework.dll"
    'MetroFramework.Design.dll' = "$gui\MetroFramework.Design.dll"
    'MetroFramework.Fonts.dll'  = "$gui\MetroFramework.Fonts.dll"
}

# the binaries must be the ones of this version, and the ini the MAME profile
foreach ($name in 'dinput8.dll', 'FFBPluginGUI.exe') {
    $v = [Diagnostics.FileVersionInfo]::GetVersionInfo($files[$name])
    if ($v.FileVersion -ne $fileVersion -or $v.ProductVersion -ne $version) { throw "$name is $($v.ProductVersion), expected ${version}: build first" }
}
if ((Get-Content -LiteralPath $files['FFBPlugin.ini'] -Raw) -notmatch '(?m)^GameId=22\r?$') { throw 'FFBPlugin.ini is not the MAME 64bit Outputs profile (GameId=22)' }

$name = "FFBArcadePlugin-$version-njz3-windows-x64"
New-Item -ItemType Directory -Force $OutDir | Out-Null
$OutDir = (Resolve-Path -LiteralPath $OutDir).Path
$folder = Join-Path $OutDir $name
$zip = "$folder.zip"
if (Test-Path -LiteralPath $folder) { Remove-Item -LiteralPath $folder -Recurse -Force }
if (Test-Path -LiteralPath $zip) { Remove-Item -LiteralPath $zip -Force }
New-Item -ItemType Directory $folder | Out-Null
foreach ($f in $files.Keys) { Copy-Item -LiteralPath $files[$f] (Join-Path $folder $f) }

# entries named with '/', whatever the PowerShell and .NET versions
Add-Type -AssemblyName System.IO.Compression, System.IO.Compression.FileSystem
$archive = [IO.Compression.ZipFile]::Open($zip, 'Create')
try {
    [void]$archive.CreateEntry("$name/")
    foreach ($f in $files.Keys) {
        [void][IO.Compression.ZipFileExtensions]::CreateEntryFromFile($archive, (Join-Path $folder $f), "$name/$f", 'Optimal')
    }
}
finally { $archive.Dispose() }

$hash = (Get-FileHash -LiteralPath $zip -Algorithm SHA256).Hash.ToLower()
[IO.File]::WriteAllText("$zip.sha256", "$hash  $name.zip`n", (New-Object Text.UTF8Encoding $false))
"$zip"
"SHA256 $hash"
