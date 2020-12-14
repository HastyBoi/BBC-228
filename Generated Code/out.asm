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
_L0 DWORD 1
_L1 DWORD 2
_L2 DWORD 228
_L3 DWORD 1488
_L4 DWORD 0

.data
_mainb DWORD 0

.code
_shok PROC 
push _L0
pop eax 
ret 0
_shok ENDP

_kok PROC _koka : DWORD
push _koka
call _shok
push eax
pop eax
pop ebx
add eax, ebx
push eax
pop eax 
ret 4
_kok ENDP

_main PROC 
push _L1
call _kok
push eax
push _L1
pop ebx
mov edx, 0
pop eax
.if ebx == 0
push offset _DIVIDE_BY_ZERO_EXCEPTION
call _echoStr
invoke ExitProcess, -1
.endif
div ebx
push edx
pop _mainb
push _L2
push _L3
pop ebx
pop eax
sub eax, ebx
push eax
pop _mainb
push _mainb
call _echoInt
push _L4
call ExitProcess
_main ENDP

end _main
