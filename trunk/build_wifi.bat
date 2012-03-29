@echo off
erase .\Makefile
copy .\makefiles\Makefile_Wifi .\Makefile
make
pause
