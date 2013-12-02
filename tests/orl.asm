; Test program to verify correct emu8051 operation
;
; Test desc: ORL
; Test output: PC = $FFF0
; Test output: SP = $07
; Test output: A = $60
; Test output: PSW = $E0
; Test output: TCON = $01

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
