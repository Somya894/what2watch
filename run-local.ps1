$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

$compilerCandidates = @()
$msysCompiler = "C:\msys64\ucrt64\bin\g++.exe"
if (Test-Path $msysCompiler) {
    $compilerCandidates += $msysCompiler
}

$wingetPackages = Join-Path $env:LOCALAPPDATA "Microsoft\WinGet\Packages"
if (Test-Path $wingetPackages) {
    $winLibsPackage = Get-ChildItem $wingetPackages -Directory -Filter "BrechtSanders.WinLibs.POSIX.UCRT.LLVM_Microsoft.Winget.Source_*" |
        Select-Object -First 1
    if ($winLibsPackage) {
        $winLibsCompiler = Join-Path $winLibsPackage.FullName "mingw64\bin\g++.exe"
        if (Test-Path $winLibsCompiler) {
            $compilerCandidates += $winLibsCompiler
        }
    }
}

$pathCompiler = Get-Command g++ -ErrorAction SilentlyContinue
if ($pathCompiler) {
    $compilerCandidates += $pathCompiler.Source
}

$compilerCandidates = $compilerCandidates | Select-Object -Unique
if (-not $compilerCandidates) {
    throw "A MinGW C++ compiler was not found. Install MSYS2 UCRT64 with g++ and standalone Asio, then try again."
}

$built = $false
foreach ($compiler in $compilerCandidates) {
    # Build to a fresh filename so a previously launched or synchronized executable cannot block linking.
    $compilerArgs = @("-std=c++17", "main.cpp", "-o", "what2watch_server_local.exe", "-pthread", "-lws2_32", "-lmswsock")
    $asioInclude = "C:\msys64\ucrt64\include"
    if ((Split-Path $compiler -Parent) -like "*WinGet*" -and (Test-Path (Join-Path $asioInclude "asio.hpp"))) {
        $compilerArgs = @("-std=c++17", "-idirafter", $asioInclude, "main.cpp", "-o", "what2watch_server_local.exe", "-pthread", "-lws2_32", "-lmswsock")
    }

    Write-Host "Building with $compiler"
    & $compiler @compilerArgs
    if ($LASTEXITCODE -eq 0) {
        $env:Path = "$(Split-Path $compiler -Parent);$env:Path"
        $built = $true
        break
    }
    Write-Warning "Build failed with this compiler; trying the next available compiler."
}

if (-not $built) {
    throw "Could not build What2Watch. Check that Crow's standalone Asio dependency is installed and review the compiler output above."
}

$displayPort = $env:WHAT2WATCH_PORT
if (-not $displayPort) { $displayPort = "18080" }
Write-Host "Starting What2Watch at http://localhost:$displayPort"
& ".\what2watch_server_local.exe"
