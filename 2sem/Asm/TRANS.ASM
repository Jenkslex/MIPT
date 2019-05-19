.model tiny
.code
org 100h

;Main
;=================================================================
start:		call ChooseOrd

		push offset inputbuff
		call readstr
		add sp, 2

		push ax
		push offset inputbuff
		call readnum
		add sp, 4

		mov bx, ax

		call Endl
		call ChooseOrd

		push bx
		push ax
		push offset outputbuff
		call Recnum
		add sp, 6

		push di
		call outstr
		add sp, 2

		mov ax, 4c00h
		int 21h

;================================================================


;================================================================
;Functions
;================================================================
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;Name:   Readstr
;Action: Record a string into the buff.
;Entry:  Buff
;Exit:   None
;Destr:  AH
;----------------------------------------------------------------
Readstr proc, string_in
		
		push bp
		mov bp, sp

      		mov dx, string_in

		mov ah, 0Ah
		int 21h

		pop bp
		ret
endp
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;Name:   Outstr
;Action: Output the string from the buff.
;Entry:  DX -> Buff
;Exit:   None
;Destr:  AH, DX
;----------------------------------------------------------------
Outstr proc, string_out 	
		
		push bp
		mov bp, sp
		
		mov dx, string_out

		mov ah, 09h
		int 21h

		pop bp
		ret
endp
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;Name:   Endl
;Action: Endl.
;Entry:  None
;Exit:   None
;Destr:  AH, DX
;----------------------------------------------------------------
Endl proc
   	
		lea dx, newstring
		mov ah, 09h
		int 21h

		ret
endp
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;Name:   ChooseOrd
;Action: Choose the system order from input symbol.
;Entry:  None
;Exit:   AL -> system order
;Destr:  AH
;----------------------------------------------------------------
ChooseOrd proc
	
		mov ah, 01h
		int 21h

		cmp al, 'b'
		jne ChooseOrdNotB
		
		mov al, 02h
		ret


ChooseOrdNotB:	cmp al, 'd'
		jne ChooseOrdNotD
		
		mov al, 0Ah
		ret


ChooseOrdNotD:	cmp al, 'h'
		jne ChooseOrdNotH
		
		mov al, 10h
		ret


ChooseOrdNotH:	cmp al, 'o'
		jne ChooseOrdExit
	
		mov al, 08h
		ret

ChooseOrdErr:   xor al, al
		
ChooseOrdExit:	ret
endp
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;Name:   ReadNum
;Action: Read number in the certain system from the string.
;Entry:  String number 
;        The system order
;Exit:   AX -> Tne number in certain system
;Destr:  BX, CX, DL, SI  
;----------------------------------------------------------------
ReadNum proc, string_in, order
	
		push bp
		mov bp, sp
		
		mov si, string_in
		mov dx, order
		                     ;--------------+
		cmp dl, 01h          ;If order == 0 +-----------+
		jbe ReadNumWrong     ;--------------+           |
		                     ;                          |
		                     ;                          | 
		inc si               ;Move to begin of string   |
		xor ax, ax           ;AX = 0                    |
		xor bx, bx           ;BX = 0                    |
		                     ;                          |
		                     ;                          | 
		mov dh, dl           ;--------------+           |
		sub dh, 1            ;If order      |           |
		and dh, dl           ;= 2^k         +---------+ |
		cmp dh, 00h          ;(n&(n-1)) == 0|         | |
		je ReadNumPow2       ;--------------+         | |
		                     ;                        | |     
		                     ;                        | |
		                     ;==== If order != 2^k ===| |
		mov ch, dl           ;Set the               ||| |
		mov cl, [si]         ;counters              ||| |
                inc si               ;                      ||| |
                xor dh, dh           ;                      ||| |
                                     ;                      ||| |
ReadNumStLoop: 	cmp cl, 00h          ;while (CL != 0)       ||| |
		je ReadNumEnd        ;{                     ||| |
		                     ;                      ||| |
		mul dx               ;  DX:AX = AX*DX;      ||| |
		mov bl, [si]         ;  BL = *SI;           ||| |
		                     ;                      ||| |
		push bx              ;                      ||| |
		call TrAToNum        ;  Transfer            ||| |
		add sp, 2            ;                      ||| |
		                     ;                      ||| |
		add ax, bx           ;  AX = AX + BX;       ||| |
	        mov dl, ch           ;  DL = CH;            ||| |
	        xor dh, dh           ;  DH = DH;            ||| |
	                             ;                      ||| |
	        dec cl               ;  CL--;               ||| |
		inc si               ;  SI++;               ||| |
		jmp ReadNumStLoop    ;}                     ||| |
		                     ;                      ||| |
	                             ;========================| |
	                             ;                        | |
	                             ;==== If order == 2^k <--+ |
ReadNumPow2:	mov ch, [si]         ;Set the               ||  |
		xor cl, cl           ;counters              ||  |
		inc si	             ;                      ||  |
		                     ;                      ||  |
		                     ;                      ||  |
ReadNumP2Cond1:	cmp dl, 01h          ;While (DH != 1)       ||  |
		je ReadNumP2Cond2    ;{	                    ||  |
		                     ;                      ||  |
		shr dl, 1            ; 	DH >> 1;            ||  |
		inc cl               ;  CL--;               ||  |
		                     ;                      ||  |
		jmp ReadNumP2Cond1   ;}                     ||  |
	                             ;                      ||  |
	                             ;                      ||  |
ReadNumP2Cond2:	cmp ch, 00h          ;while (ch != 0)       ||  |
		je ReadNumEnd        ;{                     ||  |
		                     ;	                    ||  |
		shl ax, cl           ;  AX << CL;           ||  |
		mov bl, [si]         ;  BL = *SI;           ||  |
		                     ;                      ||  |
		push bx              ;                      ||  |
		call TrAToNum        ;  Transfer            ||  |
		add sp, 2            ;                      ||  |
		                     ;                      ||  |
		xor ax, bx           ;  AX = AX ^ BX;       ||  |
		                     ;                      ||  |
		inc si               ;  SI++;               ||  |
		dec ch	             ;  CH--:               ||  |
			             ;                      ||  |
		jmp ReadNumP2Cond2   ;}                     ||  |
		                     ;======================||  |
		                     ;                          |
ReadNumWrong:	xor ax, ax           ;<--- If order is wrong <--+
                                     
                                     
ReadNumEnd:	pop bp
		ret                  ; Exit
endp
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;Name:   TrAToNum
;Action: Transfer the figure from ASCII-code to digit.
;Entry:  ASCII-code of number
;Exit:   BL -> The number
;Destr:  BH
;----------------------------------------------------------------
TrAToNum proc, Ascii_code

		push bp
		mov bp, sp	
		
		mov bx, Ascii_code

		cmp bl, '0'
		jb  TrAToNumUndS
		cmp bl, '9'
		ja  TrAToNumNotFig

		sub bl, '0'
		jmp TrAToNumEnd

TrAToNumNotFig:    cmp bl, 'A'
		jb  TrAToNumUndS
		cmp bl, 'F'
		ja  TrAToNumUndS

		sub bl, 37h          ; 'A-F' - 31h = A-F
		jmp TrAToNumEnd

TrAToNumUndS:   mov bl, 10h

TrAToNumEnd:	pop bp
		ret
endp
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;Name:   RecNum
;Action: Record number in the string in the certain system.
;Entry:  Outbuff
;        The system order
;	 The number in certain system 
;Exit:   DI -> Outbuff from begin
;Destr:  AX, BX, CX, DX  
;----------------------------------------------------------------
RecNum proc, strig_out, order, number	

		push bp
		mov bp, sp
		
		mov di, string_out
		mov dx, order
		mov ax, number
				     ;--------------+
		cmp dl, 01h          ;If order == 0 +-----------+
		jbe RecNumWrong      ;--------------+           |
		                     ;                          |
		                     ;                          | 
		add di, 09h          ;Move to the end of string |
		                     ;                          |
		                     ;                          | 
		mov dh, dl           ;--------------+           |
		sub dh, 1            ;If order      |           |
		and dh, dl           ;= 2^k         +---------+ |
		cmp dh, 00h          ;(n&(n-1)) == 0|         | |
		je RecNumPow2        ;--------------+         | |
		                     ;                        | |     
		                     ;==== If order != 2^k ===| |
		xor ch, ch           ;CH = 0;               ||| |
		mov cl, dl           ;CL = DL;              ||| |
		xor dx, dx           ;DX = 0;               ||| |
		                     ;                      ||| |
RecNumStCond1: 	cmp ax, 0000h        ;while (AX != 0)       ||| |
		je RecNumEnd         ;{                     ||| |
		                     ;                      ||| |
		div cx               ;	AX = DX:AX/[order]  ||| |
		mov bl, dl           ;  BL = DL;            ||| |
		                     ;                      ||| |
		push bx              ;                      ||| |
		call TrNumToA        ;  Transfer            ||| |
		add sp, 2            ;                      ||| |
				     ;                      ||| |
		mov [di], bl         ;  *DI = *DI + BL;     ||| |
	                             ;                      ||| |
		dec di               ;  DI--;               ||| |
		xor dx, dx           ;  DX = 0;             ||| |
		                     ;                      ||| |
		jmp RecNumStCond1    ;}                     ||| |
	                             ;========================| |
	                             ;                        | |
	                             ;==== If order == 2^k <--+ |
RecNumPow2:	xor cl, cl           ;Set the               ||  |
		mov dh, 0FFh	     ;counter               ||  |
		                     ;                      ||  |
		                     ;                      ||  |
RecNumP2Cond1:	cmp dl, 01h          ;While (DL != 1)       ||  |
		je RecNumP2EndC1     ;{	                    ||  |
		                     ;                      ||  |
		shr dl, 1            ;  DL >> 1;            ||  |
		shl dh, 1	     ; 	DH << 1;            ||  |
		inc cl               ;  CL++;               ||  |
		                     ;                      ||  |
		jmp RecNumP2Cond1    ;}                     ||  |
	                             ;                      ||  |
RecNumP2EndC1:	xor dh, 0FFh         ;DH = DH ^ 255;        ||  |
		mov bl, dh           ;BL = DH;              ||  |
	                             ;                      ||  |
RecNumP2Cond2:	cmp ax, 0000h        ;while (ch != 0)       ||  |
		je RecNumEnd         ;{                     ||  |
		                     ;                      ||  |
		and bx, ax           ;	BX = BX ^ AX;       ||  |
		                     ;                      ||  |
		push bx              ;	                    ||  |
		call TrNumToA        ;  Transfer            ||  |
		add sp, 2            ;                      ||  |
		                     ;                      ||  |
		mov [di], bl         ;  *DI = BL;           ||  |
		mov bl, dh           ;  BL = DH;            ||  |
		shr ax, cl           ;  AX >> CL;           ||  |
		                     ;                      ||  |
		dec di               ;  DI--;               ||  |
			             ;                      ||  |
		jmp RecNumP2Cond2    ;}                     ||  |
		                     ;======================||  |
		                     ;                          |
RecNumWrong:	xor ax, ax           ;<--- If order is wrong <--+
                                     
                                     
RecNumEnd:	inc di
		pop bp
		ret                  ; Exit
endp
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;Name:   TrNumToA
;Action: Transfer the digit to ASCII-code.
;Entry:  The digit
;Exit:   BL -> Ascii-code
;Destr:  BH
;----------------------------------------------------------------
TrNumToA proc, digit
		
		push bp
		mov bp, sp
		
		mov bx, digit

		cmp bl, 00h
		jb  TrNumToAUndS
		cmp bl, 09h
		ja  TrNumToANotFig

		add bl, '0'
		jmp TrNumToAEnd

TrNumToANotFig: cmp bl, 0Ah
		jb  TrNumToAUndS
		cmp bl, 10h
		ja  TrNumToAUndS

		add bl, 37h          ; A-F + 31h = 'A-F'
		jmp TrNumToAEnd

TrNumToAUndS:   mov bl, 00h

TrNumToAEnd:	pop bp
		ret
endp
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;================================================================
 
.data

newstring     db 0Ah
inputbuff     db 10, 10 dup(0)
outputbuff    db 10 dup(0), '$'

end start
