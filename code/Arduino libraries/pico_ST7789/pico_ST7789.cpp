//ST7789 driver for Pi Pico based Gamebadge3
//Uses this library mostly for setup then dumps data via DMA

#include "Arduino.h"
#include "pico_ST7789.h"

#include <string.h>
#include "hardware/gpio.h"

struct st7789_config st7789_cfg;
uint16_t st7789_width;
uint16_t st7789_height;
bool st7789_data_mode = false;

#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40

#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10

#define ST77XX_MADCTL_RGB 0x00

#define SCR_WIDTH 320    // Beware some 280x240 screens are in fact 320x240 screens

#define LCD_OFFSET_X ((SCR_WIDTH-240)/2) // set his to (screen width-240)/2, to set black bands left and right
#define SCR_OFFSET_X (320-SCR_WIDTH)     // set this to 320-screen width 
#define LCD_OFFSET_Y 0   // not used yet leave it to 0
#define LCD_ROTATION 1

uint8_t _xstart;
uint8_t _ystart;

void st7789_init(const struct st7789_config* config, uint16_t width, uint16_t height)
{
    memcpy(&st7789_cfg, config, sizeof(st7789_cfg));				//Copy referenced structure to our own copy
    st7789_width = width;
    st7789_height = height;

    spi_init(st7789_cfg.spi, 125 * 1000 * 1000);					//Set SPI to max speed, CPU must be 125Hz
    
    
    if (st7789_cfg.gpio_cs > -1) {
        spi_set_format(st7789_cfg.spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    } else {
        spi_set_format(st7789_cfg.spi, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
    }

    gpio_set_function(st7789_cfg.gpio_din, GPIO_FUNC_SPI);
    gpio_set_function(st7789_cfg.gpio_clk, GPIO_FUNC_SPI);

    if (st7789_cfg.gpio_cs > -1) {
        gpio_init(st7789_cfg.gpio_cs);
    }
    gpio_init(st7789_cfg.gpio_dc);
    gpio_init(st7789_cfg.gpio_rst);
    gpio_init(st7789_cfg.gpio_bl);

    if (st7789_cfg.gpio_cs > -1) {
        gpio_set_dir(st7789_cfg.gpio_cs, GPIO_OUT);
    }
    gpio_set_dir(st7789_cfg.gpio_dc, GPIO_OUT);
    gpio_set_dir(st7789_cfg.gpio_rst, GPIO_OUT);
    gpio_set_dir(st7789_cfg.gpio_bl, GPIO_OUT);

	gpio_put(st7789_cfg.gpio_bl, 1);

    if (st7789_cfg.gpio_cs > -1) {
        gpio_put(st7789_cfg.gpio_cs, 1);
    }
    gpio_put(st7789_cfg.gpio_dc, 1);
	gpio_put(st7789_cfg.gpio_rst, 0);
	sleep_ms(150);
    gpio_put(st7789_cfg.gpio_rst, 1);
    sleep_ms(100);
    
    // SWRESET (01h): Software Reset
    st7789_cmd(0x01, NULL, 0);
    sleep_ms(150);

    // SLPOUT (11h): Sleep Out
    st7789_cmd(0x11, NULL, 0);
    sleep_ms(10);

	st7789_setRotation(LCD_ROTATION);

    // COLMOD (3Ah): Interface Pixel Format
    // - RGB interface color format     = 65K of RGB interface
    // - Control interface color format = 16bit/pixel

    //uint8_t cmd = 0x2c;
    //spi_write_blocking(st7789_cfg.spi, &cmd, sizeof(cmd));
    
    uint8_t cmd = 0x55;
    st7789_cmd(0x3a, &cmd, 1);
    sleep_ms(10);

    st7789_caset(0, width - 1);
    st7789_raset(0, height - 1);
	
    // INVON (21h): Display Inversion On
    st7789_cmd(0x21, NULL, 0);
    sleep_ms(10);

    // NORON (13h): Normal Display Mode On
    st7789_cmd(0x13, NULL, 0);
    sleep_ms(10);

    st7789_razmem() ;
    
    // DISPON (29h): Display On
    st7789_cmd(0x29, NULL, 0);
    sleep_ms(10);
	 
}

void st7789_backlight(bool state) {

	if (state == true) {
		gpio_put(st7789_cfg.gpio_bl, 1);
	}
	else {
		gpio_put(st7789_cfg.gpio_bl, 0);
	}
	
}

void st7789_cmd(uint8_t cmd, const uint8_t* data, size_t len)
{
    if (st7789_cfg.gpio_cs > -1) {
        spi_set_format(st7789_cfg.spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    } else {
        spi_set_format(st7789_cfg.spi, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
    }
    st7789_data_mode = false;

    sleep_us(1);
    if (st7789_cfg.gpio_cs > -1) {
        gpio_put(st7789_cfg.gpio_cs, 0);
    }
    gpio_put(st7789_cfg.gpio_dc, 0);
    sleep_us(1);
    
    spi_write_blocking(st7789_cfg.spi, &cmd, sizeof(cmd));
    
    if (len) {
        sleep_us(1);
        gpio_put(st7789_cfg.gpio_dc, 1);
        sleep_us(1);
        
        spi_write_blocking(st7789_cfg.spi, data, len);
    }

    sleep_us(1);
    if (st7789_cfg.gpio_cs > -1) {
        gpio_put(st7789_cfg.gpio_cs, 1);
    }
    gpio_put(st7789_cfg.gpio_dc, 1);
    sleep_us(1);
}


void st7789_rascaset(uint8_t cmd, uint16_t xs, uint16_t xe, uint16_t offset)
{
	uint8_t data[4];
        uint16_t _tmp ;
	
        _tmp = xs+offset ;
	data[0] = _tmp >> 8;
	data[1] = _tmp & 0xFF;
        _tmp = xe+offset ;
	data[2] = _tmp >> 8;
	data[3] = _tmp & 0xFF;
	
        st7789_cmd(cmd, data, sizeof(data));			// CASET (2Ah): Column Address Set
}

inline void st7789_caset(uint16_t xs, uint16_t xe)
{
        st7789_rascaset(0x2a, xs, xe, LCD_OFFSET_X) ;
}

inline void st7789_raset(uint16_t ys, uint16_t ye)
{
        st7789_rascaset(0x2b, ys, ye, LCD_OFFSET_Y) ;
}

void st7789_ramwr()
{
    sleep_us(1);
    if (st7789_cfg.gpio_cs > -1) {
        gpio_put(st7789_cfg.gpio_cs, 0);
    }
    gpio_put(st7789_cfg.gpio_dc, 0);
    sleep_us(1);

    // RAMWR (2Ch): Memory Write
    uint8_t cmd = 0x2c;
    spi_write_blocking(st7789_cfg.spi, &cmd, sizeof(cmd));

    sleep_us(1);

	if (st7789_cfg.gpio_cs > -1) {
		gpio_put(st7789_cfg.gpio_cs, 0);
		spi_set_format(st7789_cfg.spi, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
	} else {
		spi_set_format(st7789_cfg.spi, 16, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
	}	

    gpio_put(st7789_cfg.gpio_dc, 1);
    sleep_us(1);
}

void st7789_set_cursor(uint16_t x, uint16_t y)
{
    st7789_caset(x, st7789_width - 1);
    st7789_raset(y, st7789_height - 1);
}

void st7789_setRotation(uint8_t which) {
	
  uint8_t cmd = 0;

  switch (which & 0x03) {
	  case 0:
		cmd = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
		_xstart = 0;
		_ystart = 80;
		break;
	  case 1:
		cmd = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
		_xstart = SCR_OFFSET_X;  
		_ystart = 0;
		break;
	  case 2:
		cmd = ST77XX_MADCTL_RGB;
		_xstart = 0;
		_ystart = 0;
		break;
	  case 3:
		cmd = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
		_xstart = 0;
		_ystart = 0;
		break;
  }

  st7789_cmd(0x36, &cmd, 1);
	
}

void st7789_setAddressWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	
	x += _xstart;
	y += _ystart;
	uint32_t xa = ((uint32_t)x << 16) | (x + w - 1);
	uint32_t ya = ((uint32_t)y << 16) | (y + h - 1);

	st7789_caset(x, x + (w - 1));		 // Column addr set
	st7789_raset(y, y + (h - 1)); 		// Row addr set

}

void st7789_razmem()
{   
    uint8_t data = 0;
    uint16_t i ;
#if LCD_OFFSET_X != 0
        st7789_rascaset(0x2a, _xstart, _xstart+LCD_OFFSET_X-1, 0) ;
        st7789_rascaset(0x2b, 0, st7789_height-1, 0) ;
        st7789_ramwr();
        for (i=0; i<LCD_OFFSET_X*st7789_height; ++i)
            spi_write_blocking(st7789_cfg.spi, &data, 1);     
        sleep_us(1);
        st7789_rascaset(0x2a, _xstart+LCD_OFFSET_X+st7789_width, _xstart+st7789_width+(2*LCD_OFFSET_X)-1, 0) ;
        st7789_rascaset(0x2b, 0, st7789_height - 1, 0) ;
        st7789_ramwr();
        for (i=0; i<LCD_OFFSET_X*st7789_height; ++i)
            spi_write_blocking(st7789_cfg.spi, &data, 1); 
        sleep_us(1);
#endif
}

