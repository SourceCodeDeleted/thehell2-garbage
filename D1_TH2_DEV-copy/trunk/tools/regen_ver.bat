@set TH1_REV=230
@if exist %SVN_PATH% goto SVN
@if exist "%ProgramFiles%\TortoiseSVN\bin\SubWCRev.exe" goto TSVN
@if exist "%ProgramW6432%\TortoiseSVN\bin\SubWCRev.exe" goto TSVN64
@goto NOSVN

:SVN
@set SVN_REV=0
@"%SVN_PATH%/bin/svnversion.exe" -n .\ > tmp
@set /p SVN_REV=< tmp
@del tmp
@echo #define TH1_REV "%TH1_REV%" >version
@echo #define TH2_REVISION %SVN_REV% >>version
@echo #define TH1_REVISION '1%TH1_REV%' >>version
@echo #ifdef TH1 >>version
@echo #define THE_HELL_VERSION_INT 1,%TH1_REV%,%SVN_REV%,0 >>version
@echo #define THE_HELL_VERSION_STRING "1.%TH1_REV%.%SVN_REV%" >>version
@echo #else >>version
@echo #define THE_HELL_VERSION_INT 0,%SVN_REV%,0 >>version
@echo #define THE_HELL_VERSION_STRING "v0.%SVN_REV%" >>version
@echo #endif >> version
@echo #define THE_HELL_VERSION_HUMAN_STRING PRJ_NAME " " THE_HELL_VERSION_STRING >>version
@goto CHKVER

:TSVN
@echo #define TH1_REV "%TH1_REV%" >version
@echo #define TH2_REVISION $WCRANGE$ >>version
@echo #define TH1_REVISION '1%TH1_REV%' >>version
@echo #ifdef TH1 >>version
@echo #define THE_HELL_VERSION_INT 1,%TH1_REV%,$WCRANGE$,0 >>version
@echo #define THE_HELL_VERSION_STRING "1.%TH1_REV%.$WCRANGE$" >>version
@echo #else >>version
@echo #define THE_HELL_VERSION_INT 0,$WCRANGE$,0 >>version
@echo #define THE_HELL_VERSION_STRING "v0.$WCRANGE$" >>version
@echo #endif >> version
@echo #define THE_HELL_VERSION_HUMAN_STRING PRJ_NAME " " THE_HELL_VERSION_STRING >>version
@"%ProgramFiles%\TortoiseSVN\bin\SubWCRev.exe" . version version
@goto CHKVER

:TSVN64
@echo #define TH1_REV "%TH1_REV%" >version
@echo #define TH2_REVISION $WCRANGE$ >>version
@echo #define TH1_REVISION '1%TH1_REV%' >>version
@echo #ifdef TH1 >>version
@echo #define THE_HELL_VERSION_INT 1,%TH1_REV%,$WCRANGE$,0 >>version
@echo #define THE_HELL_VERSION_STRING "1.%TH1_REV%.$WCRANGE$" >>version
@echo #else >>version
@echo #define THE_HELL_VERSION_INT 0,$WCRANGE$,0 >>version
@echo #define THE_HELL_VERSION_STRING "v0.$WCRANGE$" >>version
@echo #endif >> version
@echo #define THE_HELL_VERSION_HUMAN_STRING PRJ_NAME " " THE_HELL_VERSION_STRING >>version
@"%ProgramW6432%\TortoiseSVN\bin\SubWCRev.exe" . version version
@goto CHKVER

:NOSVN
@echo #define TH1_REV "%TH1_REV%" >version
@echo #define TH2_REVISION 0 >>version
@echo #define TH1_REVISION '1%TH1_REV%' >>version
@echo #ifdef TH1 >>version
@echo #define THE_HELL_VERSION_INT 1,%TH1_REV%,0,0 >>version
@echo #define THE_HELL_VERSION_STRING "1.%TH1_REV%.0" >>version
@echo #else >>version
@echo #define THE_HELL_VERSION_INT 0,0,0 >>version
@echo #define THE_HELL_VERSION_STRING "v0.0" >>version
@echo #endif >> version
@echo #define THE_HELL_VERSION_HUMAN_STRING PRJ_NAME " " THE_HELL_VERSION_STRING >>version
@goto CHKVER

:CHKVER
@if not exist version.h goto NEWVER 
@fc version version.h >nul
@if %ERRORLEVEL%==0 goto OLDVER 
@del version.h

:NEWVER
@ren version version.h
@exit

:OLDVER
@del version