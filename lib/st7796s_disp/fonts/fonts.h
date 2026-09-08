#ifndef GM_WATCHY_FIRMWARE_FONTS_H
#define GM_WATCHY_FIRMWARE_FONTS_H

typedef struct {
    uint32_t offset;
    uint8_t x_advance;
    uint8_t width;
    uint8_t height;
    uint8_t x_offset;
    uint8_t y_offset;
} glyph_desc_t;

typedef struct {
    uint8_t ascii_start;
    uint8_t ascii_end;
    const glyph_desc_t *glyph_desc;
    const uint8_t *glyph_data;
} font_t;

#endif //GM_WATCHY_FIRMWARE_FONTS_H
