; Test program to verify correct emu8051 operation
; Taken from http://www.vzsite.us/8051/
;
; Test desc: 32-bit division
; Test output: PC = $FFF0
; Test output: SP = $30
; Test output: PSW = $04
; Test output: A = $44
; Test output: B = $44

;17 Jan 00 re-written for consistency with assembler/compiler byte order
;21 May 99 8051 source code
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;SQROOT1 is called to calculate the square root of a 32-bit number.
;
;Call:
;
;     r0 => MSB of 32-bit input
;     lcall SQROOT1
;
;Return:
;
; computed square root is in acc(LSB's) and b(MSB's)
; (root is also in est(MSB's) and est+1(LSB's))
;
;SQROOT1 uses the formula
;
; estimate = (last_estimate + input/last_estimate)/2
;
;Method is described as Newton's, Newton-Raphson, and Babylonian method.
;DIV32U is called to do the 32/16-bit division.
;SQROOT1 perfoms a fixed number of iterations to converge to the root.
;
;SQROOT1 will destroy all registers except r0, dptr.
;
;Original Author: John Veazey, Ridgecrest, CA, 18 Apr 99
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

q0      EQU     70h
q1      EQU     71h
est     EQU     72h     ; 2 bytes: The estimated value being developed
sqrcnt  EQU     74h     ; Counts iterations
square  EQU     75h
TOS     EQU     30h     ; Adresse du dessus de la pile.

        ORG     0000h           ; Reset vector
        MOV     SP,#TOS         ; Init stack pointer

        ;; Set 32-bit input value
        MOV     square+0,#012h
        MOV     square+1,#034h
        MOV     square+2,#032h
        MOV     square+3,#010h

        ;; Set pointer to square
        mov   r0,#square

        LCALL   SQROOT
        LJMP    0FFF0h

SQROOT:
;
;Save r0 on the stack
;
      mov   a,r0
      push  acc
;
;Initialize the iteration counter
;
      mov   sqrcnt,#7   ;There will be seven calculations
;
;Find the MSB set in input, call it n as in 2**n. Calculate 
; r = ceiling(n+1)/2, then set up the first estimate to
; be (2**r)-1.
;
      mov   a,@r0       ;Is MS byte of input not-zero?
      jz    sqr112
      mov   r1,#33      ; Yes, set r1 to the bit number+2
      sjmp  sqr140
sqr112:
      inc   r0          ; No,
      mov   a,@r0       ;Is 2nd MS byte of input not-zero?
      jz    sqr114
      mov   r1,#25      ;  Yes
      sjmp  sqr140
sqr114:
      inc   r0
      mov   a,@r0       ;Is 3rd MS byte of input not-zero?
      jz    sqr116
      mov   r1,#17      ;  Yes
      sjmp  sqr140
sqr116:
      inc   r0
      mov   est,#0
      mov   a,@r0       ;Is LS byte of input zero?
      jnz   sqr122
      mov   est+1,#0      ; Yes, return with a zero
      ljmp  sqr900      ;  because input is zero
sqr122:
      mov   est+1,#1
      dec   a           ;Is LS byte of input = 1?
      jz    sqr900      ; Yes, return with a one
      inc   a           ; No
      mov   r1,#9
sqr140:
      dec   r1
      rlc   a
      jnc   sqr140
      mov   a,r1        ;Form ceiling[(n+1)/2]
      clr   c
      rrc   a           ; (divide by 2, add remainder)
      jnc   sqr142
      inc   a
sqr142:
      mov   r1,a
      clr   a
      mov   est,a
sqr144:
      setb  c           ;Get the 1 to shift in
      rlc   a
      mov   b,a
      mov   a,est
      rlc   a
      mov   est,a
      mov   a,b
      dec   r1
      cjne  r1,#0,sqr144
      mov   est+1,a
;
;Load input into DIV32U dividend register
;
sqr200:
      pop   acc
      push  acc
      mov   r0,a
      mov   a,@r0		;Set MSB's
      mov   r7,a
      inc   r0
      mov   a,@r0
      mov   r6,a
      inc   r0
      mov   a,@r0
      mov   r5,a
      inc   r0
      mov   a,@r0
      mov   r4,a
;
;Load last estimate into DIV32U divisor registers
;
      mov   a,est+1	;Set LSB's
      mov   r0,a
      mov   a,est		;Set MSB's
      mov   r1,a
;
;Call DIV32U to do the 32/16-bit division (input/est)
;
      lcall DIV32U      ;(r7,r6,r5,r4)/(r1,r0) = (r5,r4)
      jnc   sqr310      ;If overflow, set to maximum number
      mov   r4,#-1
      mov   r5,#-1
;
;Add the last estimate to the quotient
;
sqr310:
      mov   a,r4
      add   a,est+1     ;sets c
      mov   est+1,a
      mov   a,est
      addc  a,r5        ;sets c
;
;Divide sum by 2 and save as new estimate
;
      rrc   a           ;sets c
      mov   est,a
      mov   a,est+1
      rrc   a           ;c discarded
      mov   est+1,a
;
;Decrement the iteration counter and repeat if not zero
;
      djnz  sqrcnt,sqr200
;
;Make the normal return
;
sqr900:
      pop   acc         ;Restore caller's pointer
      mov   r0,a
      mov   b,est       ;Get answer in (b,a)
      mov   a,est+1
      ret


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

