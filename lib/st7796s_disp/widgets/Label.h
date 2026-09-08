#ifndef BUZZERS_LABEL_H
#define BUZZERS_LABEL_H

#include "Widget.h"
#include "fonts/fonts.h"

class Label : public Widget {
public:
    Label(vector2_t pos, const char *text, const font_t *font, color_t textColor, color_t bgColor);
    void draw() override;

private:
    const char *_text;
    const font_t *_font;
    color_t _textColor, _bgColor;
};


#endif //BUZZERS_LABEL_H
