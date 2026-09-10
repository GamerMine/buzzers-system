#ifndef BUZZERS_LABEL_H
#define BUZZERS_LABEL_H

#include <string>

#include "Widget.h"
#include "fonts/fonts.h"

class Label : public Widget {
public:
    Label(vector2_t pos, const std::string &text, const font_t *font, color_t textColor, color_t bgColor);
    void draw() override;

private:
    std::string _text;
    const font_t *_font;
    color_t _textColor, _bgColor;
};


#endif //BUZZERS_LABEL_H
