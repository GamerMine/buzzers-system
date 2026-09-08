#ifndef BUZZERS_GAMESTATE_H
#define BUZZERS_GAMESTATE_H

#include <Arduino.h>
#include <string>

#include "Buzzer.h"
#include "config.h"
#include "gui/composable/PlayerResponseDialog.h"

class GameState {
public:
    static void init(BackgroundAudioWAV *wav) {
        Buzzer::add(BUZZER1_GPIO, BUZZER1_LED_GPIO, buzzer_pushed);
        Buzzer::add(BUZZER2_GPIO, BUZZER2_LED_GPIO, buzzer_pushed);

        _wav = wav;
    }

    static void start() {
        Buzzer::resetAll();
        Buzzer::enableAll();
    }

    static void update() {
        if (_f && _wav->availableForWrite() >= 512) {
            int len = _f.read(_filebuff, 512);

            if (len > 0) _wav->write(_filebuff, len);
            if (len < 512) _f.close();
        }

        if (_last_buzzer_id > 0) {
            _f = SD.open((std::to_string(_last_buzzer_id) + ".wav").c_str(), "r");
            Buzzer *sel_buzzer = Buzzer::get(_last_buzzer_id);

            Buzzer::disableAllExcept(_last_buzzer_id);

            switch (PlayerResponseDialog::open()) {
                case DIALOG_ACTION_YES:
                    sel_buzzer->incrementScore();
                    break;
                case DIALOG_ACTION_NO:
                    break;
                case DIALOG_ACTION_CLOSE:
                    break;
            }

            //  FIXME: TEMPORARY
            for (const Buzzer buzzer : Buzzer::getAll()) {
                Serial.printf("Player %d, score: %d\n", buzzer.id(), buzzer.score());
            }
            Serial.println();

            _last_buzzer_id = 0;
        }
    }

private:
    inline static File _f = {};
    inline static unsigned char _filebuff[512] = {};
    inline static BackgroundAudioWAV *_wav = nullptr;

    inline static uint8_t _last_buzzer_id = 0;

    static void buzzer_pushed(void *pin) {
        const Buzzer *buzzer = static_cast<Buzzer *>(pin);

        if (!buzzer->isEnabled() || _f || _last_buzzer_id == buzzer->id()) return;

        _last_buzzer_id = buzzer->id();
    }
};

#endif //BUZZERS_GAMESTATE_H
