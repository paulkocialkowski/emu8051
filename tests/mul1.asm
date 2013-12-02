; Test program to verify correct emu8051 operation
;
; Test desc: MUL AB (no overflow)
; Test output: PC = $FFF0
; Test output: SP = $07
; Test output: A = $C2
; Test output: B = $00
; Test output: PSW = $01

        ORG     0000h           ; Reset vector

        MOV     A, #061h
        MOV     B, #002h
        MUL     AB              ; CY should be cleared, OV should be cleared

        LJMP    0FFF0h
        END
