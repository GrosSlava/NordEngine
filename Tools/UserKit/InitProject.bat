@echo off


rem Set absolute path to engine
set EngineRoot=%~dp0

rem Set name of project
set ProjectName=



rem ....................................................................................... rem
rem ....................................................................................... rem
rem ....................................................................................... rem


python "%EngineRoot%\Tools\NewProjectGenerator\InitNewProject.py" %~dp0 %EngineRoot% %ProjectName%
pause