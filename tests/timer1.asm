; Test program to verify correct emu8051 operation
;
; Test desc: timers
; Test output: PC = $FFF0
; Test output: SP = $60
; Test output: R0 = $22
; Test output: R1 = $97
; Test output: R2 = $20
; Test output: R3 = $42
; Test output: TIMER0 = $10DA
; Test output: TIMER1 = $1F42
; Test output: TCON = $A0
; Test output: TMOD = $11
; Test output: PSW = $00

TOS     EQU     60h     ; Adresse du dessus de la pile.

        ORG     0000h           ; Reset vector
        MOV     SP,#TOS         ; Init stack pointer

        MOV     TMOD,#00010001B ; Init timers 0 and 1 as 16-bit timers,
                                ; incremented by the internal clock

        ;;  First test: no overflow
	MOV	TH0,#2
	MOV	TL0,#55h        ; Set initial value of timer0 to $0255
        LCALL   DELAY

        MOV     00h, TH0         ; Save value of timer 0
        MOV     01h, TL0
        MOV     02h, TH1         ; Save value of timer 1
        MOV     03h, TL1
        MOV     04h, TCON

        ;;  Second test: overflow
	MOV	TH0,#0F0h
	MOV	TL0,#98h        ; Set initial value of timer0 to $F098
	MOV	Th1,#0FFh
	MOV	TL1,#00h        ; Set initial value of timer1 to $FF00
        LCALL   DELAY

        LJMP    0FFF0h

        ;; Loop of $2040 cycles
DELAY:
	MOV	A,#10h
       	ORL     TCON,#50h	; Timer 0 and timer 1 ON.
B2:
	MOV	B,#0
B1:
	DJNZ	B,B1
	DJNZ	ACC,B2
        ANL     TCON,#0AFh      ; Stop both timers
        RET

        END
