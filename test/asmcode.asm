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
        mov        al,0Bh               ; CMOS OBh - управл€ющий регистр ¬
        out        70h,al               ; порт 70h - индекс CMOS
        in         al,71h               ; порт 71h - данные CMOS
        and        al,11111011b         ; обнулить бит 2 (форма чисел - BCD)
        out        71h,al               ; и записать обратно
        mov        al,32h               ; CMOS 32h - две старшие цифры года
        call       print_cmos           ; вывод на экран
        mov        al,9                 ; CMOS 09h - две младшие цифры года
        call       print_cmos
        mov        al,'-'               ; минус
        int        29h                  ; вывод на экран
        mov        al,8                 ; CMOS 08h - текущий мес€ц
        call       print_cmos
        mov        al,'-'               ; еще один минус
        int        29h
        mov        al,7                 ; CMOS 07h - день
        call       print_cmos
        mov        al,' '               ; пробел
        int        29h
        mov        al,4                 ; CMOS 04h - час
        call       print_cmos
        mov        al,'h'               ; буква "h"
        int        29h
        mov        al,' '               ; пробел
        int        29h
        mov        al,2                 ; CMOS 02h - минута
        call       print_cmos
        mov        al,':'               ; двоеточие
        int        29h
        mov        al,0h                ; CMOS 00h - секунда
        call       print_cmos
        ret
start ENDP


; процедура print_cmos
; выводит на экран содержимое €чейки CMOS с номером в AL
; считает, что число, читаемое из CMOS, находитс€ в формате BCD
print_cmos PROC
        out        70h,al               ; послать AL в индексный порт CMOS
        in         al,71h               ; прочитать данные
        push       ax
        shr        al,4                 ; выделить старшие четыре бита
        add        al,'0'               ; добавить ASCII-код цифры 0
        int        29h                  ; вывести на экран
        pop        ax
        and        al,0Fh               ; выделить младшие четыре бита
        add        al,30h               ; добавить ASCII-код цифры 0
        int        29h                  ; вывести на экран
        ret
print_cmos endp

END


;int:
;1 - RCX
; 2 - RDX
;  3 - R8
;   4 - R9
;    ... - стек
;ret = RAX


;float:
;1 - XMM0
; 2 - XMM1
;  3 - XMM2
;   4 - XMM3
;    ... - стек
;ret = XMM0
