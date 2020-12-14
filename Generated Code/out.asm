.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
ExitProcess PROTO : DWORD
includelib ../Debug/stdlib.lib
_echoInt PROTO : DWORD
_echoStr PROTO : DWORD
_parseInt PROTO : DWORD
_concat PROTO : DWORD

.stack 4096

.const
_DIVIDE_BY_ZERO_EXCEPTION BYTE 'деление на 0',0
_L0 BYTE '228', 0
_L1 BYTE 'false', 0
_L2 DWORD 0

.data
_maina DWORD 0

.code
_main PROC 
push offset _L0
call _parseInt
push eax
pop _maina
.if _maina != 0
push _maina
call _echoInt
.else
push offset _L1
call _echoStr
.endif
push _L2
call ExitProcess
_main ENDP

end _main
