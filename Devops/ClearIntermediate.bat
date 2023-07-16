@echo off



rem Set absolute path to project root
set ProjectRoot=%~dp0../



:begin
python "%ProjectRoot%\Devops\Scripts\ClearIntermediate.py" %ProjectRoot%

:end
pause