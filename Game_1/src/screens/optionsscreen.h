#ifndef OPTIONSSCREEN_H
#define OPTIONSSCREEN_H

#include "abstractscreen.h"

class OptionsScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit OptionsScreen(QWidget *parent = nullptr);
};

#endif // OPTIONSSCREEN_H
