#include "creditsscreen.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>

CreditsScreen::CreditsScreen(QWidget *parent)
    : AbstractScreen(parent)
{
    // QTextEdit en lecture seule : gère nativement le défilement,
    // pratique pour une liste de crédits qui grossira avec le temps.
    auto *creditsText = new QTextEdit();
    creditsText->setReadOnly(true);
    creditsText->setPlainText(
        "Game_1\n\n"
        "Développement : BotPiotr\n\n"
        "(liste à compléter)"
    );

    auto *backButton = new QPushButton("Retour");
    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit navigateTo(ScreenId::MainMenu);
    });

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(creditsText);
    layout->addWidget(backButton);
}
