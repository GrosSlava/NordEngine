@echo off



rem Set absolute path to project root
set ProjectRoot=%~dp0../



:begin
cmake -S %ProjectRoot% -B %ProjectRoot%/Intermediate --install-prefix %ProjectRoot%/Intermediate/Install -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CONFIGURATION_TYPES:STRING=Debug -DCMAKE_TRY_COMPILE_CONFIGURATION:STRING=Debug

:end
pause