#ifndef BUZZERS_HOME_H
#define BUZZERS_HOME_H

#include <widgets/Button.h>
#include <fonts/adwaita_mono_15.h>

class Home {
public:
    static void show() {
        if (!_created) {
            btnStart.setMargin(76, 76, 105, 105);
            btnConfigure.setMargin(70, 70, 103, 103);

            btnStart.setOnClick([] {homeButton(1);});
            btnConfigure.setOnClick([] {homeButton(2);});

            _created = true;
        }

        ST7796S::addWidget(btnStart);
        ST7796S::addWidget(btnConfigure);
    }

    static void hide() {
        ST7796S::removeWidget(btnStart);
        ST7796S::removeWidget(btnConfigure);
    }

private:
    inline static bool _created = false;
    inline static Button btnStart {{10, 86}, {}, BLUE, "Demarrer", &adwaita_mono_15_font};
    inline static Button btnConfigure {{240, 86}, {}, BLUE, "Configurer", &adwaita_mono_15_font};

    static void homeButton(const uint8_t btnNb) {
        switch (btnNb) {
            case 1:
                hide();
                GameState::start();
                break;
            case 2:
                hide();
                break;
            default: ;
        }
    }
};

#endif //BUZZERS_HOME_H
