.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
ExitProcess PROTO : SDWORD
includelib ../Debug/stdlib.lib
_echoInt PROTO : SDWORD
_echoStr PROTO : SDWORD
_parseInt PROTO : SDWORD
_concat PROTO : SDWORD, : SDWORD

.stack 4096

.const
_DIVIDE_BY_ZERO_EXCEPTION BYTE 'деление на 0',0
_L0 SDWORD 0
_L1 BYTE 'fols', 0

.data
_mainshs SDWORD 0

.code
_main PROC 
.if _mainshs != 0
push _mainshs
call _echoStr
.else
.endif
push offset _L1
call _echoStr
push _L0
call ExitProcess
_main ENDP

end _main
