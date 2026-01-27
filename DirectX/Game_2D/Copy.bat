robocopy ".\..\Core\Bin" ".\..\Bin" *.dll /S /XO /NJH /NJS

:: robocopy의 반환 코드가 8보다 작으면 성공으로 간주하고 exit code를 0으로 리셋
if %ERRORLEVEL% LSS 8 set ERRORLEVEL=0
