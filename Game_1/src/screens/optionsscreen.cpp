#include "optionsscreen.h"
#include "../settings/gamesettings.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QLineEdit>
#include <QFileDialog>

OptionsScreen::OptionsScreen(QWidget *parent)
    : AbstractScreen(parent)
{
    auto *title = new QLabel("Options");

    // --- Volume ---
    m_volumeSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider->setRange(0, 100);
    connect(m_volumeSlider, &QSlider::valueChanged, this, [](int value) {
        GameSettings::instance().setVolume(value);
    });

    // --- Plein écran ---
    m_fullscreenCheckBox = new QCheckBox("Activer le plein écran");
    connect(m_fullscreenCheckBox, &QCheckBox::toggled, this, [](bool checked) {
        GameSettings::instance().setFullscreen(checked);
    });

    // --- Dossier de sauvegarde ---
    m_saveDirectoryEdit = new QLineEdit();
    m_saveDirectoryEdit->setReadOnly(true); // modification uniquement via le sélecteur

    auto *browseButton = new QPushButton("Parcourir...");
    connect(browseButton, &QPushButton::clicked, this, &OptionsScreen::browseSaveDirectory);

    auto *saveDirLayout = new QHBoxLayout();
    saveDirLayout->addWidget(m_saveDirectoryEdit);
    saveDirLayout->addWidget(browseButton);

    auto *formLayout = new QFormLayout();
    formLayout->addRow("Volume", m_volumeSlider);
    formLayout->addRow("Affichage", m_fullscreenCheckBox);
    formLayout->addRow("Dossier de sauvegarde", saveDirLayout);

    auto *backButton = new QPushButton("Retour");
    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit navigateTo(ScreenId::MainMenu);
    });

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addLayout(formLayout);
    layout->addStretch();
    layout->addWidget(backButton);
}

void OptionsScreen::onEnter()
{
    // Synchronise l'affichage avec l'état actuel de GameSettings à chaque
    // fois qu'on arrive sur cet écran (bloque les signaux le temps de
    // l'init pour ne pas déclencher une écriture disque inutile).
    const GameSettings &settings = GameSettings::instance();

    m_volumeSlider->blockSignals(true);
    m_volumeSlider->setValue(settings.volume());
    m_volumeSlider->blockSignals(false);

    m_fullscreenCheckBox->blockSignals(true);
    m_fullscreenCheckBox->setChecked(settings.fullscreen());
    m_fullscreenCheckBox->blockSignals(false);

    m_saveDirectoryEdit->setText(settings.saveDirectory());
}

void OptionsScreen::browseSaveDirectory()
{
    const QString chosen = QFileDialog::getExistingDirectory(
        this,
        "Choisir le dossier de sauvegarde",
        GameSettings::instance().saveDirectory()
        );

    if (chosen.isEmpty()) {
        return; // annulé par l'utilisateur
    }

    GameSettings::instance().setSaveDirectory(chosen);
    m_saveDirectoryEdit->setText(chosen);
}
