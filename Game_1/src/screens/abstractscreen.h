#ifndef ABSTRACTSCREEN_H
#define ABSTRACTSCREEN_H

#include <QWidget>

/**
 * @brief Identifiants uniques de chaque écran de l'application.
 *        Ajoute une nouvelle valeur ici à chaque nouvel écran créé.
 */
enum class ScreenId {
    MainMenu,
    Options,
    Credits,
    GameBoard
};

/**
 * @brief Classe de base pour tout écran affichable par le ScreenManager.
 *
 * Un écran ne connaît jamais les autres écrans directement : il communique
 * son intention de navigation uniquement via le signal navigateTo(), que
 * le ScreenManager écoute et traduit en changement d'écran affiché.
 */
class AbstractScreen : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractScreen(QWidget *parent = nullptr) : QWidget(parent) {}
    ~AbstractScreen() override = default;

    // Appelé par le ScreenManager juste avant que l'écran devienne visible.
    // Redéfinissable par les sous-classes (ex: rafraîchir une liste d'options).
    virtual void onEnter() {}

    // Appelé juste avant qu'un autre écran ne prenne le relais.
    virtual void onExit() {}

signals:
    // Émis par n'importe quel écran pour demander une navigation.
    void navigateTo(ScreenId target);
};

#endif // ABSTRACTSCREEN_H
