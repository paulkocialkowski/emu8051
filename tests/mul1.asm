; Test program to verify correct emu8051 operation
;
; Test desc: MUL AB (no overflow)
; Test output1: A = $C2
; Test output2: B = $00
; Test output3: PSW = $01

        CSEG

        ORG     0000h           ; Reset vector

        MOV     A, #061h
        MOV     B, #002h
        MUL     AB              ; CY should be cleared, OV should be cleared

        LJMP    0FFF0h
        END
