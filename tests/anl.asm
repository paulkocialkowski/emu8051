; Test program to verify correct emu8051 operation
;
; Test desc: ANL
; Test output1: A = $60
; Test output2: B = $60
; Test output3: PSW = $E0
; Test output4: TCON = $01

        CSEG

        ORG     0000h           ; Reset vector

        MOV     PSW, #0E0h
        CLR   	TCON.0
        CLR     C
        ANL     C, TCON.0       ; CY Should be 0
        MOV     A, PSW

        SETB   	TCON.0
        ANL     C, TCON.0       ; CY Should be 0
        MOV     B, PSW

        SETB    C
        ANL     C, TCON.0       ; CY Should be 1

        LJMP    0FFF0h
        END
