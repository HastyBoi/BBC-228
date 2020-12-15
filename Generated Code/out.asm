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
_L0 BYTE '2', 0
_L1 SDWORD 3
_L2 BYTE 'ложь', 0
_L3 BYTE 'hello ', 0
_L4 BYTE 'empty', 0
_L5 SDWORD 0

.data
_msgmessage SDWORD 0
_mainx SDWORD 0
_mainsum SDWORD 0
_mainmessage SDWORD 0

.code
_findSum PROC _findSumx : SDWORD, _findSumy : SDWORD
push _findSumx
push _findSumy
pop eax
pop ebx
add eax, ebx
push eax
pop eax 
ret 8
_findSum ENDP

_msg PROC _msgs : SDWORD
push _msgs
invoke _concat, _msgs, _msgs
push eax
pop _msgmessage
push _msgs
invoke _concat, _msgmessage, _msgs
push eax
pop eax 
ret 4
_msg ENDP

_main PROC 
invoke _parseInt, offset _L0
push eax
pop _mainx
push _L1
invoke _findSum, _mainx, _L1
push eax
pop _mainsum
.if _mainsum != 0
push _mainsum
call _echoInt
.else
push offset _L2
call _echoStr
.endif
invoke _msg, offset _L3
push eax
pop _mainmessage
.if _mainmessage != 0
push _mainmessage
call _echoStr
.else
push offset _L4
call _echoStr
.endif
push _L5
call ExitProcess
_main ENDP

end _main
