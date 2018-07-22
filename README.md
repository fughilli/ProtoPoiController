# ProtoPoi Firmware

This is an implementation of an RGB lights controller for my ProtoPoi
open-source poi. The hardware designs can be found on [Thingiverse](https://www.thingiverse.com/thing:3014531).

## Dependencies
You must have ```ninja-build``` and ```ninja_syntax``` installed. Install with:
```
$ sudo apt-get install ninja-build
...
$ sudo -H pip install ninja_syntax
...
```

## Building
To build, first run ```./configure.py``` and then ```ninja```.

## Programming the Target
Connect TEST, RST, and GND between the MSP430G2553 on the ProtoPoi board and an
MSP-EXP430G2 development board (with no device installed in the development
board). If you have a LiPo installed (and it is charged), turn on the power
switch. If you do not have a battery installed, also connect VCC between the two
boards. Then, run:
```
$ ./program -p
```

If you built the hardware correctly, you should see the LEDs flash rapidly
between red and green.
