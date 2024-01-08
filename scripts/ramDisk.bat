@echo off
REM Check for administrator privileges
REM >nul suppresses printing error messages
REM >nul 2 suppresses printing correct messages
REM >nul 2>&1 treats error messages as correct messages, and suppresses printing
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"

REM Permission check: when current errorlevel is 0, it's administrator privileges
REM NEQ means 'not equal'
if '%errorlevel%' NEQ '0' (
    goto UACPrompt
) else (
    goto gotAdmin
)

:UACPrompt
  REM Generate VBS to acquire permissions
  echo Set UAC= CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
  echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%temp%\getadmin.vbs"
  "%temp%\getadmin.vbs"
  REM Restart batch file
  exit /B

:gotAdmin
  REM Remove getadmin.vbs
  if exist "%temp\getadmin.vbs%" ( del "%temp\getadmin.vbs%" )
  REM Admin privileges acquired, add content to run as admin below
  echo You have administrator privileges
    REM Debug: Echo the full imdisk command
    reg add "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\Explorer" /v NoDrives /t REG_DWORD /d 0 /f
    imdisk -a -t vm -s 1M -m A: -p "/fs:fat /q /y /v:DATADRILL"
    reg add "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\Explorer" /v NoDrives /t REG_DWORD /d 1 /f
