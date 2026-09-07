#ifndef ELEMENT_H
#define ELEMENT_H

#include <QString>
#include <QVector>
#include <qvariant.h>

/**
 * @brief Les 5 éléments du jeu de bataille, inspirés du Wu Xing chinois.
 */
enum class Element {
    Metal = 0,
    Eau = 1,
    Bois = 2,
    Feu = 3,
    Terre = 4
};

/**
 * @brief Résultat de la confrontation entre deux cartes.
 */
enum class RoundOutcome {
    PlayerWins,
    OpponentWins,
    Draw
};

// Liste des 5 éléments, dans un ordre fixe et déterministe (utile pour
// générer une main complète ou itérer sur tous les éléments).
QVector<Element> allElements();

// Nom affichable d'un élément (ex: pour les labels UI ou les tests).
QString elementName(Element element);

/**
 * @brief Applique le cycle de contrôle : Métal -> Bois -> Terre -> Eau -> Feu -> Métal.
 *        Chaque élément de ce cycle bat le suivant.
 * @return true si "a" bat "b" selon le cycle de contrôle.
 */
bool controls(Element a, Element b);

/**
 * @brief Détermine le gagnant d'un round à partir de la carte du joueur et
 *        de celle de l'adversaire, selon le cycle de contrôle.
 */
RoundOutcome resolveElements(Element playerElement, Element opponentElement);

Q_DECLARE_METATYPE(Element)
Q_DECLARE_METATYPE(RoundOutcome)

#endif // ELEMENT_H
