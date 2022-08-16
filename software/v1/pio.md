Code assumes state-machine works at 114kHz. This means that using 3 instructions per carrier-period, we can get to the 38kHz carrier frequency (with either 33% or 66% duty cycle).

This also means that from the main clock of 125MHz, we have to use a clock divider of 1096.

```asm
.wrap_target
; Take data from TX FIFO and move it to OSR.
; Stall if TX FIFO is empty.
PULL                      ;  1 ;

; First part: generate 9ms of pulses
; Pulses have a frequency of 38kHz with duty-cycle of 33%

; In this block, X represents the outer loop's counter.
SET X, 11                 ;  2 ;
init_outer:
  SET PINS, 0             ;  3 ;
  SET PINS, 1             ;  4 ;
  ; In this block, Y represents the inner loop's counter.
  SET Y, 31               ;  5 ;
init_inner:
    SET PINS, 0           ;  6 ;
    SET PINS, 1           ;  7 ;
    JMP Y--, init_inner;  ;  8 ;
  SET PINS, 0             ;  9 ;
  SET PINS, 1             ; 10 ;
  JMP X--, init_outer     ; 11 ;
```

Next, we  attempt to generate 4.5 ms of silence:

$$ 16 \cdot (31+1) \cdot {1 \over 114000} = 0.00449 \approxeq 0.0045 \text{sec} $$

And in code:

```asm
; Second part: generate 4.5ms of silence

; In this block, X represents the loop's counter.
SET X, 16                    ; 12 ;
init_silence:
  JMP X--, init_silence [31] ; 13 ;
```



```asm
; Third part: transmit the payload

; In this block, X represents the bit we're working with.
SET X, 31                    ; 14 ;
next_bit:

  ; Set up a loop to send a burst of pulses.
  ; In this block, Y represents a loop's counter.
  SET Y, 21                  ; 15 ;
  ; Loop takes 21*3*(1/114000) seconds
modulate:
    SET PINS, 1              ; 16 ;
    SET PINS, 0              ; 17 ;
    JMP Y--, modulate        ; 18 ;

  ; In this decision block, Y represents the bit we're sending.
  ; Shift 1 LSB bit from OSR to scratch-register Y.
  OUT Y, 1                   ; 19 ;
  JMP !Y, half_sleep         ; 20 ;

  ; In this block, Y represents a loop's counter.
  ; Loop takes 4*32*(1/114000) seconds
  SET Y, 4                   ; 21 ;
sleeping_1:
    JMP Y--, sleeping [31]   ; 22 ;

  ; In this block, Y represents a loop's counter.
  ; Loop takes 2*32*(1/114000) seconds
half_sleep:
  SET Y, 2:                  ; 23 ;
sleeping_2:
    JMP Y--, sleeping [31]   ; 24 ;

  JMP X--, next_bit          ; 25 ;
```

```asm
; Fourth part: generate another 562us of pulses.

; In this block, X represents the loop's counter.
SET X, 21                   ; 26 ;
end_modulation:
  SET PINS, 1               ; 27 ;
  SET PINS, 0               ; 28 ;
  JMP X--, end_modulation   ; 29 ;
.wrap
```
