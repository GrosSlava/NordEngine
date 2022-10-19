@echo off


rem Set absolute path to engine
set EngineRoot=%~dp0



rem ....................................................................................... rem
rem ....................................................................................... rem
rem ....................................................................................... rem


call "%EngineRoot%\Tools\BuildEngine\GetMSBuildPath.bat"
if errorlevel 1 goto Error_NoVisualStudioEnvironment

python "%EngineRoot%\Tools\BuildEngine\ProjectBuildToolWindows.py" %~dp0 %MSBUILD_PATH%
pause
goto Exit



:Error_NoVisualStudioEnvironment
echo ERROR: Missing Visual Studio 2015 or newer.
pause
goto Exit

:Exit
exit /B 1