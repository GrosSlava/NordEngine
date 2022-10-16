@echo off




rem Set absolute path to engine
set EngineRoot=%~dp0





rem .......................................................................................rem
rem .......................................................................................rem


python "%EngineRoot%\Tools\ClearIntermediateFiles\ClearIntermediate.py" %~dp0
pause