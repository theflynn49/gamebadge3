
This is a kicad project for an adapter board to install a cheap Aliexpress TZT screen on your GameBadge.

Beware : BenHeck is NOT recommanding this, since cheap display modules may not work well.

On another hand, they cost about $3 instead of $17 as of today.

I tried these up to now :
 
 - <url>https://fr.aliexpress.com/item/1005005921876295.html</url> (round corners without pixels, upside-down)
 - <url>https://fr.aliexpress.com/item/1005005933513340.html</url> (tested OK, is upside-down)
 - <url>https://fr.aliexpress.com/item/32858069771.html</url> (7-wires, do NOT work yet)

Important note : make sure you use my version of gameBadgePico.cpp (as of 2024/02/24), which contains a modification to make the chinese TZT screen stable.
I pushed this mod on both my branches of this repo.

I tested the gerber files with PCBway.

This has been tested now, it works fine. 

Have fun.
