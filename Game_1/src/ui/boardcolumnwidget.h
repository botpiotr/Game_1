#ifndef BOARDCOLUMNWIDGET_H
#define BOARDCOLUMNWIDGET_H

#include <QWidget>

class CardWidget;

/**
 * @brief Une colonne du plateau de jeu : un titre (ex: "Défausse",
 *        "Round 1"), la carte de l'adversaire au-dessus, celle du joueur
 *        en dessous. Une fois remplie, la colonne reste affichée telle
 *        quelle pour le reste de la partie (le plateau ne s'efface jamais).
 */
class BoardColumnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoardColumnWidget(const QString &title, QWidget *parent = nullptr);

    CardWidget *opponentCardWidget() const;
    CardWidget *playerCardWidget() const;

    // Vide les deux emplacements (utilisé au début d'une nouvelle partie).
    void reset();

private:
    CardWidget *m_opponentCard;
    CardWidget *m_playerCard;
};

#endif // BOARDCOLUMNWIDGET_H
