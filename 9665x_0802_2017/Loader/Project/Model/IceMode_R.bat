@echo off
set CurrentDIR=%CD%
pushd C:\Program Files\teraterm
ttpmacro.exe /V %CurrentDIR%\IceMode.ttl
popd
set OLD_PATH=%PATH%
PATH=C:\NMake;%PATH%
start sde-insight --command=init_IceMode.gdb %CurrentDIR%/Loader650_Data/Release/Loader650.axf
PATH=%OLD_PATH%
set CurrentDIR=
set OLD_PATH=