#ifndef BUZZERS_BUTTON_H
#define BUZZERS_BUTTON_H
#include "Label.h"
#include "Widget.h"

class Button : public Widget {
public:
    Button(vector2_t pos, vector2_t size, color_t bgColor, const char *text, const font_t *font, color_t textColor = WHITE);
    Button(const vector2_t pos, const vector2_t size, const color_t bgColor) : Button(pos, size, bgColor, "", nullptr) {}

    void draw() override;
    void setMargin(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom);
    void markDirty() override;

private:
    color_t _bgColor;
    Label *_label;

};

#endif //BUZZERS_BUTTON_H
