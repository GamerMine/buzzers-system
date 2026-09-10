#ifndef BUZZERS_PLAYERSETUP_H
#define BUZZERS_PLAYERSETUP_H

#include "fonts/adwaita_mono_15.h"
#include "gui/composable/PayerSettingsCard.h"
#include "widgets/Label.h"

class PlayerSetup : public Page {
public:
    void show() override {
        _cards.reserve(sizeof(BUZZERS_GPIO));
        ST7796S::addWidget(_lblInstr);
        ST7796S::_drawFilledRectangle({0, _lblInstr.rect().end.y + 10, ST7796S_DISP_WIDTH - 1, _lblInstr.rect().end.y + 10 + 2}, WHITE);

        for (int i = 0; i < sizeof(BUZZERS_GPIO); i++) {
            pinMode(BUZZERS_GPIO[i], INPUT_PULLUP);
            pinMode(BUZZERS_LED_GPIO[i], OUTPUT);
            attachInterruptParam(digitalPinToInterrupt(BUZZERS_GPIO[i]), cb, FALLING, reinterpret_cast<void *>(i));
        }
    }

    void hide() override {
        ST7796S::removeWidget(_lblInstr);
    }

    void update() override {
        if (_pendingIndex != -1) {
            const uint8_t id = Buzzer::add(BUZZERS_GPIO[_pendingIndex], BUZZERS_LED_GPIO[_pendingIndex], nullptr);
            //Buzzer::get(id)->enable();

            _cards.emplace_back(vector2_t{_lastCardPos.x + 10, _lastCardPos.y + _lblInstr.rect().end.y + 22}, "Joueur " + std::to_string(_pendingIndex), WHITE);
            _lastCardPos = {_cards.back().rect().end.x, 0};
            ST7796S::addWidget(_cards.back());
            _pendingIndex = -1;
        }
    }

private:
    inline static Label _lblInstr {{10, 10}, "Appuyez sur votre buzzer pour rejoindre la partie.", &adwaita_mono_15_font, WHITE, BLACK};
    inline static std::vector<PlayerSettingsCard> _cards;
    inline static std::vector<int> _addedIndices;
    inline static vector2_t _lastCardPos = {};
    inline static int _pendingIndex = -1;

    static void cb(void *index) {
        const auto i = reinterpret_cast<intptr_t>(index);
        if (_pendingIndex != -1 || _pendingIndex == i || std::find(_addedIndices.begin(), _addedIndices.end(), i) != _addedIndices.end()) return;
        _addedIndices.push_back(i);

        detachInterrupt(digitalPinToInterrupt(BUZZERS_GPIO[i]));
        _pendingIndex = i;
        Serial.printf("%d\n", i);
    }
};

#endif //BUZZERS_PLAYERSETUP_H
