@echo off
set CurrentDIR=%CD%
pushd C:\Program Files\teraterm
ttpmacro.exe /V %CurrentDIR%\LoadCode.ttl %CurrentDIR:\=/% Release Halt
popd
set OLD_PATH=%PATH%
PATH=C:\NMake;%PATH%
start sde-insight --command=init.gdb %CurrentDIR%/DemoKit_Data/Release/DemoKit.axf
PATH=%OLD_PATH%
set CurrentDIR=
set OLD_PATH=