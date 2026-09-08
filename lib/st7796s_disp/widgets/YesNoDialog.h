#ifndef BUZZERS_YESNODIALOG_H
#define BUZZERS_YESNODIALOG_H

#include "Button.h"
#include "Widget.h"
#include "fonts/adwaita_mono_15.h"

enum DialogActionType {
    DIALOG_ACTION_YES,
    DIALOG_ACTION_NO,
    DIALOG_ACTION_CLOSE,
};

typedef void DialogAction(DialogActionType);

class YesNoDialog : public Widget {
public:
    YesNoDialog(const char *contentText, DialogAction dialogAction);

    void draw() override;
    [[nodiscard]] bool handleClickEvent(TS_Point p) const override;
    void markDirty() override;

private:
    DialogAction *_dialogAction;
    Label _contentLabel;
    Button _yesBtn;
    Button _noBtn;
    Button _closeBtn;
};


#endif //BUZZERS_YESNODIALOG_H
