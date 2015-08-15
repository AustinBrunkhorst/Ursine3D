@echo off

:: command to check if admin priviledges are available
net session >nul 2>&1

if %errorLevel% neq 0 (
    :: create popup
    start cmd /C "color 0C && echo Visual Studio must be run as an administrator to create symbolic links. && echo. && pause"

    :: exit with error
    exit /B 1
)

set link=%1
set target=%2

echo SymLink: %link% ==^> %target%

mklink /J %link% %target% >nul 2>&1

exit /B 0
