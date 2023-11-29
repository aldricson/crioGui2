@echo off
:: Set the variables based on command line arguments
set HOST=%1
set USER=%2
set PASS=%3
set PORT=%4
set COMMAND=%5
set PARAMETER1=%6
set PARAMETER2=%7

:: Check if the command is to get the module list
if "%COMMAND%" == "getModuleList" (
    :: Use plink to execute a command that lists .ini files in the /home/dataDrill directory on the remote machine
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "cd /home/dataDrill; ls | grep \"^NI.*\.ini$\""
) else if "%COMMAND%" == "dir" (
    :: Use plink to list the contents of a directory on the remote machine
    :: %PARAMETER1% is the path of the folder
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "cd %PARAMETER1%; ls"
) else if "%COMMAND%" == "serverState" (
    :: Use plink to read the server status on the remote machine
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "cd /home/dataDrill; sh dataDrillStatus.sh"
) else if "%COMMAND%" == "startServer" (
    :: Use plink to start the server on the remote machine
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "cd /home/dataDrill; sh dataDrillStart.sh"
) else if "%COMMAND%" == "stopServer" (
    :: Use plink to stop the server on the remote machine
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "pkill dataDrill"
) else if "%COMMAND%" == "downloadModule" (
    :: Use pscp to download a file from the remote machine to the local machine
    :: %PARAMETER1% is the full path of the source file on the remote machine
    :: %PARAMETER2% is the full path for the file on the local machine
    pscp -P %PORT% -pw %PASS% %USER%@%HOST%:%PARAMETER1% %PARAMETER2%
) else if "%COMMAND%" == "downloadModbusSetup" (
    :: Use pscp to download a file from the remote machine to the local machine
    :: %PARAMETER1% is the full path of the source file on the remote machine
    :: %PARAMETER2% is the full path for the file on the local machine
    pscp -P %PORT% -pw %PASS% %USER%@%HOST%:%PARAMETER1% %PARAMETER2%
)
