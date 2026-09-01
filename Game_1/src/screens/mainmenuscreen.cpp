#include "mainmenuscreen.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QApplication>

MainMenuScreen::MainMenuScreen(QWidget *parent)
    : AbstractScreen(parent)
{
    auto *title = new QLabel("Game_1");

    auto *playButton = new QPushButton("Jouer");
    auto *optionsButton = new QPushButton("Options");
    auto *creditsButton = new QPushButton("Crédits");
    auto *quitButton = new QPushButton("Quitter");

    connect(playButton, &QPushButton::clicked, this, [this]() {
        emit navigateTo(ScreenId::GameBoard);
    });
    connect(optionsButton, &QPushButton::clicked, this, [this]() {
        emit navigateTo(ScreenId::Options);
    });
    connect(creditsButton, &QPushButton::clicked, this, [this]() {
        emit navigateTo(ScreenId::Credits);
    });
    connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    auto *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(playButton);
    layout->addWidget(optionsButton);
    layout->addWidget(creditsButton);
    layout->addWidget(quitButton);
    layout->addStretch();
}
