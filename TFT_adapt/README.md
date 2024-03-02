
This is a kicad project for an adapter board to install a cheap Aliexpress TZT screen on your GameBadge.

Beware : BenHeck is NOT recommanding this, since cheap display modules may not work well.

On another hand, they cost about $3 instead of $17 as of today.

I tried these up to now :
 
 - <url>https://fr.aliexpress.com/item/1005005921876295.html</url> (round corners without pixels, upside-down)
 - <url>https://fr.aliexpress.com/item/1005005933513340.html</url> (tested OK, is upside-down)
 - <url>https://fr.aliexpress.com/item/32858069771.html</url> (7-wires, tested OK, is upside down)

For the 7-bit screen, you need to change gameBadgePico.cpp, and make the following change :
<code>
const struct st7789_config lcd_config = {
    .spi      = PICO_DEFAULT_SPI_INSTANCE,
    .gpio_din = PICO_DEFAULT_SPI_TX_PIN,
    .gpio_clk = PICO_DEFAULT_SPI_SCK_PIN,
    .gpio_cs  = PICO_DEFAULT_SPI_CSN_PIN,
    .gpio_dc  = 20,
    .gpio_rst = 21,
    .gpio_bl  = 22
};
</code>

has to be replaced by :
<code>
const struct st7789_config lcd_config = {
    .spi      = PICO_DEFAULT_SPI_INSTANCE,
    .gpio_din = PICO_DEFAULT_SPI_TX_PIN,
    .gpio_clk = PICO_DEFAULT_SPI_SCK_PIN,
    .gpio_cs  = -1, // <- to indicate only 7 pins, no CS
    .gpio_dc  = 20,
    .gpio_rst = 21,
    .gpio_bl  =  PICO_DEFAULT_SPI_CSN_PIN // <- if you use my TFT_adapt board this will be misrouted, so use CS instead of BLK
};
</code>
and you'll leave the pin "Blk" not connected on all connectors.
<br/>
Important note : make sure you use my version of gameBadgePico.cpp (as of 2024/02/24), which contains a modification to make the chinese TZT screen stable.
I pushed this mod on both my branches of this repo.

I tested the gerber files with PCBway.

This has been tested now, it works fine. 

Have fun.
