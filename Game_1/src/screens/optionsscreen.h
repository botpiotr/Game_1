#ifndef OPTIONSSCREEN_H
#define OPTIONSSCREEN_H

#include "abstractscreen.h"

class QSlider;
class QCheckBox;
class QLineEdit;

class OptionsScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit OptionsScreen(QWidget *parent = nullptr);

    // Recharge l'affichage depuis GameSettings (utile si les réglages
    // ont pu changer ailleurs, ex: reset via un autre écran).
    void onEnter() override;

private:
    void browseSaveDirectory();

    QSlider *m_volumeSlider;
    QCheckBox *m_fullscreenCheckBox;
    QLineEdit *m_saveDirectoryEdit;
};

#endif // OPTIONSSCREEN_H
