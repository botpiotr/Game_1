#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <QStackedWidget>
#include <QMap>

#include "abstractscreen.h"

/**
 * @brief Gère l'affichage et la navigation entre tous les écrans du jeu.
 *
 * À utiliser comme widget central d'une QMainWindow (ou directement comme
 * fenêtre principale). Chaque écran est enregistré une seule fois via
 * registerScreen(), puis la navigation se fait uniquement via showScreen()
 * ou automatiquement lorsqu'un écran émet son signal navigateTo().
 */
class ScreenManager : public QStackedWidget
{
    Q_OBJECT

public:
    explicit ScreenManager(QWidget *parent = nullptr);

    // Enregistre un écran sous un identifiant. Le ScreenManager prend
    // possession du widget (parent Qt classique).
    void registerScreen(ScreenId id, AbstractScreen *screen);

    // Affiche l'écran correspondant à l'identifiant. Ne fait rien si
    // l'identifiant n'a pas été enregistré au préalable.
    void showScreen(ScreenId id);

private:
    QMap<ScreenId, AbstractScreen *> m_screens;
    ScreenId m_currentScreenId = ScreenId::MainMenu;
};

#endif // SCREENMANAGER_H
