; Test program to verify correct emu8051 operation
;
; Test desc: DIV AB
; Test output: PC = $FFF0
; Test output: SP = $07
; Test output: A = $3C
; Test output: B = $00
; Test output: PSW = $00

        ORG     0000h           ; Reset vector

        MOV     A, #0FFh
        MOV     B, #00h
        DIV     AB              ; CY should be cleared, OV should be set
                                ; PSW = $04

        MOV     A, #240         ; 240d
        MOV     B, PSW          ; 4d
        DIV     AB              ; CY should be cleared, OV should be cleared, result = 10

        LJMP    0FFF0h
        END
