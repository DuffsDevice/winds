@echo off
erase .\Makefile
copy .\makefiles\Makefile_WifiLess .\Makefile
make
pause