@echo off



rem Set absolute path to project root
set ProjectRoot=%~dp0../



:begin
cmake --build %ProjectRoot%/Intermediate --target AllTests -j%NUMBER_OF_PROCESSORS%
ctest --test-dir %ProjectRoot%/Intermediate -C Debug --output-on-failure

:end
pause