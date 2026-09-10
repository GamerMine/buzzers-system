#include "Label.h"

Label::Label(const vector2_t pos, const std::string &text, const font_t *font, const color_t textColor, const color_t bgColor)
    : _font(font), _textColor(textColor), _bgColor(bgColor) {
    uint16_t maxWidth = 0;
    uint16_t maxHeight = 0;

    _text.reserve(text.size());

    for (const unsigned char c : text) {
        if (c != 0x20 && (c < font->ascii_start || c > font->ascii_end)) continue;

        _text.push_back(static_cast<char>(c));

        if (c == 0x20) {
            maxWidth += font->glyph_desc[0].width;
            continue;
        }

        const glyph_desc_t* glyph = &font->glyph_desc[c - font->ascii_start];

        maxWidth += glyph->width;

        if (glyph->height + glyph->y_offset > maxHeight) maxHeight = glyph->height + glyph->y_offset;
    }

    this->_rect = rect_from_vector2(pos, {static_cast<uint16_t>(maxWidth + pos.x), static_cast<uint16_t>(maxHeight + pos.y)});
    this->_old_rect = this->_rect;
}

void Label::draw() {
    if (this->_dirty)
        ST7796S::_drawString(this->_text, this->_font, this->_rect, this->_textColor, this->_bgColor);

    this->_dirty = false;
}
