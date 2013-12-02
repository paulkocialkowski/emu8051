; Test program to verify correct emu8051 operation
;
; Test desc: MOV
; Test output: PC = $FFF0
; Test output: SP = $07
; Test output: A = $66
; Test output: B = $98
; Test output: PSW = $00
; Test output: DPTR = $9876
; Test output: R0 = $66

        ORG     0000h           ; Reset vector

        MOV     A, #099h
        MOV     B, A
        DEC     B
        MOV     R0, #066h
        MOV     A, R0
        MOV     DPTR,#9876h

        LJMP    0FFF0h
        END
