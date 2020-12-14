.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
ExitProcess PROTO : DWORD
includelib ../Debug/stdlib.lib
_echoInt PROTO : DWORD
_echoStr PROTO : DWORD
_parseInt PROTO : DWORD
_concat PROTO : DWORD, : DWORD

.stack 4096

.const
_DIVIDE_BY_ZERO_EXCEPTION BYTE 'деление на 0',0
_L0 BYTE 'chel ne levi', 0
_L1 BYTE 'pervi ', 0
_L2 DWORD 0

.data
_klasout DWORD 0
_mainres DWORD 0

.code
_klas PROC _klass : DWORD
push _klass
invoke _concat, _klass, _klass
push eax
pop _klasout
push offset _L0
invoke _concat, _klasout, offset _L0
push eax
pop eax 
ret 4
_klas ENDP

_main PROC 
invoke _klas, offset _L1
push eax
pop _mainres
push _mainres
call _echoStr
push _L2
call ExitProcess
_main ENDP

end _main
