@echo off
title NetSwimmer Launchpad
echo.
echo [*] Starting TOR proxy service...
start /min tor\tor.exe
timeout /t 10 >nul

echo [*] Launching net_swimmer...
.\build\net_swimmer.exe

echo.
pause
