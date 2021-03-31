.DATA   
.CODE

; Params:
;	(1) CL - address to editable byte
;	(2) DL - new value
; Return value:
;	none
writecmos PROC
	mov AL, CL
	out 70h, AL
	mov AL, DL
	out 71h, AL
	ret
writecmos ENDP

; Params:
;	(1) CL - address to reading byte
; Return value:
;	AL - readed byte
readcmos PROC
	mov RAX, 0
	mov AL, CL
	out 70h, AL
	in AL, 71h
	ret
readcmos ENDP

END