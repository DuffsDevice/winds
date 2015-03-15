@echo off

REM Enter Version
set /p version= "Please enter version for filenames: "
echo Building Version %version%

REM Delete all files in build folder
cd build
del WinDS_*

REM Create Documentation
doxygen.exe doxyinfo

REM Create Archives
7za.exe a -ttar "WinDS_%version%.tar" "../Hello World.lua" "../LICENCE" "../../WinDS.nds" "../README" "../windows/" "documentation"
7za a -tgzip "WinDS_%version%.tar.gz" "WinDS_%version%.tar" -mx=9

REM Delete temporary
del *.tar
pause