; Test program to verify correct emu8051 operation
;
; Test desc: ORL
; Test output0: PC = $FFF0
; Test output1: A = $60
; Test output2: PSW = $E0
; Test output3: TCON = $01

        CSEG

        ORG     0000h           ; Reset vector

        MOV     PSW, #0E0h
        CLR   	TCON.0
        CLR     C
        ORL     C, TCON.0       ; CY Should be 0
        MOV     A, PSW

        SETB   	TCON.0
        ORL     C, TCON.0       ; CY Should be 1

        LJMP    0FFF0h
        END
