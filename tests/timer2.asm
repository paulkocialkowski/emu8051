; Test program to verify correct emu8051 operation
;
; Test desc: timers
; Test output: PC = $FFF0
; Test output: SP = $60
; Test output: R0 = $80
; Test output: R1 = $16
; Test output: R2 = $F4
; Test output: R3 = $3E
; Test output: TIMER0 = $8086
; Test output: TIMER1 = $F4F9
; Test output: TCON = $A0
; Test output: TMOD = $22
; Test output: PSW = $00

TOS     EQU     60h     ; Adresse du dessus de la pile.

        ORG     0000h           ; Reset vector
        MOV     SP,#TOS         ; Init stack pointer

        MOV     TMOD,#00100010B ; Init timers 0 and 1 as 8-bit timers (TL),
                                ; reloaded with TH when overflow occurs.

        ;;  First test: no overflow
	MOV	TH0,#080h       ; Reload value
	MOV	TH1,#0F4h       ; Reload value
	MOV	TL0,#00h        ; Set initial value of timer0
       	MOV	TL1,#28h        ; Set initial value of timer1
        LCALL   DELAY

        MOV     00h, TH0         ; Save value of timer 0
        MOV     01h, TL0
        MOV     02h, TH1         ; Save value of timer 1
        MOV     03h, TL1
        MOV     04h, TCON

        ;;  Second test: overflow
	MOV	TL0,#0F0h        ; Set initial value of timer0
	MOV	TL1,#0EFh        ; Set initial value of timer1
        LCALL   DELAY

        LJMP    0FFF0h

        ;; Loop of ~25 cycles
DELAY:
	MOV	B,#10
       	ORL     TCON,#50h	; Timer 0 and timer 1 ON.
B1:
	DJNZ	B,B1
        ANL     TCON,#0AFh      ; Stop both timers
        RET

        END
