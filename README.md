<b>New 2024 march 2</b> : Good news : all screens work, including the 7-pins ones ! 
Read the details in ST7789_280x240/TFT_adapt/README.md

This a a fork of BenHeck's GameBadge3 repo

I have a branch here "ST7789_280x240" where I made some changes
to support wider screens

There is also a TZT_adapt directory with gerber files for an adapter for cheap AliExpress screens (ST7789V screens with a connector layout like (Gnd/Vcc/Scl/Sda/Res/DC/CS/Blk)

To use it, modify code/Arduino libraries/pico_ST7789/pico_ST7789.cpp to set the width of your physical screen, i.e. 320, 280 or 240.
You should set also the appropriate rotation, leaving it to "1" if possible by rotating physically the screen (soldering it
in the left or right slot of the TZT adpter) 

If you do that, NOT-recompiled games may work on your screen without modifications,

exemple :
  #define SCR_WIDTH 320
  #define LCD_ROTATION 1

and then you should recompile.

Centering vertically has not been done yet, and will probably never be.

Don't forget to do all this on the correct branch :

<code>
git clone https://github.com/theflynn49/gamebadge3.git
git checkout ST7789_280x240
  
</code>


Warning : Games for GB3 must be compiled with 
  - flash size=1MB / Sketch Size=1MB
  - CPU Speed = 125Mhz (important !)
  - USB Stack : Adafruit TinyUSB

![DSC02983_s1](https://github.com/theflynn49/gamebadge3/assets/9721006/8da74c5c-58ae-41b0-9883-c14727476b0f)

Have fun.
jF

