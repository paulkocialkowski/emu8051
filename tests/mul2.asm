; Test program to verify correct emu8051 operation
;
; Test desc: MUL AB (overflow)
; Test output0: PC = $FFF0
; Test output1: A = $5B
; Test output2: B = $0B
; Test output3: PSW = $05

        CSEG

        ORG     0000h           ; Reset vector

        MOV     A, #099h
        MOV     B, #013h
        MUL     AB              ; CY should be cleared, OV should be cleared

        LJMP    0FFF0h
        END
