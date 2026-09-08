#ifndef BUZZERS_WIDGET_H
#define BUZZERS_WIDGET_H

#include <functional>

#include "../fonts/types.h"
#include "st7796s.h"

using ClickEvent = std::function<void()>;

class Widget {
public:
    virtual ~Widget() = default;
    virtual void draw() = 0;
    [[nodiscard]] virtual bool handleClickEvent(const TS_Point p) const {
        if (p.x >= this->_rect.start.x &&
            p.x <= this->_rect.end.x &&
            p.y >= this->_rect.start.y &&
            p.y <= this->_rect.end.y)
        {
            return true;
        }
        return false;
    }
    virtual void markDirty() { _dirty = true; }

    std::function<void()> onClick = nullptr;

    [[nodiscard]] rect_t rect() const { return _rect; }
    [[nodiscard]] bool isDirty() const { return _dirty; }

    void setOnClick(const ClickEvent& event) { onClick = event; }
    void setPosition(const vector2_t pos) {
        const int dx = pos.x - this->_rect.start.x;
        const int dy = pos.y - this->_rect.start.y;
        this->_rect.start.x = pos.x;
        this->_rect.start.y = pos.y;
        this->_rect.end.x += dx;
        this->_rect.end.y += dy;

        this->_dirty = true;
    }

protected:
    rect_t _rect = {};
    rect_t _old_rect = {};
    bool _dirty = true;
};

#endif //BUZZERS_WIDGET_H
