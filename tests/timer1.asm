; Test program to verify correct emu8051 operation
;
; Test desc: timers
; Test output0: PC = $FFF0
; Test output1: R0 = $22
; Test output2: R1 = $97
; Test output3: R2 = $20
; Test output4: R3 = $42
; Test output5: TIMER0 = $10DA
; Test output6: TIMER1 = $1F42
; Test output7: TCON = $A0
; Test output8: TMOD = $11
; Test output9: PSW = $00

TOS     EQU     60h     ; Adresse du dessus de la pile.

        CSEG

        ORG     0000h           ; Reset vector
        MOV     SP,#TOS         ; Init stack pointer

        ;;  First test: no overflow
	MOV	TH0,#2
	MOV	TL0,#55h        ; Set initial value of timer0 to $0255
	MOV     TMOD,#00010001B ; Init timers 0 and 1 as 16-bit timers,
                                ; incremented by the internal clock
        CALL    DELAY

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
	MOV     TMOD,#00010001B ; Init timers 0 and 1 as 16-bit timers,
                                ; incremented by the internal clock
                                ; Both timers should overflow
        CALL    DELAY

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
