Welcome !
=========

What?
-----

TWI stands for Tangible Wireless IMU. It is basically composed with a 9 axis IMU
and an ARM-BLE SOC made by Nordic Semi, the nRF51822.
This repository contains the software to program this CPU, and the PCB can be
found here:

https://upverter.com/honnet/0e8141d8154cb633/twi


How?
----

### Base

Before testing on the custom PCB a cheap dev board can be used:

http://www.aliexpress.com/snapshot/251687009.html

...this cheap programmer is fine too:

http://www.aliexpress.com/snapshot/251611169.html

Once everything is gathered, most of the nRF51822 bringup procedure can be made
thanks to this tuto:

http://www.funwithelectronics.com/?id=168

### Workarounds

A few steps might no work when following this tutorial, here are a few solutions:

1) It might not be useful to download the Mentor toolchain, it is incomplete.
Instead, the one Linaro one can be used (from the official ARM developers):

https://launchpad.net/gcc-arm-embedded

2) If you miss the Nordic files you can use git to travel in the past:

https://github.com/Sproutling/nRF51822-OSX-Sample

It won't be the latest version but you should find what you need.
The nrf51 SDK install doesn't always work with wine. A Windows XP in virtualbox
doesn't alway help but it works with a real windows (vista for example).

3) The linker script used in the tuto's makefile is supposed to be given by
Nordic but it doesn't seem to exist anymore (with this name) so this one can be
used: `gcc_nrf51_blank_xxaa.ld`

4) You have to define your exact CPU such as `NRF51422_QFAA_C0` in your CFLAGS
otherwise it won't compile compile (but you might be using another kind of CPU).

5) In Ubuntu, to be able to use the segger script, you might need to install
`lib32readline5` even if you use a 64 bits OS.


Bonus!
------

* Video of the 1st blinking test:

http://youtu.be/sgKb9N6MPIM

* Test IMU (video to come later):

http://www.aliexpress.com/snapshot/252001193.html


Enjoy!
------

A lot of awesome nRF51 repo are available on github or bitbucket, but feel free
to ask if you need any help:

Drix(a)TangibleDisplay.com

May the force be with you!



License
-------

![Creative Commons NonCommercial License](http://i.creativecommons.org/l/by-nc-sa/3.0/88x31.png)


This project is released under a Creative Commons, Attribution-NonCommercial-ShareAlike, Version 3.0 License:
http://creativecommons.org/licenses/by-nc-sa/3.0
