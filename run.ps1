# Digital Phone Directory Launcher Script
# Run this script using PowerShell to launch the application.

Clear-Host
Write-Host "==========================================================" -ForegroundColor Cyan
Write-Host "      Starting Digital Phone Directory Application        " -ForegroundColor Cyan
Write-Host "==========================================================" -ForegroundColor Cyan

$root = Get-Location

# 1. Compile Backend
$compileSuccess = $false
if (Get-Command g++ -ErrorAction SilentlyContinue) {
    Write-Host "`n[1/3] g++ found. Compiling C++ REST API Backend..." -ForegroundColor Green
    g++ -std=c++17 -Wall -Wextra server.cpp Contact.cpp ContactLinkedList.cpp ContactBST.cpp UndoStack.cpp RecentlyAddedQueue.cpp DirectoryManager.cpp -o server.exe -lws2_32
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✔ Backend compiled successfully!" -ForegroundColor Green
        $compileSuccess = $true
    } else {
        Write-Warning "Backend compilation failed. You will need to resolve build issues before running server.exe."
    }
} else {
    Write-Host "`n[1/3] g++ compiler not found in PATH." -ForegroundColor Yellow
    Write-Host "NOTE: Please compile server.cpp manually using your C++ IDE (Visual Studio, CLion, Dev-C++)" -ForegroundColor Yellow
    Write-Host "      Ensure you link against the 'ws2_32' library (required for Windows socket API)." -ForegroundColor Yellow
}

# 2. Launch Backend (if compiled)
if ($compileSuccess) {
    Write-Host "`n[2/3] Launching C++ Backend API Server on http://localhost:5000..." -ForegroundColor Green
    Start-Process -FilePath ".\server.exe" -NoNewWindow -PassThru
    # Give the backend server a moment to bind to socket
    Start-Sleep -Seconds 1
} else {
    if (Test-Path ".\server.exe") {
        Write-Host "`n[2/3] Found existing server.exe. Launching..." -ForegroundColor Green
        Start-Process -FilePath ".\server.exe" -NoNewWindow -PassThru
        Start-Sleep -Seconds 1
    } else {
        Write-Host "`n[2/3] Backend server executable not found." -ForegroundColor Yellow
        Write-Host "      Skipping auto-launch. Please run server.exe manually on port 5000." -ForegroundColor Yellow
    }
}

# 3. Launch Frontend React
Write-Host "`n[3/3] Launching React Vite Frontend Development Server..." -ForegroundColor Green
Set-Location -Path "$root\frontend"
npm run dev
