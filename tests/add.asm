; Test program to verify correct emu8051 operation
;
; Test desc: ADD
; Test output: PC = $FFF0
; Test output: SP = $07
; Test output: A = $20
; Test output: B = $00
; Test output: PSW = $01
; Test output: R0 = $0F
; Test output: R1 = $11
; Test output: R2 = $10
; Test output: R3 = $1F
; Test output: R4 = $20

        ORG     0000h           ; Reset vector

        ;; ADDC

        CLR     C
        MOV     A, #0FFh
        ADDC    A, #10h         ; CY=1, A=$0F
        MOV     R0, A

        MOV     A, #00h
        ADDC    A, #10h         ; CY=0, A=$11
        MOV     R1, A

        SETB    C
        MOV     A, #0FFh
        ADDC    A, #10h         ; CY=1, A=$10
        MOV     R2, A

        ;; ADD

        MOV     A, #0FFh
        ADD     A, #20h         ; CY=1, A=$1F
        MOV     R3, A

        MOV     A, #00h
        ADD     A, #20h         ; CY=0, A=$20
        MOV     R4, A

        LJMP    0FFF0h
        END
