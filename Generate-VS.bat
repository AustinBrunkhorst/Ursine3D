@echo off

set vsDir="Build-VS-2015"

echo.
echo ===== Cleaning =====
echo.

call "Clean-VS.bat"

if NOT EXIST %vsDir% mkdir %vsDir%

cd %vsDir%

echo.
echo ===== Building =====
echo.

cmake -G "Visual Studio 14" "../Source"

cd ..