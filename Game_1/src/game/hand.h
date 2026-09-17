#ifndef HAND_H
#define HAND_H

#include "element.h"

/**
 * @brief Main d'un joueur. Contient toujours, au départ, exactement un
 *        exemplaire de chacun des 5 éléments. On ne pioche jamais de
 *        nouvelle carte : la main ne fait que se vider au fil des rounds.
 */
class Hand
{
public:
    Hand();

    // Remet la main à son état initial : les 5 éléments, tous disponibles.
    void reset();

    // Éléments encore disponibles (non joués) dans cette main.
    QVector<Element> remainingElements() const;

    bool hasElement(Element element) const;

    // Retire l'élément de la main (il vient d'être joué). Ne fait rien si
    // l'élément n'est pas/plus disponible.
    void playElement(Element element);

private:
    QVector<Element> m_remaining;
};

#endif // HAND_H
