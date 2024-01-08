:: BatchGotAdmin

:-------------------------------------

REM  --> Check for permissions

>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"

REM --> If error flag set, we do not have admin.

if '%errorlevel%' NEQ '0' (

    echo Requesting administrative privileges...

    goto UACPrompt

) else ( goto gotAdmin )

:UACPrompt

    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"

    echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"

    exit /B

:gotAdmin

    if exist "%temp%\getadmin.vbs" ( del "%temp%\getadmin.vbs" )

    pushd "%CD%"

    CD /D "%~dp0"



if not "%1"=="7" start /min cmd /c ""%~0" 7 %*" & exit /b
set F=%USERPROFILE%\ImDisk%TIME::=%
if exist "%TEMP%" set F=%TEMP%\ImDisk%TIME::=%
extrac32.exe /e /l "%F%" "%~dp0files.cab"
"%F%\config.exe" /silent %3 %4
rd /s /q "%F%"
