#ifndef BUZZERS_TYPES_H
#define BUZZERS_TYPES_H

#include <cstdint>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

typedef struct {
    uint16_t x;
    uint16_t y;
} vector2_t;

typedef struct {
    vector2_t start;
    vector2_t end;
} rect_t;

inline rect_t rect_from_vector2(const vector2_t v1, const vector2_t v2) {
    return {v1.x, v1.y, v2.x, v2.y};
}

inline vector2_t vector2_sum(const vector2_t v1, const vector2_t v2) {
    return {static_cast<uint16_t>(v1.x + v2.x), static_cast<uint16_t>(v1.y + v2.y)};
}

#endif //BUZZERS_TYPES_H
