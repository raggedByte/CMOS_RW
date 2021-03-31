.DATA   

.CODE

foo PROC
mov RAX, 0
cycle:

mov qword ptr [RDX + RAX], RAX
inc RAX
cmp RAX, RCX
jnz cycle

ret
foo ENDP
 
fooa PROC
mov RBX, 0
cycle:
	mov al, bl
	out 70h, al
	in al, 71h
	mov qword ptr[RCX + RBX], RAX
	cmp RBX, 12
	jnz cycle
	ret
fooa ENDP

start PROC
        mov        al,0Bh               ; CMOS OBh - ����������� ������� �
        out        70h,al               ; ���� 70h - ������ CMOS
        in         al,71h               ; ���� 71h - ������ CMOS
        and        al,11111011b         ; �������� ��� 2 (����� ����� - BCD)
        out        71h,al               ; � �������� �������
        mov        al,32h               ; CMOS 32h - ��� ������� ����� ����
        call       print_cmos           ; ����� �� �����
        mov        al,9                 ; CMOS 09h - ��� ������� ����� ����
        call       print_cmos
        mov        al,'-'               ; �����
        int        29h                  ; ����� �� �����
        mov        al,8                 ; CMOS 08h - ������� �����
        call       print_cmos
        mov        al,'-'               ; ��� ���� �����
        int        29h
        mov        al,7                 ; CMOS 07h - ����
        call       print_cmos
        mov        al,' '               ; ������
        int        29h
        mov        al,4                 ; CMOS 04h - ���
        call       print_cmos
        mov        al,'h'               ; ����� "h"
        int        29h
        mov        al,' '               ; ������
        int        29h
        mov        al,2                 ; CMOS 02h - ������
        call       print_cmos
        mov        al,':'               ; ���������
        int        29h
        mov        al,0h                ; CMOS 00h - �������
        call       print_cmos
        ret
start ENDP


; ��������� print_cmos
; ������� �� ����� ���������� ������ CMOS � ������� � AL
; �������, ��� �����, �������� �� CMOS, ��������� � ������� BCD
print_cmos PROC
        out        70h,al               ; ������� AL � ��������� ���� CMOS
        in         al,71h               ; ��������� ������
        push       ax
        shr        al,4                 ; �������� ������� ������ ����
        add        al,'0'               ; �������� ASCII-��� ����� 0
        int        29h                  ; ������� �� �����
        pop        ax
        and        al,0Fh               ; �������� ������� ������ ����
        add        al,30h               ; �������� ASCII-��� ����� 0
        int        29h                  ; ������� �� �����
        ret
print_cmos endp

END


;int:
;1 - RCX
; 2 - RDX
;  3 - R8
;   4 - R9
;    ... - ����
;ret = RAX


;float:
;1 - XMM0
; 2 - XMM1
;  3 - XMM2
;   4 - XMM3
;    ... - ����
;ret = XMM0
