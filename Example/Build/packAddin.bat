@echo off

SET WinRARLocation=
SET DLLsPrefix=%1

cd Build

IF "%DLLsPrefix%"=="" (
	ECHO Specify addin name!
	EXIT /B 2
)

CALL :CheckForWinRAR "%ProgramFiles%\WinRAR"
CALL :CheckForWinRAR "%ProgramW6432%\WinRAR"
IF NOT EXIST %WinRARLocation%\WinRAR.exe (
	ECHO Unable to find WinRAR!
	EXIT /B 1
)


IF NOT EXIST "MANIFEST.XML" (
	ECHO Creating manifest.xml...
	ECHO ^<?xml version="1.0" encoding="UTF-8" ?^> > MANIFEST.XML
	ECHO ^<bundle xmlns="http://v8.1c.ru/8.2/addin/bundle"^> >> MANIFEST.XML
	ECHO ^<component os="Windows" path="%DLLsPrefix%_Release_Win32.dll" type="native" arch="i386" /^> >> MANIFEST.XML
	ECHO ^<component os="Windows" path="%DLLsPrefix%_Release_x64.dll" type="native" arch="x86_64" /^> >> MANIFEST.XML
	ECHO ^</bundle^> >> MANIFEST.XML
)

SET AddinFiles=
IF EXIST "%DLLsPrefix%_Release_Win32.dll" SET AddinFiles=%AddinFiles% %DLLsPrefix%_Release_Win32.dll
IF EXIST "%DLLsPrefix%_Release_x64.dll" SET AddinFiles=%AddinFiles% %DLLsPrefix%_Release_x64.dll

%WinRARLocation%\WinRAR.exe u -afzip -idq -m0 %DLLsPrefix%.zip %AddinFiles% MANIFEST.XML
GOTO ENDOFSCRIPT

:: Subroutines
:CheckForWinRAR
IF NOT EXIST %WinRARLocation%\WinRAR.exe (
	SET WinRARLocation="%~1"
)

:ENDOFSCRIPT