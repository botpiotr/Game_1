#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include "abstractscreen.h"

class MainMenuScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit MainMenuScreen(QWidget *parent = nullptr);
};

#endif // MAINMENUSCREEN_H
