@echo off
set arg1=%1
set arg2=%2
set arg3=%3
set arg4=%4
shift
shift
shift
shift
set COMPILER_PATH=%CD%build\%arg4%
set C_INCLUDE_PATH=%CD%include
set PSYQ_PATH=%CD%build
@echo on
%CD%build\psylink.exe /l build/lib /c /n 4000 @LINKDOS.TXT,sound.cpe,sound.sym,sound.map