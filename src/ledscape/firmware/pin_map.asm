    ; we use 32 register bytes as scratch
  ; so r11-r18
  .asg r11, scratch

  .asg r26, gpio0_mask
  .asg r27, gpio1_mask
  .asg r28, gpio2_mask
  .asg r29, gpio3_mask
  ; 0
  .asg 2, c0_gpio ; the gpio bank we output to
  .asg gpio2_mask, c0_gpio_mask ;the gpio mask we intoract with
  .asg 3, c0_pin ; our output bit within that bank
  .asg r11.b0, c0_byte ;which register byte will hold the data for this pin
  SET c0_gpio_mask, c0_gpio_mask, c0_pin ;set our mask pin
  ; 1
  .asg 2, c1_gpio
  .asg gpio2_mask, c1_gpio_mask
  .asg 2, c1_pin
  .asg r11.b1, c1_byte

  SET c1_gpio_mask, c1_gpio_mask, c1_pin
  ; 2
  .asg 2, c2_gpio
  .asg gpio2_mask, c2_gpio_mask
  .asg 5, c2_pin
  .asg r11.b2, c2_byte
  SET c2_gpio_mask, c2_gpio_mask, c2_pin
  ; 3
  .asg 2, c3_gpio
  .asg gpio2_mask, c3_gpio_mask
  .asg 4, c3_pin
  .asg r11.b3, c3_byte
  SET c3_gpio_mask, c3_gpio_mask, c3_pin
  ; 4
  .asg 1, c4_gpio
  .asg gpio1_mask, c4_gpio_mask
  .asg 12, c4_pin
  .asg r12.b0, c4_byte
  SET c4_gpio_mask, c4_gpio_mask, c4_pin
  ; 5
  .asg 1, c5_gpio
  .asg gpio1_mask, c5_gpio_mask
  .asg 13, c5_pin
  .asg r12.b1, c5_byte
  SET c5_gpio_mask, c5_gpio_mask, c5_pin
  ; 6
  .asg 0, c6_gpio
  .asg gpio0_mask, c6_gpio_mask
  .asg 23, c6_pin
  .asg r12.b2, c6_byte
  SET c6_gpio_mask, c6_gpio_mask, c6_pin
  ; 7
  .asg 0, c7_gpio
  .asg gpio0_mask, c7_gpio_mask
  .asg 26, c7_pin
  .asg r12.b3, c7_byte
  SET c7_gpio_mask, c7_gpio_mask, c7_pin
  ; 8
  .asg 1, c8_gpio
  .asg gpio1_mask, c8_gpio_mask
  .asg 14, c8_pin
  .asg r13.b0, c8_byte
  SET c8_gpio_mask, c8_gpio_mask, c8_pin
  ; 9
  .asg 1, c9_gpio
  .asg gpio1_mask, c9_gpio_mask
  .asg 15, c9_pin
  .asg r13.b1, c9_byte
  SET c9_gpio_mask, c9_gpio_mask, c9_pin
  ; 10
  .asg 0, c10_gpio
  .asg gpio0_mask, c10_gpio_mask
  .asg 27, c10_pin
  .asg r13.b2, c10_byte
  SET c10_gpio_mask, c10_gpio_mask, c10_pin
  ; 11
  .asg 2, c11_gpio
  .asg gpio2_mask, c11_gpio_mask
  .asg 1, c11_pin
  .asg r13.b3, c11_byte
  SET c11_gpio_mask, c11_gpio_mask, c11_pin
  ; 12
  .asg 1, c12_gpio
  .asg gpio1_mask, c12_gpio_mask
  .asg 29, c12_pin
  .asg r14.b0, c12_byte
  SET c12_gpio_mask, c12_gpio_mask, c12_pin
  ; 13
  .asg 0, c13_gpio
  .asg gpio0_mask, c13_gpio_mask
  .asg 30, c13_pin
  .asg r14.b1, c13_byte
  SET c13_gpio_mask, c13_gpio_mask, c13_pin
  ; 14
  .asg 1, c14_gpio
  .asg gpio1_mask, c14_gpio_mask
  .asg 28, c14_pin
  .asg r14.b2, c14_byte
  SET c14_gpio_mask, c14_gpio_mask, c14_pin
  ; 15
  .asg 0, c15_gpio
  .asg gpio0_mask, c15_gpio_mask
  .asg 31, c15_pin
  .asg r14.b3, c15_byte
  SET c15_gpio_mask, c15_gpio_mask, c15_pin
  ; 16
  .asg 1, c16_gpio
  .asg gpio1_mask, c16_gpio_mask
  .asg 18, c16_pin
  .asg r15.b0, c16_byte
  SET c16_gpio_mask, c16_gpio_mask, c16_pin
  ; 17
  .asg 2, c17_gpio
  .asg gpio2_mask, c17_gpio_mask
  .asg 25, c17_pin
  .asg r15.b1, c17_byte
  SET c17_gpio_mask, c17_gpio_mask, c17_pin
  ; 18
  .asg 1, c18_gpio
  .asg gpio1_mask, c18_gpio_mask
  .asg 19, c18_pin
  .asg r15.b2, c18_byte
  SET c18_gpio_mask, c18_gpio_mask, c18_pin
  ; 19
  .asg 0, c19_gpio
  .asg gpio0_mask, c19_gpio_mask
  .asg 5, c19_pin
  .asg r15.b3, c19_byte
  SET c19_gpio_mask, c19_gpio_mask, c19_pin
  ; 20
  .asg 0, c20_gpio
  .asg gpio0_mask, c20_gpio_mask
  .asg 4, c20_pin
  .asg r16.b0, c20_byte
  SET c20_gpio_mask, c20_gpio_mask, c20_pin
  ; 21
  .asg 2, c21_gpio
  .asg gpio2_mask, c21_gpio_mask
  .asg 24, c21_pin
  .asg r16.b1, c21_byte
  SET c21_gpio_mask, c21_gpio_mask, c21_pin
  ; 22
  .asg 2, c22_gpio
  .asg gpio2_mask, c22_gpio_mask
  .asg 22, c22_pin
  .asg r16.b2, c22_byte
  SET c22_gpio_mask, c22_gpio_mask, c22_pin
  ; 23
  .asg 0, c23_gpio
  .asg gpio0_mask, c23_gpio_mask
  .asg 3, c23_pin
  .asg r16.b3, c23_byte
  SET c23_gpio_mask, c23_gpio_mask, c23_pin
  ; 24
  .asg 0, c24_gpio
  .asg gpio0_mask, c24_gpio_mask
  .asg 2, c24_pin
  .asg r17.b0, c24_byte
  SET c24_gpio_mask, c24_gpio_mask, c24_pin
  ; 25
  .asg 1, c25_gpio
  .asg gpio1_mask, c25_gpio_mask
  .asg 17, c25_pin
  .asg r17.b1, c25_byte
  SET c25_gpio_mask, c25_gpio_mask, c25_pin
  ; 26
  .asg 0, c26_gpio
  .asg gpio0_mask, c26_gpio_mask
  .asg 15, c26_pin
  .asg r17.b2, c26_byte
  SET c26_gpio_mask, c26_gpio_mask, c26_pin
  ; 27
  .asg 3, c27_gpio
  .asg gpio3_mask, c27_gpio_mask
  .asg 21, c27_pin
  .asg r17.b3, c27_byte
  SET c27_gpio_mask, c27_gpio_mask, c27_pin
  ; 28
  .asg 0, c28_gpio
  .asg gpio0_mask, c28_gpio_mask
  .asg 14, c28_pin
  .asg r18.b0, c28_byte
  SET c28_gpio_mask, c28_gpio_mask, c28_pin
  ; 29
  .asg 3, c29_gpio
  .asg gpio3_mask, c29_gpio_mask
  .asg 19, c29_pin
  .asg r18.b1, c29_byte
  SET c29_gpio_mask, c29_gpio_mask, c29_pin
  ; 30
  .asg 3, c30_gpio
  .asg gpio3_mask, c30_gpio_mask
  .asg 16, c30_pin
  .asg r18.b2, c30_byte
  SET c30_gpio_mask, c30_gpio_mask, c30_pin
  ; 31
  .asg 0, c31_gpio
  .asg gpio0_mask, c31_gpio_mask
  .asg 20, c31_pin
  .asg r18.b3, c31_byte
  SET c31_gpio_mask, c31_gpio_mask, c31_pin
