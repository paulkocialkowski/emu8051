; Test program to verify correct emu8051 operation
;
; Test desc: MOV
; Test output0: PC = $FFF0
; Test output1: A = $66
; Test output2: B = $98
; Test output3: PSW = $00
; Test output4: DPTR = $9876
; Test output5: R0 = $66

        CSEG

        ORG     0000h           ; Reset vector

        MOV     A, #099h
        MOV     B, A
        DEC     B
        MOV     R0, #066h
        MOV     A, R0
        MOV     DPTR,#9876h

        LJMP    0FFF0h
        END
