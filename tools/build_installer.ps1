# Set paths
$projectRoot = Resolve-Path ".."
$releaseDir = "$projectRoot\launcher\release"
$dataDir = "$projectRoot\packages\com.oxcorp.launcher\data"
$binarycreator = "binarycreator.exe"
$config = "$projectRoot\config\config.xml"
$packages = "$projectRoot\packages"
$output = "$projectRoot\tools\OxcorpInstaller.exe"

Write-Host "Step 1: Initializing data directory..."
if (Test-Path $dataDir) { Remove-Item $dataDir -Recurse -Force }
New-Item -ItemType Directory -Force -Path $dataDir | Out-Null

Write-Host "Step 2: Copying launcher/release to data directory..."
Copy-Item -Path "$releaseDir\*" -Destination $dataDir -Recurse -Force

Write-Host "Step 3: Building installer using Qt Installer Framework..."
& $binarycreator --config $config --packages $packages $output

Write-Host "Done: Installer built at $output"
