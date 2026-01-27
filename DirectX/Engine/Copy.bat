rd /s /q ..\Core\Inc 2>nul
robocopy . ..\Core\Inc *.h *.inl *.cs *.hpp /E

rd /s /q ..\Bin\EngineAsset 2>nul
robocopy ..\Core\Bin\EngineAsset ..\Bin\EngineAsset /E

exit /b 0
