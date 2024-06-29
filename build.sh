mkdir -p build

cp psy-q/3.5/PSYQ/CCPSX.EXE build
cp psy-q/3.6/PSYQ/BIN/ASPSX.EXE build
cp psy-q/3.5/PSYQ/PSYQ.INI build
cp psy-q/3.6/PSYQ/BIN/PSYLINK.EXE build

cp psy-q/3.5/PSX/BIN/CPE2X.EXE build

cp psy-q/cc_2.60/cc1psx.exe build
cp psy-q/cc_2.60/cpppsx.exe build

cp -r psy-q/3.5/PSX/LIB build

cp psy-q/3.6/PSYQ/BIN/ASMPSX.EXE build

cp -r vb build

dosemu -quiet -dumb -f ./dosemurc -K . -E "build.bat test.c test.o -O2 3.5"
dosemu -quiet -dumb -f ./dosemurc -K . -E "asmpsx.bat"
unix2dos -n link.txt linkdos.txt
dosemu -quiet -dumb -f ./dosemurc -K . -E "link.bat"
dosemu -quiet -dumb -f ./dosemurc -K . -E "cpe.bat"
