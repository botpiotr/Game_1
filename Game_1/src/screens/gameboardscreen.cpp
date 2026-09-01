#include "gameboardscreen.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>

// #include "hextileitem.h"

GameBoardScreen::GameBoardScreen(QWidget *parent)
    : AbstractScreen(parent)
{
    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene);
    m_view->setRenderHint(QPainter::Antialiasing);

    auto *backButton = new QPushButton("Retour au menu");
    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit navigateTo(ScreenId::MainMenu);
    });

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_view);
    layout->addWidget(backButton);
}

void GameBoardScreen::onEnter()
{
    // Repart d'un plateau neuf à chaque nouvelle partie
    setupNewGame();
}

void GameBoardScreen::setupNewGame()
{
    m_scene->clear();

    // // Exemple d'installation initiale : une petite grille hexagonale.
    // // À remplacer par la vraie logique de mise en place du jeu.
    // for (int q = -2; q <= 2; ++q) {
    //     for (int r = -2; r <= 2; ++r) {
    //         auto *tile = new HexTileItem(40.0, HexTileItem::Orientation::PointyTop);
    //         tile->setAxialCoords(q, r);
    //         tile->updatePositionFromAxialCoords();
    //         m_scene->addItem(tile);
    //     }
    // }
}
