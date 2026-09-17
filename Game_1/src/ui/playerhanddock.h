#ifndef PLAYERHANDDOCK_H
#define PLAYERHANDDOCK_H

#include <QWidget>
#include <QVector>

#include "../game/element.h"

class QHBoxLayout;
class CardWidget;

/**
 * @brief Affiche la main du joueur sous forme d'une rangée de cartes
 *        cliquables. Se reconstruit entièrement à chaque appel de
 *        setHandElements() (typiquement après chaque round, une fois
 *        la carte jouée retirée de la main).
 */
class PlayerHandDock : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerHandDock(QWidget *parent = nullptr);

    void setHandElements(const QVector<Element> &elements);

    // Désactive temporairement le clic sur les cartes (ex: pendant qu'un
    // round est en cours de résolution/animation).
    void setInteractive(bool interactive);

signals:
    // cardWidget est le widget cliqué dans la main : sert de point de
    // départ pour l'animation de déplacement vers le plateau.
    void cardSelected(Element element, QWidget *cardWidget);

private:
    QHBoxLayout *m_layout;
    QVector<CardWidget *> m_cardWidgets;
    bool m_interactive = true;
};

#endif // PLAYERHANDDOCK_H
