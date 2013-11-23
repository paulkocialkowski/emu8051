; Test program to verify correct emu8051 operation
;
; Test desc: MUL AB (overflow)
; Test output1: ACC = 0x5B
; Test output2: B = 0x0B
; Test output3: PSW = 0x05

        CSEG

        ORG     0000h           ; Reset vector

        MOV     A, #099h
        MOV     B, #013h
        MUL     AB              ; CY should be cleared, OV should be cleared

        END
