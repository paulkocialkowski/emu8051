; Test program to verify correct emu8051 operation
; Taken from http://www.vzsite.us/8051/
;
; Test desc: 32-bit division
; Test output: PC = $FFF0
; Test output: SP = $60
; Test output: R0 = $E6
; Test output: R1 = $55
; Test output: R2 = $00
; Test output: R3 = $00
; Test output: R4 = $20
; Test output: R5 = $1B
; Test output: PSW = $05

;26 Oct 00 added code to zero remainder when dividend is zero
;19 Dec 99 corrected comments, removed unnecessary instruction
;16 May 99 8051 source code
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;DIV32U is called to divide (unsigned) a 32-bit dividend using a
; 16-bit divisor.
;
;DIV32U solves for quotient and remainder the equation:
;
; dividend = divisor*quotient + remainder
;
;Call:
;  r7,r6,r5,r4 = dividend
;  r1,r0 = divisor
;  lcall DIV32U
;
;Return:
; r5,r4 = quotient
; r7,r6 = remainder
; c flag set to 1 if overflow occured
; All registers, acc, b and two caller-assigned direct memory bytes
;   (q0 and q1)have been changed.
; Data pointer has not been disturbed
;
;Note:
; (1)Overflow is a divide by zero or any value that will cause
;    the quotient to be greater than 16 bits.
; (2)Most significant (ms) register always listed first when comma separates
;    two in a comment. Example: r3,r2 (r3 contains the ms bits)
; (3) The algorithm used in this code borrows heavily from work posted
;   by John C. Wren who said he got it from a C complier.
;
;Original author: John Veazey, Ridgecrest, CA, 18 APR 99
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
q0      EQU     70h
q1      EQU     71h
TOS     EQU     60h     ; Adresse du dessus de la pile.

        ORG     0000h           ; Reset vector
        MOV     SP,#TOS         ; Init stack pointer

        ;; Set dividend
        MOV     R7,#012h
        MOV     R6,#034h
        MOV     R5,#056h
        MOV     R4,#078h

        ;; Set divisor
        MOV     R1,#0ABh
        MOV     R0,#0CDh

        LCALL   DIV32U
        LJMP    0FFF0h

DIV32U:
;
;Clear the working quotient
;
      clr   a
      mov   q1,a
      mov   q0,a
;
;Clear the msb's of a 32-bit working divisor (r3,r2,r1,r0)
;
      mov   r3,a
      mov   r2,a
;
;b counts the number of places+1 the divisor was initially
; shifted left to align its ms bit set with the ms bit set
; in the dividend
;
      mov   b,#1
;
;Make an error return if trying to divide by zero
;
      mov   a,r1
      orl   a,r0
      jnz   du100
      ljmp  du920       ;Make the error return
;
;Just return with quotient and remainder zero if dividend is zero
;
du100:
      mov   a,r7
      orl   a,r6
      orl   a,r5
      orl   a,r4
      jnz   du200
      mov   r7,a
      mov   r6,a
      ljmp  du910       ;Make a normal return
;
;Align the msb set in the demoninator with the msb set in the
; numerator. Increment the shift count in b each time a shift left
; is performed.
;
du200:
      mov   a,r3        ;Stop if msb set
      clr   c
      rlc   a
      jc    du600
      subb  a,r7        ;Compare r3 & r7, (c clear)
      jz    du210       ; jump if r3=r7
      jnc   du600       ; jump if r3>r7
      sjmp  du240       ; r3<r7
du210:
      mov   a,r6        ;r3=r7, so compare r2 & r6
      subb  a,r2
      jc    du600       ; jump if r2>r6
      jnz   du240       ; jump if r2<r6
      mov   a,r5        ;r2=r6, so compare r1 & r5
      subb  a,r1
      jc    du600       ; jump if r1>r5
      jnz   du240       ; jump if r1<r5
      mov   a,r4        ;r1=r5, so compare r0 & r4
      subb  a,r0
      jc    du600       ; jump if r0>r4
du240:
      clr   c           ;Now shift the denominator
      mov   a,r0        ; left 1 bit position
      rlc   a
      mov   r0,a
      mov   a,r1
      rlc   a
      mov   r1,a
      mov   a,r2
      rlc   a
      mov   r2,a
      mov   a,r3
      rlc   a
      mov   r3,a
      inc   b           ;Increment b counter and
      sjmp  du200       ; continue
;
;Compare the shifted divisor with the remainder (what's
; left of the dividend)
;
du600:
      mov   a,r7
      clr   c
      subb  a,r3
      jc    du720       ;jump if r3>r7
      jnz   du700       ;jump if r3<r7
      mov   a,r6
      subb  a,r2
      jc    du720       ;jump if r2>r6
      jnz   du700       ;jump if r2<r6
      mov   a,r5
      subb  a,r1
      jc    du720       ;jump if r1>r5
      jnz   du700       ;jump if r1<r5
      mov   a,r4
      subb  a,r0
      jc    du720       ;jump if r0>r4
;
;Divisor is equal or smaller, so subtract it off and
; get a 1 for the quotient
;
du700:
      mov   a,r4
      clr   c
      subb  a,r0
      mov   r4,a
      mov   a,r5
      subb  a,r1
      mov   r5,a
      mov   a,r6
      subb  a,r2
      mov   r6,a
      mov   a,r7
      subb  a,r3
      mov   r7,a
      clr   c
      cpl   c           ;Get a 1 for the quotient
      sjmp  du730
;
;Divisor is greater, get a 0 for the quotient
;
du720:
      clr   c
;
;Shift 0 or 1 into quotient
;
du730:
      mov   a,q0
      rlc   a
      mov   q0,a
      mov   a,q1
      rlc   a
      mov   q1,a
      jc    du920       ;overflow - make the error return
;
;Now shift the denominator right 1, decrement the counter
; in b until b = 0
;
du740:
      clr   c
      mov   a,r3
      rrc   a
      mov   r3,a
      mov   a,r2
      rrc   a
      mov   r2,a
      mov   a,r1
      rrc   a
      mov   r1,a
      mov   a,r0
      rrc   a
      mov   r0,a
      djnz  b,du600
;
;Move quotient and remainder
;
      mov   a,r5
      mov   r7,a
      mov   a,r4
      mov   r6,a
      mov   a,q1
      mov   r5,a
      mov   a,q0
      mov   r4,a
;
;Make the normal return
;
du910:
      clr   c
      ret
;
;Make the error return
;
du920:
      clr   c
      cpl   c
      ret
