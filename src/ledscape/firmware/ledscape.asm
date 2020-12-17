; LEDScape ASM heavily based on the original LEDscape software
; but updated
; This expects strip-oriented input data, that is to say
; 32 bytes for G (one per strip), then 32 bytes for R (one per strip), etc.
; Should work with either 24-bit or 32-bit color values

; memtest ASM
  .cdecls "main.c"
  .clink
  .global START

  ;shared DDR memory
  .asg C31, DDR

  ;offsets for struct parsing
  .asg 0, G0
  .asg 4, G1
  .asg 8, FRAME_PTR_HOLD
  .asg 12, LEN
  .asg 14, CMD
  .asg 15, RESP
  .asg 16, CHANNELS
  .asg 0x80010000, FRAME ;we start the frame by skipping
                      ;over a suspicious range
                      ; I don't know what's in there,
                      ; but if I touch it it crashes
  
  ;registers for storing important values
  .asg r4.b0, command
  .asg r4.w1, npixels
  .asg r5, pixels
  .asg r20, length

  ; registers for key data
  .asg r7, gpio0_zeros
  .asg r8, gpio1_zeros
  .asg r9, gpio2_zeros
  .asg r10, gpio3_zeros

  .asg r24, channel_loop
  .asg r25, bit_loop
  .asg r30, bytes

  ; aliases registers usable as temp values
  .asg r0, t0
  .asg r1, t1
  .asg r2, t2
  .asg r31, t3
  .asg r4, sleeper
  .asg r6, t6

  .include "pin_map.asm"

  ; Mappings of the GPIO devices
  .asg 0x44E07000, GPIO0
  .asg 0x4804c000, GPIO1
  .asg 0x481AC000, GPIO2
  .asg 0x481AE000, GPIO3

  ; Offsets for the clear and set registers in the devices */
  .asg 0x190, GPIO_CLEARDATAOUT
  .asg 0x194, GPIO_SETDATAOUT

  .asg 0xC, CLOCK

  .asg 650, BASE_WAIT

SET_OUTPUT_BIT .macro bit,channel
  .var BANK
  .asg c:channel:_gpio, BANK
  .var BYTE
  .asg c:channel:_byte, BYTE
  QBBS skip?, BYTE, bit  ;if the bit is set in the origin
                          ; we clear it in the output
  SET gpio:BANK:_zeros, gpio:BANK:_zeros, c:channel:_pin
skip?:
  ;nop
  .endm

; lazy sleep implementation
SLEEPNS .macro ns,inst
  LDI32 t0,  ns/10-1-inst ; 10ns because there are two instructions
reset?:
  SUB t0, t0, 1
  QBNE reset?, t0, 0
  .endm

; pulls the cycle counter into reg
READ_TIME .macro reg
  LBCO  &reg, C28, CLOCK, 4
  .endm

; waits by checking the cycle counter
; very precise
WAITNS .macro ns
waitns?:
  READ_TIME t6 ; read the elapsed cycle counter
  SUB  t6, t6, sleeper ;sleeper should store the base cycle
  QBGT waitns?, t6, (ns)/5
  .endm

; parse details from the command struct into register
READ_CMD .macro
  LBCO  &command, DDR, CMD, 1
  LBCO  &npixels, DDR, LEN, 4
  .endm

; ack the data by flipping the command
; and the response to 0
DATA_RECVD .macro
  LDI   t0.b0, 0
  SBCO    &t0.b0, DDR, CMD, 1
  SBCO    &t0.b0, DDR, RESP, 1
  .endm

; ack complete by setting the response 
; to 1
DATA_DONE .macro val
  LDI   t0.b0, val
  SBCO    &t0.b0, DDR, RESP, 1
  .endm

; dump a literal to one of our debugging registers
DB .macro bank, val
  LDI32   t0, val
  SBCO    &t0, DDR, bank, 4
  .endm

; handy macro for building our GPIO mask
; for the pins we actually control
SET_MASK  .macro channel
  SET c:channel:_gpio_mask, c:channel:_gpio_mask, c:channel:_pin
  .endm

; this waits until debug flag G1 is set to 0
PAUSE .macro
  DB G1, 1
pause?:
  LBCO    &t0, DDR, G1, 4
  QBEQ    pause?, t0, 1
  .endm

; dumps the contents of a register to G0
DUMP_REG .macro reg
  SBCO    &reg, DDR, G0, 4
  .endm

RESET_CLOCK .macro
  ;disable the cycle counter
  LBCO    &t6, C28, 0, 4
  CLR     t6, t6, 3
  SBCO    &t6, C28, 0, 4

  ;then set it to 0
  LDI32   t2, 0
  SBCO    &t2, C28, CLOCK, 4

  ;then turn it back on
  SET     t6, t6, 3
  SBCO    &t6, C28, 0, 4
  .endm

START:
  ; Enable the OCP master port
  LBCO    &r0, C4, 4, 4    ; load SYSCFG reg into r0 (use c4 const addr)
  CLR     r0, r0, 4        ; clear bit 4 (STANDBY_INIT)
  SBCO    &r0, C4, 4, 4    ; store the modified r0 back at the load addr

  ;zero out our message area
  LDI32   r0, 0
  SBCO    &r0, DDR, 0, 4
  SBCO    &r0, DDR, 4, 4
  SBCO    &r0, DDR, 8, 4
  SBCO    &r0, DDR, 12, 4

  ; configure C28 to point to
  ; the control registers
  LDI32   t0, 0x22028
  LDI32   t1, 0x00000220
  SBBO    &t1, t0, 0, 4

  DB G0, 1 ; announce that we've started

  ; configure the GPIO mask for all the pins we control
  SET_MASK 0
  SET_MASK 1
  SET_MASK 2
  SET_MASK 3
  SET_MASK 4
  SET_MASK 5
  SET_MASK 6
  SET_MASK 7
  SET_MASK 8
  SET_MASK 9
  SET_MASK 10
  SET_MASK 11
  SET_MASK 12
  SET_MASK 13
  SET_MASK 14
  SET_MASK 15
  SET_MASK 16
  SET_MASK 17
  SET_MASK 18
  SET_MASK 19
  SET_MASK 20
  SET_MASK 21
  SET_MASK 22
  SET_MASK 23
  SET_MASK 24
  SET_MASK 25
  SET_MASK 26
  SET_MASK 27
  SET_MASK 28
  SET_MASK 29
  SET_MASK 30
  SET_MASK 31

_LOOP:
  DB G0, 2 ; we're in the main loop
  DB G1, 0
  LDI32   r0, 0
  READ_CMD
  QBEQ    _LOOP, command, 0 ; loop if we haven't gotten a command
  QBEQ    _EXIT, command, 0xFF ;exit on 255

  DB G0, 3 ; we've gotten a command

  DATA_RECVD

  LBCO    &t6, DDR, CHANNELS, 1 ; check how many bytes we have

  LDI32   length, 0
  ; length is in pixels, so let's convert it to bytes
  ADD     r2, npixels, npixels ;add once to make npixels * 2
  QBEQ    RGB_ONLY, t6, 3 ;if we're RGB bytes, skip to adding just once more
  ADD     r2, r2, npixels ;otherwise we're RGBW
RGB_ONLY:
  ADD     length, r2, npixels

  LDI32   pixels, FRAME ; frame starts someplace above 

FRAME_LOOP:
  DB G0, 4
  ;load 32 bytes from our input
  LBBO    &scratch, pixels, 0, 32
  LDI32   bit_loop, 8

BIT_LOOP:
  SUB     bit_loop, bit_loop, 1

  RESET_CLOCK

  READ_TIME sleeper ;track the time at which we start signalling
                    ;waits will be relative to this

  ; fill the output buffers
  LDI32   gpio0_zeros, 0
  LDI32   gpio1_zeros, 0
  LDI32   gpio2_zeros, 0
  LDI32   gpio3_zeros, 0

  ;parse the input values into the output data
  SET_OUTPUT_BIT bit_loop, 0
  SET_OUTPUT_BIT bit_loop, 1
  SET_OUTPUT_BIT bit_loop, 2
  SET_OUTPUT_BIT bit_loop, 3
  SET_OUTPUT_BIT bit_loop, 4
  SET_OUTPUT_BIT bit_loop, 5
  SET_OUTPUT_BIT bit_loop, 6
  SET_OUTPUT_BIT bit_loop, 7
  SET_OUTPUT_BIT bit_loop, 8
  SET_OUTPUT_BIT bit_loop, 9
  SET_OUTPUT_BIT bit_loop, 10
  SET_OUTPUT_BIT bit_loop, 11
  SET_OUTPUT_BIT bit_loop, 12
  SET_OUTPUT_BIT bit_loop, 13
  SET_OUTPUT_BIT bit_loop, 14
  SET_OUTPUT_BIT bit_loop, 15
  SET_OUTPUT_BIT bit_loop, 16
  SET_OUTPUT_BIT bit_loop, 17
  SET_OUTPUT_BIT bit_loop, 18
  SET_OUTPUT_BIT bit_loop, 19
  SET_OUTPUT_BIT bit_loop, 20
  SET_OUTPUT_BIT bit_loop, 21
  SET_OUTPUT_BIT bit_loop, 22
  SET_OUTPUT_BIT bit_loop, 23
  SET_OUTPUT_BIT bit_loop, 24
  SET_OUTPUT_BIT bit_loop, 25
  SET_OUTPUT_BIT bit_loop, 26
  SET_OUTPUT_BIT bit_loop, 27
  SET_OUTPUT_BIT bit_loop, 28
  SET_OUTPUT_BIT bit_loop, 29
  SET_OUTPUT_BIT bit_loop, 30
  SET_OUTPUT_BIT bit_loop, 31

  ; mark the SET BIT address
  LDI32 t0, GPIO0 | GPIO_SETDATAOUT
  LDI32 t1, GPIO1 | GPIO_SETDATAOUT
  LDI32 t2, GPIO2 | GPIO_SETDATAOUT
  LDI32 t3, GPIO3 | GPIO_SETDATAOUT

  WAITNS BASE_WAIT ;we start waiting a bit at the top of the loop
              ; this is to ensure we hold low long enough between bits

  ;send HIGH for all the pins we control
  SBBO    &gpio0_mask, t0, 0, 4
  SBBO    &gpio1_mask, t1, 0, 4
  SBBO    &gpio2_mask, t2, 0, 4
  SBBO    &gpio3_mask, t3, 0, 4

  ; mark the clear bit address
  LDI32 t0, GPIO0 | GPIO_CLEARDATAOUT
  LDI32 t1, GPIO1 | GPIO_CLEARDATAOUT
  LDI32 t2, GPIO2 | GPIO_CLEARDATAOUT
  LDI32 t3, GPIO3 | GPIO_CLEARDATAOUT

  WAITNS BASE_WAIT + 200
  ; set low for the pins that are sending 0
  SBBO    &gpio0_zeros, t0, 0, 4
  SBBO    &gpio1_zeros, t1, 0, 4
  SBBO    &gpio2_zeros, t2, 0, 4
  SBBO    &gpio3_zeros, t3, 0, 4

  WAITNS  BASE_WAIT + 600
  READ_TIME sleeper 

  ;set low for all the pins we control
  SBBO    &gpio0_mask, t0, 0, 4
  SBBO    &gpio1_mask, t1, 0, 4
  SBBO    &gpio2_mask, t2, 0, 4
  SBBO    &gpio3_mask, t3, 0, 4

  QBNE BIT_LOOP, bit_loop, 0 ;back to the bit loop

  ;after outputing 32 bytes, we advance the frame pointer
  ADD     pixels, pixels, 32
  SUB     length, length, 1

  QBNE    FRAME_LOOP, length, 0  ;loop until we have blasted all the data

  ;delay for >50usec for the LEDs
  SLEEPNS 50000, 1

  DATA_DONE 1
  QBA     _LOOP

_EXIT:
  DATA_DONE 255
  QBA _LOOP
  ; there is no escape

