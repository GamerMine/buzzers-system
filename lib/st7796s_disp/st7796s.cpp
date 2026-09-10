#include "st7796s.h"

#include <utility>

static auto widgets = std::vector<Widget*>();
static Page *currentPage;

void ST7796S::begin(SPIClassRP2040 *spi, const uint8_t lcd_cs, const uint8_t lcd_dc, const uint8_t lcd_reset, const uint8_t touch_cs, const uint8_t touch_irq) {
    _spi = spi;
    _lcd_cs = lcd_cs;
    _lcd_dc = lcd_dc;
    _lcd_reset = lcd_reset;
    _ts = new XPT2046_Touchscreen(touch_cs, touch_irq);

    pinMode(lcd_cs, OUTPUT);
    pinMode(lcd_reset, OUTPUT);
    pinMode(lcd_dc, OUTPUT);
    digitalWrite(lcd_cs, HIGH);

    // HW RESET
    digitalWrite(lcd_reset, LOW);
    delay(10);
    digitalWrite(lcd_reset, HIGH);

    spi->begin(false);
    _ts->begin(*_spi);
    _ts->setRotation(1);

    fullReset();
    _writeCommand(ST7796S_REG_MADCTL, (uint8_t[]){0x28}, 1);
    wakeup();
    displayOn();
}

void ST7796S::fullReset() {
    digitalWrite(_lcd_reset, LOW);
    delay(10);
    digitalWrite(_lcd_reset, HIGH);
    delay(10);

    _writeCommand(ST7796S_REG_SWRESET, nullptr, 0);
    delay(5);
}

void ST7796S::wakeup() {
    _writeCommand(ST7796S_REG_SLPOUT, nullptr, 0);
    delay(120);
}

void ST7796S::displayOn() {
    _writeCommand(ST7796S_REG_DISPON, nullptr, 0);
}

void ST7796S::_setDrawArea(const rect_t rect) {
    _writeCommand(ST7796S_REG_CASET, (uint8_t[]){
        static_cast<uint8_t>(rect.start.x >> 8),
        static_cast<uint8_t>(rect.start.x & 0xFF),
        static_cast<uint8_t>((rect.end.x - 1) >> 8),
        static_cast<uint8_t>((rect.end.x - 1) & 0xFF),
    }, 4);
    _writeCommand(ST7796S_REG_RASET, (uint8_t[]){
        static_cast<uint8_t>(rect.start.y >> 8),
        static_cast<uint8_t>(rect.start.y & 0xFF),
        static_cast<uint8_t>((rect.end.y - 1) >> 8),
        static_cast<uint8_t>((rect.end.y - 1) & 0xFF),
    }, 4);
}

void ST7796S::clear() {
    _drawFilledRectangle({0, 0, ST7796S_DISP_WIDTH, ST7796S_DISP_HEIGHT}, CLEAR_COLOR);
}

void ST7796S::_drawFilledRectangle(const rect_t rect, const color_t color) {
    _setDrawArea(rect);
    _startRamWrite();

    const uint16_t lineLength = rect.end.x - rect.start.x;
    color_t lineBuff[lineLength];
    std::fill_n(lineBuff, lineLength, color);

    for (size_t y = 0; y < rect.end.y - rect.start.y; y++) {
        _writeRam(lineBuff, lineLength * sizeof(color_t));
    }

    _endRamWrite();
}

void ST7796S::_drawRectangle(const rect_t rect, const uint16_t thickness, const color_t color) {
    // Top
    _drawFilledRectangle({
        {rect.start.x, rect.start.y},
        {rect.end.x, static_cast<uint16_t>(rect.start.y + thickness)}
    }, color);

    // Bottom
    _drawFilledRectangle({
        {rect.start.x, static_cast<uint16_t>(rect.end.y - thickness)},
        {rect.end.x, rect.end.y}
    }, color);

    // Left
    _drawFilledRectangle({
        {rect.start.x, static_cast<uint16_t>(rect.start.y + thickness)},
        {static_cast<uint16_t>(rect.start.x + thickness), static_cast<uint16_t>(rect.end.y - thickness)}
    }, color);

    // Right
    _drawFilledRectangle({
        {static_cast<uint16_t>(rect.end.x - thickness), static_cast<uint16_t>(rect.start.y + thickness)},
        {rect.end.x, static_cast<uint16_t>(rect.end.y - thickness)}
    }, color);
}

void ST7796S::_drawString(const std::string &text, const font_t *font, const rect_t rect, const color_t color, const color_t bgColor) {
    const size_t textLen = text.size();
    const uint16_t width = rect.end.x - rect.start.x;
    color_t rowBuffer[width];

    _setDrawArea(rect);
    _startRamWrite();

    for (uint16_t row = 0; row < rect.end.y - rect.start.y; row++) {
        std::fill_n(rowBuffer, width, bgColor);
        size_t bufferPos = 0;
        for (size_t c = 0; c < textLen; c++) {
            if (static_cast<uint8_t>(text[c]) == 0x20) {
                bufferPos += font->glyph_desc[0].width;
                continue;
            }
            const glyph_desc_t gd = font->glyph_desc[static_cast<uint8_t>(text[c]) - font->ascii_start];

            if (row < gd.height + gd.y_offset && row >= gd.y_offset) {
                const uint8_t *bytePtr = &font->glyph_data[gd.offset + (row - gd.y_offset) * gd.width / 8];
                uint8_t bit            = 7 - (row - gd.y_offset) * gd.width % 8;

                for (size_t col = 0; col < gd.width; col++) {
                    if ((*bytePtr >> bit) & 0x01) rowBuffer[bufferPos] = color;
                    bufferPos++;

                    if (bit == 0) {
                        bit = 7;
                        bytePtr++;
                    } else bit--;
                }
            } else bufferPos += gd.width;
        }

        _writeRam(rowBuffer, width * sizeof(color_t));
    }
    _endRamWrite();
}

void ST7796S::addWidget(Widget& widget) {
    widgets.push_back(&widget);}

void ST7796S::removeWidget(Widget& widget) {
    const auto it = std::find(
        widgets.begin(),
        widgets.end(),
        &widget
    );

    if (it != widgets.end()) {
        widget.markDirty();
        widgets.erase(it);
        _drawFilledRectangle(widget.rect(), CLEAR_COLOR);
    }
}

static bool wasTouched = false;
static TS_Point p;
void ST7796S::update() {
    // Rendering
    for (Widget *widget : widgets) {
        if (widget->isDirty()) {
            widget->draw();
        }
    }
    if (currentPage != nullptr) currentPage->show();

    // Input handling
    const bool touched = _ts->touched();

    if (touched && !wasTouched) p = _touchMapped(_ts->getPoint());
    if (!touched && wasTouched) {
        Serial.printf("CLICK x=%d y=%d\n", p.x, p.y);
        for (const Widget *widget : widgets) {
            if (widget->handleClickEvent(p) && widget->onClick != nullptr) widget->onClick();
        }
    }

    wasTouched = touched;

    // Update pages
    if (currentPage != nullptr) currentPage->update();
}

void ST7796S::setPage(Page *page) {
    if (currentPage != nullptr) currentPage->hide();
    currentPage = page;
    if (page != nullptr) page->markDirty();
}

void ST7796S::_writeCommand(const uint8_t reg, uint8_t *data, const size_t len) {
    _spi->beginTransaction(_lcd_spi_settings);
    digitalWrite(_lcd_cs, LOW);
    digitalWrite(_lcd_dc, LOW);
    _spi->transfer(reg);
    if (len > 0) {
        digitalWrite(_lcd_dc, HIGH);
        _spi->transfer(data, len);
        digitalWrite(_lcd_dc, LOW);
    }
    digitalWrite(_lcd_cs, HIGH);
    _spi->endTransaction();
}

void ST7796S::_startRamWrite() {
    _writeCommand(ST7796S_REG_RAMWR, nullptr, 0);
    _spi->beginTransaction(_lcd_spi_settings);
    digitalWrite(_lcd_cs, LOW);
    digitalWrite(_lcd_dc, HIGH);
}

void ST7796S::_endRamWrite() {
    digitalWrite(_lcd_cs, HIGH);
    digitalWrite(_lcd_dc, LOW);
    _spi->endTransaction();
}

void ST7796S::_writeRam(const color_t *data, const size_t len) {
    _spi->transfer(data, nullptr, len);
}

TS_Point ST7796S::_touchMapped(const TS_Point p) {
    TS_Point ret;

    ret.x =
        (TOUCH_X_MAX - p.x) *
        (ST7796S_DISP_WIDTH - 1) /
        (TOUCH_X_MAX - TOUCH_X_MIN);

    ret.y =
        ST7796S_DISP_HEIGHT - 1
        -
        (p.y - TOUCH_Y_MIN) *
        (ST7796S_DISP_HEIGHT - 1) /
        (TOUCH_Y_MAX - TOUCH_Y_MIN);

    ret.z = p.z;

    return ret;
}
