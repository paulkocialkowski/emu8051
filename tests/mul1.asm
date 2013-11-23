; Test program to verify correct emu8051 operation
;
; Test desc: MUL AB (no overflow)
; Test output1: ACC = 0xC2
; Test output2: B = 0x00
; Test output2: PSW = 0x01

        CSEG

        ORG     0000h           ; Reset vector

        MOV     A, #061h
        MOV     B, #002h
        MUL     AB              ; CY should be cleared, OV should be cleared

        END
