#ifndef BUZZERS_ST7796S_H
#define BUZZERS_ST7796S_H

#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <vector>
#include <algorithm>

#include "fonts/fonts.h"
#include "widgets/Widget.h"
#include "fonts/types.h"
#include "fonts/fonts.h"

#define ST7796S_DISP_WIDTH  480
#define ST7796S_DISP_HEIGHT 320

#define TOUCH_X_MIN 341
#define TOUCH_Y_MIN 334
#define TOUCH_X_MAX 3770
#define TOUCH_Y_MAX 3720

#define ST7796S_REG_SWRESET 0x01
#define ST7796S_REG_SLPOUT  0x11
#define ST7796S_REG_DISPON  0x29
#define ST7796S_REG_CASET   0x2A
#define ST7796S_REG_RASET   0x2B
#define ST7796S_REG_RAMWR   0x2C
#define ST7796S_REG_MADCTL  0x36

#define WHITE ((color_t) {0xFF, 0xFF, 0xFF})
#define BLACK ((color_t) {0x00, 0x00, 0x00})
#define GREEN ((color_t) {0x00, 0x9c, 0x60})
#define RED   ((color_t) {0xfc, 0x68, 0x60})
#define BLUE  ((color_t) {0x00, 0x48, 0xa0})

#define CLEAR_COLOR BLACK

class Widget;

class ST7796S {
public:
    static void begin(SPIClassRP2040 *spi, uint8_t lcd_cs, uint8_t lcd_dc, uint8_t lcd_reset, uint8_t touch_cs, uint8_t touch_irq);
    static void fullReset();
    static void wakeup();
    static void displayOn();
    static void _setDrawArea(rect_t rect);
    static void clear();
    static void _drawFilledRectangle(rect_t rect, color_t color);
    static void _drawRectangle(rect_t rect, uint16_t thickness, color_t color);
    static void _drawString(const char *text, const font_t *font, rect_t rect, color_t color, color_t bgColor);
    static void addWidget(Widget& widget);
    static void removeWidget(Widget& widget);
    static void update();

private:
    inline static SPIClassRP2040 *_spi;
    inline static uint8_t _lcd_cs, _lcd_dc, _lcd_reset;
    inline static auto _lcd_spi_settings = SPISettings(40000000, MSBFIRST, SPI_MODE0);
    inline static XPT2046_Touchscreen *_ts;

    static void _writeCommand(uint8_t reg, uint8_t *data, size_t len);
    static void _startRamWrite();
    static void _endRamWrite();
    static void _writeRam(const color_t *data, size_t len);
    static TS_Point _touchMapped(TS_Point p);
};

#endif //BUZZERS_ST7796S_H
