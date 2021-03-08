REM Make a DLL
REM CALL cl /LD checksums.c

REM Make a static lib
CALL cl /c checksums.c
CALL lib checksums.obj
