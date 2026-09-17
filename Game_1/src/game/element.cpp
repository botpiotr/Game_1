#include "element.h"

namespace {

// Ordre du cycle de contrôle : chaque élément bat le suivant, en boucle.
// Métal bat Bois, Bois bat Terre, Terre bat Eau, Eau bat Feu, Feu bat Métal.
// un deuxième cycle doit être mis en place :
// Feu bat Bois, Bois bat Eau, Eau bat Métal, Métal bat Terre, Terre bat Feu.
const QVector<Element> kControlCycle = {
    Element::Metal,
    Element::Eau,
    Element::Bois,
    Element::Feu,
    Element::Terre
};

} // namespace

QVector<Element> allElements()
{
    return kControlCycle;
}

QString elementName(Element element)
{
    switch (element) {
    case Element::Eau:   return "Eau";
    case Element::Bois:  return "Bois";
    case Element::Terre: return "Terre";
    case Element::Feu:   return "Feu";
    case Element::Metal: return "Métal";
    }
    return QString();
}

bool controls(Element a, Element b)
{
    const int indexA = kControlCycle.indexOf(a);
    const int indexB = kControlCycle.indexOf(b);

    bool b_cycle1 = (indexB == (indexA + 2) % kControlCycle.size());
    bool b_cycle2 = (indexB == (indexA + 4) % kControlCycle.size());

    return b_cycle1 || b_cycle2;
}

RoundOutcome resolveElements(Element playerElement, Element opponentElement)
{
    if (playerElement == opponentElement) {
        return RoundOutcome::Draw;
    }
    if (controls(playerElement, opponentElement)) {
        return RoundOutcome::PlayerWins;
    }
    if (controls(opponentElement, playerElement)) {
        return RoundOutcome::OpponentWins;
    }
    return RoundOutcome::Draw; // ne devrait jamais arriver avec un cycle complet
}
