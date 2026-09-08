#include "Button.h"

Button::Button(const vector2_t pos, const vector2_t size, const color_t bgColor, const char *text, const font_t *font, const color_t textColor)
    : _bgColor(bgColor), _label(nullptr) {
    if (font != nullptr) this->_label = new Label(pos, text, font, textColor, bgColor);
    if (this->_label != nullptr && size.x == 0 && size.y == 0)
        this->_rect = rect_from_vector2(pos, {this->_label->rect().end.x, this->_label->rect().end.y});
    else
        this->_rect = rect_from_vector2(pos, {static_cast<uint16_t>(pos.x + size.x), static_cast<uint16_t>(pos.y + size.y)});
    this->_old_rect = _rect;
}

void Button::draw() {
    ST7796S::_drawFilledRectangle(this->_rect, this->_bgColor);
    if (this->_label != nullptr) this->_label->draw();
    this->_dirty = false;
}

void Button::setMargin(const uint16_t left, const uint16_t right, const uint16_t top, const uint16_t bottom) {
    this->_rect.end.x += left + right;
    this->_rect.end.y += top + bottom;
    this->_label->setPosition({static_cast<uint16_t>(this->_label->rect().start.x + left), static_cast<uint16_t>(this->_label->rect().start.y + top)});
    this->_dirty = true;
}

void Button::markDirty() {
    this->_label->markDirty();
    Widget::markDirty();
}
