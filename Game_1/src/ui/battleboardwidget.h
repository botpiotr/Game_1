#ifndef BATTLEBOARDWIDGET_H
#define BATTLEBOARDWIDGET_H

#include <QWidget>
#include <QVector>

class BoardColumnWidget;

/**
 * @brief Plateau de jeu : une colonne "Défausse" suivie d'une colonne par
 *        round. Contrairement aux anciens emplacements de carte "jouée",
 *        ce plateau ne s'efface jamais entre les rounds : chaque colonne
 *        remplie reste visible pour le reste de la partie.
 */
class BattleBoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BattleBoardWidget(int totalRounds, QWidget *parent = nullptr);

    BoardColumnWidget *discardColumn() const;

    // roundNumber commence à 1.
    BoardColumnWidget *roundColumn(int roundNumber) const;

    // Vide toutes les colonnes (nouvelle partie).
    void reset();

private:
    BoardColumnWidget *m_discardColumn;
    QVector<BoardColumnWidget *> m_roundColumns; // index 0 = round 1
};

#endif // BATTLEBOARDWIDGET_H
