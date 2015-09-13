@echo off

set vsDir="Build-VS-2015-x64"

if NOT EXIST %vsDir% mkdir %vsDir%

rd /s /q %vsDir%