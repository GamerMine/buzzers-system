#include "Label.h"

Label::Label(const vector2_t pos, const char *text, const font_t *font, const color_t textColor, const color_t bgColor)
    : _text(nullptr), _font(font), _textColor(textColor), _bgColor(bgColor) {
    const size_t strLen = strlen(text);
    const auto filteredText = new char[strLen + 1];

    uint16_t maxWidth = 0;
    uint16_t maxHeight = 0;
    size_t cIndex = 0;

    for (size_t i = 0; i < strLen; i++) {
        const uint8_t c = text[i];
        if (c != 0x20 && (c < font->ascii_start || c > font->ascii_end)) continue;

        filteredText[cIndex++] = static_cast<char>(c);
        if (c == 0x20) {
            maxWidth += font->glyph_desc[0].width;
            continue;
        }
        const glyph_desc_t *glyph = &font->glyph_desc[c - font->ascii_start];
        maxWidth += glyph->width;
        if (glyph->height + glyph->y_offset > maxHeight) maxHeight = glyph->height + glyph->y_offset;
    }

    filteredText[cIndex] = '\0';

    this->_text = filteredText;
    this->_rect = rect_from_vector2(pos, {static_cast<uint16_t>(maxWidth + pos.x), static_cast<uint16_t>(maxHeight + pos.y)});
    this->_old_rect = this->_rect;
}

void Label::draw() {
    if (this->_dirty)
        ST7796S::_drawString(this->_text, this->_font, this->_rect, this->_textColor, this->_bgColor);

    this->_dirty = false;
}
