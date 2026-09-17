#ifndef CREDITSSCREEN_H
#define CREDITSSCREEN_H

#include "abstractscreen.h"

class CreditsScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit CreditsScreen(QWidget *parent = nullptr);
};

#endif // CREDITSSCREEN_H
