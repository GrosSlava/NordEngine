@echo off



rem Set absolute path to project root
set ProjectRoot=%~dp0../



:begin
cmake -S %ProjectRoot% -B %ProjectRoot%/Intermediate --install-prefix %ProjectRoot%/Intermediate/Install -DCMAKE_BUILD_TYPE=Release -DCMAKE_CONFIGURATION_TYPES:STRING=Release -DCMAKE_TRY_COMPILE_CONFIGURATION:STRING=Release

:end
pause