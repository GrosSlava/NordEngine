@echo off




rem Set absolute path to engine
set EngineRoot=%~dp0

rem Set modules names in order to build. For plugins use Plugins\<ModuleName>.
rem Example: set Modules=Game;Plugins\Module1;Plugins\Module2;Game2
set Modules=





rem ....................................................................................... rem
rem ....................................................................................... rem


call "%EngineRoot%\Tools\BuildEngine\GetMSBuildPath.bat"
if errorlevel 1 goto Error_NoVisualStudioEnvironment

python "%EngineRoot%\Tools\BuildEngine\ProjectBuildToolWindows.py" %~dp0 %EngineRoot% %MSBUILD_PATH% %Modules%
pause
goto Exit




:Error_NoVisualStudioEnvironment
echo ERROR: Missing Visual Studio 2015 or newer.
pause
goto Exit

:Exit
exit /B 1