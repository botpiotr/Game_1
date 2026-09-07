#ifndef ROUNDTRACKERWIDGET_H
#define ROUNDTRACKERWIDGET_H

#include <QWidget>
#include <QVector>

#include "../game/element.h"

class QLabel;

/**
 * @brief Affiche, sous forme d'une colonne de pastilles, le résultat de
 *        chaque round déjà joué (du point de vue du joueur) :
 *        ✓ vert = round gagné, ✗ rouge = round perdu, – gris = égalité.
 *        Les rounds pas encore joués restent affichés comme un cercle vide.
 */
class RoundTrackerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoundTrackerWidget(int totalRounds, QWidget *parent = nullptr);

    // Remet toutes les pastilles à l'état neutre (avant une nouvelle partie).
    void reset();

    // Renseigne le résultat d'un round (roundNumber commence à 1).
    void setRoundResult(int roundNumber, RoundOutcome outcome);

private:
    QVector<QLabel *> m_slots;
};

#endif // ROUNDTRACKERWIDGET_H
