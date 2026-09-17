#include "screenmanager.h"

ScreenManager::ScreenManager(QWidget *parent)
    : QStackedWidget(parent)
{
}

void ScreenManager::registerScreen(ScreenId id, AbstractScreen *screen)
{
    m_screens.insert(id, screen);
    addWidget(screen);

    // Connexion automatique : dès qu'un écran demande une navigation,
    // le ScreenManager s'en charge. Pas besoin de connecter chaque écran
    // manuellement à l'extérieur.
    connect(screen, &AbstractScreen::navigateTo, this, &ScreenManager::showScreen);
}

void ScreenManager::showScreen(ScreenId id)
{
    if (!m_screens.contains(id)) {
        return;
    }

    if (auto *current = m_screens.value(m_currentScreenId, nullptr)) {
        current->onExit();
    }

    AbstractScreen *next = m_screens.value(id);
    setCurrentWidget(next);
    next->onEnter();

    m_currentScreenId = id;
}
