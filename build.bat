@echo off
set arg1=%1
set arg2=%2
set arg3=%3
set arg4=%4
shift
shift
shift
shift
set COMPILER_PATH=%CD%build\
set C_INCLUDE_PATH=%CD%include
set PSYQ_PATH=%CD%build
@echo on
%CD%build\ccpsx.exe %arg3% -O2 -g0 -G0 -funsigned-char -c -I./src/snd %arg1% -o%arg2%