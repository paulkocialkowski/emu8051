; Test program to verify correct emu8051 operation
;
; Test desc: MUL AB (overflow)
; Test output: PC = $FFF0
; Test output: SP = $07
; Test output: A = $5B
; Test output: B = $0B
; Test output: PSW = $05

        ORG     0000h           ; Reset vector

        MOV     A, #099h
        MOV     B, #013h
        MUL     AB              ; CY should be cleared, OV should be cleared

        LJMP    0FFF0h
        END
