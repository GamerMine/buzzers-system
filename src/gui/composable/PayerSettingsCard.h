#ifndef BUZZERS_PAYERSETTINGSCARD_H
#define BUZZERS_PAYERSETTINGSCARD_H

#include "fonts/adwaita_mono_15.h"
#include "widgets/Label.h"
#include "widgets/Widget.h"

class PlayerSettingsCard : public Widget {
public:
    PlayerSettingsCard(const vector2_t pos, const std::string &username, const color_t playerColor) : _lblUsername(nullptr), _playerColor(playerColor) {
        this->_rect = rect_from_vector2(pos, {pos.x + 114, pos.y + 59});
        this->_old_rect = _rect;
        this->_lblUsername = new Label({pos.x + 10, pos.y + 10}, username, &adwaita_mono_15_font, WHITE, BLACK);
    }

    void draw() override {
        ST7796S::_drawRectangle(this->_rect, 2, WHITE);
        this->_lblUsername->draw();
        ST7796S::_drawFilledRectangle({this->_rect.start.x + 10, this->_rect.start.y + 30, this->_rect.end.x - 10, this->_rect.end.y - 10}, _playerColor);
        this->_dirty = false;
    }

    void markDirty() override {
        this->_lblUsername->markDirty();
        Widget::markDirty();
    }

private:
    Label *_lblUsername;
    color_t _playerColor;
};

#endif //BUZZERS_PAYERSETTINGSCARD_H
