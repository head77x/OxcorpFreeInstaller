# 완전한 배포 준비 스크립트
# 사용법: .\complete_deploy.ps1

param(
    [string]$LauncherDir = "./",
    [string]$VcpkgRoot = "d:\Works\vcpkg"
)

$releaseDir = Join-Path $LauncherDir "release"
$qtBinPath = "C:\Qt\6.9.0\mingw_64\bin"
$vcpkgBinPath = Join-Path $VcpkgRoot "installed\x64-windows\bin"

Write-Host "=== 완전한 배포 준비 시작 ===" -ForegroundColor Green

# 1. 기본 확인
if (!(Test-Path $releaseDir)) {
    Write-Host "❌ Release 폴더가 없습니다: $releaseDir" -ForegroundColor Red
    Write-Host "먼저 다음 명령으로 빌드하세요:" -ForegroundColor Yellow
    Write-Host "  cd $LauncherDir" -ForegroundColor White
    Write-Host "  qmake Oxlauncher.pro" -ForegroundColor White
    Write-Host "  mingw32-make release" -ForegroundColor White
    exit 1
}

# 2. windeployqt 실행 확인
$exePath = Join-Path $releaseDir "Oxlauncher.exe"
if (!(Test-Path $exePath)) {
    Write-Host "❌ Oxlauncher.exe가 없습니다: $exePath" -ForegroundColor Red
    exit 1
}

Write-Host "📁 현재 release 폴더 상태:" -ForegroundColor Yellow
$qtDllCount = (Get-ChildItem "$releaseDir\Qt*.dll" -ErrorAction SilentlyContinue).Count
if ($qtDllCount -eq 0) {
    Write-Host "⚠️  Qt DLL이 없습니다. windeployqt를 실행합니다..." -ForegroundColor Yellow
    
    Push-Location $releaseDir
    try {
        & "$qtBinPath\windeployqt.exe" "Oxlauncher.exe"
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ windeployqt 완료" -ForegroundColor Green
        } else {
            Write-Host "❌ windeployqt 실패" -ForegroundColor Red
            exit 1
        }
    } finally {
        Pop-Location
    }
} else {
    Write-Host "✅ Qt DLL들이 이미 존재합니다 ($qtDllCount 개)" -ForegroundColor Green
}

# 3. vcpkg DLL들 추가
Write-Host "`n📦 vcpkg DLL들 추가..." -ForegroundColor Yellow

if (Test-Path $vcpkgBinPath) {
    $vcpkgDlls = @(
        "zip.dll",      # libzip
        "zlib1.dll",    # zlib
        "bz2.dll"       # bzip2 (libzip이 의존할 수 있음)
    )
    
    foreach ($dll in $vcpkgDlls) {
        $sourcePath = Join-Path $vcpkgBinPath $dll
        $destPath = Join-Path $releaseDir $dll
        
        if (Test-Path $sourcePath) {
            if (!(Test-Path $destPath)) {
                Copy-Item $sourcePath -Destination $destPath -Force
                Write-Host "  ✅ 복사: $dll" -ForegroundColor Green
            } else {
                Write-Host "  ℹ️  이미 존재: $dll" -ForegroundColor Cyan
            }
        } else {
            Write-Host "  ❌ 없음: $dll (경로: $sourcePath)" -ForegroundColor Red
        }
    }
} else {
    Write-Host "  ❌ vcpkg bin 폴더를 찾을 수 없습니다: $vcpkgBinPath" -ForegroundColor Red
}

# 4. 최종 확인
Write-Host "`n🔍 최종 DLL 목록:" -ForegroundColor Yellow
Get-ChildItem "$releaseDir\*.dll" | Sort-Object Name | ForEach-Object {
    $size = [math]::Round($_.Length / 1KB, 1)
    Write-Host "  📄 $($_.Name) ($size KB)" -ForegroundColor White
}

# 5. 의존성 검증 (선택적)
Write-Host "`n🧪 의존성 검증..." -ForegroundColor Yellow
$missingDlls = @()

# zip.dll 확인
if (!(Test-Path "$releaseDir\zip.dll")) {
    $missingDlls += "zip.dll"
}

if ($missingDlls.Count -gt 0) {
    Write-Host "  ⚠️  누락된 중요 DLL들: $($missingDlls -join ', ')" -ForegroundColor Red
    Write-Host "  이 DLL들이 없으면 사용자 PC에서 오류가 발생할 수 있습니다." -ForegroundColor Yellow
} else {
    Write-Host "  ✅ 필수 DLL들이 모두 준비되었습니다!" -ForegroundColor Green
}

Write-Host "`n🎉 배포 준비 완료!" -ForegroundColor Green
Write-Host "이제 $releaseDir 폴더의 모든 파일을 배포하세요." -ForegroundColor Cyan