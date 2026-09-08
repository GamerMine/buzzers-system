#ifndef BUZZERS_PLAYERRESPONSEDIALOG_H
#define BUZZERS_PLAYERRESPONSEDIALOG_H

#include <widgets/YesNoDialog.h>

class PlayerResponseDialog {
public:
    static DialogActionType open() {
        ST7796S::addWidget(playerRespDialog);
        _type = nullptr;

        while (true) {
            sleep_ms(10);
            if (_type != nullptr) {
                return *_type;
            }
        }
    }

private:
    static void pDialogRespAction(const DialogActionType type) {
        _type = reinterpret_cast<DialogActionType *>(type);
        ST7796S::removeWidget(playerRespDialog);
    }

    inline static YesNoDialog playerRespDialog {"La reponse est-elle correcte ?", pDialogRespAction};
    inline static DialogActionType *_type = nullptr;
};

#endif //BUZZERS_PLAYERRESPONSEDIALOG_H
