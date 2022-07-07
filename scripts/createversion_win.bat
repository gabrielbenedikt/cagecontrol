@echo off
cd ..\src
for /f %%i in ('git describe') do set VAR1=%%i
git show -s --format=%%cd --date=format:"%%Y%%m%%d%%H%%M" > tmp.txt
set /p VAR2=<tmp.txt

SET BUILDDATE=%date:~-4%%date:~3,2%%date:~7,2%%time:~0,2%%time:~3,2%
echo #ifndef VERSION_H > version.h
echo #define VERSION_H >> version.h
echo( >> version.h
echo #define VERSION_GIT "%VAR1%" >> version.h
echo #define VERSION_GIT_DATE %VAR2% >> version.h
echo #define VERSION_BUILD_DATE %BUILDDATE% >> version.h
echo( >> version.h
echo #endif >> version.h
