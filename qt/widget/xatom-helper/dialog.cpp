#include "dialog.h"
#include "xatom-helper.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XatomHelper::getInstance()->setWindowMotifHint(winId(), hints);
}

Dialog::~Dialog()
{
}

