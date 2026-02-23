@ECHO off
cls
.\Premake5\Premake5.exe vs2022

xcopy /s /d /Y .\..\assets .\..\bin\debug\x32
xcopy /s /d /Y .\..\assets .\..\bin\release\x32

xcopy /s /d /Y .\..\assets .\..\build\PR03_00_Template\vs2022
xcopy /s /d /Y .\..\assets .\..\build\PR03_01_Demo\vs2022
xcopy /s /d /Y .\..\assets .\..\build\PR03_02_FirstScene\vs2022
xcopy /s /d /Y .\..\assets .\..\build\PR03_03_SolarSystem\vs2022
xcopy /s /d /Y .\..\assets .\..\build\PR03_04_BasicMaterial\vs2022
xcopy /s /d /Y .\..\assets .\..\build\PR03_05_CustomMaterial\vs2022
pause
