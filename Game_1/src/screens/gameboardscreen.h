#ifndef GAMEBOARDSCREEN_H
#define GAMEBOARDSCREEN_H

#include "abstractscreen.h"

class QGraphicsScene;
class QGraphicsView;

class GameBoardScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit GameBoardScreen(QWidget *parent = nullptr);

    // Redéfini depuis AbstractScreen : reconstruit une partie neuve
    // à chaque fois qu'on arrive sur cet écran depuis le menu "Jouer".
    void onEnter() override;

private:
    void setupNewGame();

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
};

#endif // GAMEBOARDSCREEN_H
