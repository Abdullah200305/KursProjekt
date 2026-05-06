@echo off

cd /d "client"

for /L %%i in (1,1,4) do (
    start "" cmd /k client.exe
)