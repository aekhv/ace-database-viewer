@echo off
rem *** Run this script from MinGW 32-bit environment! ***

set fb_bin=c:\Program Files (x86)\Firebird\Firebird_2_5\bin
set fb_include=c:\Program Files (x86)\Firebird\Firebird_2_5\include
set fb_lib=c:\Program Files (x86)\Firebird\Firebird_2_5\lib\fbclient_ms.lib
set sql=c:\Qt\5.15.2\Src\qtbase\src\plugins\sqldrivers

for %%A in ("%fb_include%") do set fb_include=%%~sA
for %%A in ("%fb_lib%") do set fb_lib=%%~sA
set PATH=%PATH%;%fb_bin%
cd /d %sql%

qmake -- IBASE_INCDIR="%fb_include%" IBASE_LIBS="%fb_lib%"
mingw32-make sub-ibase 
mingw32-make install
