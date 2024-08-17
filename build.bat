@echo off

IF NOT EXIST build (
    mkdir build
)

pushd build

cl /nologo ^
/O2 ^
/std:c++latest /Zc:__cplusplus /utf-8 ^
/external:anglebrackets /external:W0 /analyze:external- /Wall ^
/wd4711 /wd4710 ^
/EHa ^
/D "_CONSOLE" /D "_UNICODE" /D "UNICODE" ^
/MT ^
/Fe:running_as_admin.exe ^
..\main.cpp ^
/link /SUBSYSTEM:CONSOLE Advapi32.lib

popd
