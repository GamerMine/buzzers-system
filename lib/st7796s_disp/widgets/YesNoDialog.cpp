#include "YesNoDialog.h"

YesNoDialog::YesNoDialog(const char *contentText, DialogAction dialogAction)
    : _dialogAction(dialogAction), _contentLabel({90, 115}, contentText, &adwaita_mono_15_font, WHITE, BLACK),
      _yesBtn({320, 180}, {}, GREEN, "Oui", &adwaita_mono_15_font),
      _noBtn({110, 180}, {}, RED, "Non", &adwaita_mono_15_font),
      _closeBtn({375, 82}, {}, RED, "X", &adwaita_mono_15_font)
{
    this->_rect = {80, 80, 400, 240};
    this->_old_rect = this->_rect;

    this->_yesBtn.setMargin(10, 10, 10, 15);
    this->_noBtn.setMargin(10, 10, 10, 15);
    this->_closeBtn.setMargin(7, 7, 3, 5);
}

void YesNoDialog::draw() {
    if (_dirty) {
        ST7796S::_drawRectangle(this->_rect, 2, WHITE);
        ST7796S::_drawFilledRectangle({this->_rect.start.x, static_cast<uint16_t>(this->_rect.start.y + 25), this->_rect.end.x, static_cast<uint16_t>(this->_rect.start.y + 27)}, WHITE);
        this->_contentLabel.draw();
        this->_yesBtn.draw();
        this->_noBtn.draw();
        this->_closeBtn.draw();
    }

    _dirty = false;
}

bool YesNoDialog::handleClickEvent(const TS_Point p) const {
    if (this->_yesBtn.handleClickEvent(p)) this->_dialogAction(DIALOG_ACTION_YES);
    if (this->_noBtn.handleClickEvent(p)) this->_dialogAction(DIALOG_ACTION_NO);
    if (this->_closeBtn.handleClickEvent(p)) this->_dialogAction(DIALOG_ACTION_CLOSE);

    return Widget::handleClickEvent(p);
}

void YesNoDialog::markDirty() {
    this->_yesBtn.markDirty();
    this->_noBtn.markDirty();
    this->_closeBtn.markDirty();
    this->_contentLabel.markDirty();
    Widget::markDirty();
}
