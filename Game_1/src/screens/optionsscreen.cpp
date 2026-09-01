#include "optionsscreen.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

OptionsScreen::OptionsScreen(QWidget *parent)
    : AbstractScreen(parent)
{
    auto *title = new QLabel("Options");

    // Placeholder : à remplacer plus tard par de vrais réglages
    // (volume, résolution, contrôles, etc.)
    auto *placeholder = new QLabel("(réglages à venir)");

    auto *backButton = new QPushButton("Retour");
    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit navigateTo(ScreenId::MainMenu);
    });

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addWidget(placeholder);
    layout->addStretch();
    layout->addWidget(backButton);
}
