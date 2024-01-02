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
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "cd /home/dataDrill; ls | grep \"^NI.*\.ini$\""
) else if "%COMMAND%" == "dir" (
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "cd %PARAMETER1%; ls"
) else if "%COMMAND%" == "serverState" (
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% -batch "cd /home/dataDrill; sh dataDrillStatus.sh"
) else if "%COMMAND%" == "startServer" (
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "cd /home/dataDrill; sh dataDrillStart.sh"
) else if "%COMMAND%" == "totalCPU" (
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% -batch "cd /home/dataDrill; sh dataDrillTotalCPU.sh"
) else if "%COMMAND%" == "dataDrillCPU" (
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% -batch "cd /home/dataDrill; sh dataDrillCPU.sh"
) else if "%COMMAND%" == "totalHDD" (
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% -batch "cd /home/dataDrill; sh dataDrillTotalHDD.sh"
) else if "%COMMAND%" == "dataDrillHDD" (
       plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% -batch "cd /home/dataDrill; sh dataDrillHDD.sh"
) else if "%COMMAND%" == "dataDrillTotalRamUsage" (
       plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% -batch "cd /home/dataDrill; sh dataDrillTotalRamUsage.sh"
) else if "%COMMAND%" == "dataDrillRamUsage" (
              plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% -batch "cd /home/dataDrill; sh dataDrillRamUsage.sh"
) else if "%COMMAND%" == "dataDrillGlobalStats" (
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% -batch "cd /home/dataDrill; sh dataDrillGlobalStats.sh"
) else if "%COMMAND%" == "stopServer" (
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "pkill -f dataDrill"
) else if "%COMMAND%" == "downloadModule" (
    pscp -P %PORT% -pw %PASS% %USER%@%HOST%:%PARAMETER1% %PARAMETER2%
) else if "%COMMAND%" == "downloadModbusSetup" (
    pscp -P %PORT% -pw %PASS% %USER%@%HOST%:%PARAMETER1% %PARAMETER2%
) else if "%COMMAND%" == "downloadModbusMapping" (
    pscp -P %PORT% -pw %PASS% %USER%@%HOST%:%PARAMETER1% %PARAMETER2%
) else if "%COMMAND%" == "uploadModbusMapping" (
        pscp -P %PORT% -pw %PASS% %USER%@%HOST%:%PARAMETER1% %PARAMETER2%
) else if "%COMMAND%" == "getInterfacesIPv4" (
    plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "ip addr show | grep inet | grep -v inet6 | awk '{print $2}' | cut -d'/' -f1"
)
