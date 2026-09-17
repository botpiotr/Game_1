#ifndef CARDBATTLESCREEN_H
#define CARDBATTLESCREEN_H

#include "abstractscreen.h"
#include "../game/battleengine.h"

#include <QRect>
#include <functional>

class QLabel;
class QPushButton;
class CardStackWidget;
class PlayerHandDock;
class RoundTrackerWidget;
class BattleBoardWidget;
class BoardColumnWidget;

/**
 * @brief Écran de la bataille de cartes élémentaire (joueur contre IA).
 *
 * Déroulement affiché : chaque joueur écarte d'abord une carte face
 * visible (colonne "Défausse" du plateau), puis 3 rounds sont joués face
 * cachée et révélés un par un. Le plateau garde toutes les cartes jouées
 * visibles pour le reste de la partie. Chaque carte jouée est animée
 * depuis son point de départ (main du joueur, ou tas de l'adversaire)
 * jusqu'à son emplacement sur le plateau.
 */
class CardBattleScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit CardBattleScreen(QWidget *parent = nullptr);

    // Démarre une nouvelle partie à chaque arrivée sur cet écran.
    void onEnter() override;

private:
    void onHandCardClicked(Element element, QWidget *sourceCardWidget);
    void onInitialDiscardResolved(Element playerCard, Element opponentCard);
    void onRoundResolved(int roundNumber, Element playerCard, Element opponentCard, RoundOutcome outcome);

    void resetForNewGame();
    void refreshHandAndStack();
    void updateScoreLabels();

    // Anime le déplacement de la carte de l'adversaire depuis son tas
    // jusqu'à la colonne donnée. startFaceDown=true simule un tirage
    // secret révélé à l'arrivée (rounds) ; false l'affiche visible dès
    // le départ (écart initial, "face visible").
    void animateOpponentCardToColumn(BoardColumnWidget *column, Element opponentElement, bool startFaceDown);

    // Anime un déplacement générique de carte entre deux positions
    // (exprimées en coordonnées relatives à cet écran).
    void animateCardMovement(const QRect &startRect, const QRect &endRect, Element element,
                              bool startFaceDown, bool endFaceDown, std::function<void()> onFinished);

    // Convertit la géométrie d'un widget quelconque en coordonnées
    // relatives à cet écran, pour pouvoir positionner la carte volante.
    QRect widgetRectRelativeToScreen(QWidget *widget) const;

    // Appelé à la fin de chaque animation ; une fois que les deux cartes
    // (joueur + adversaire) sont arrivées, enchaîne sur la suite du jeu.
    void onCardAnimationFinished();

    BattleEngine *m_engine;

    QLabel *m_roundLabel;
    QLabel *m_scoreLabel;
    QLabel *m_resultLabel;

    CardStackWidget *m_opponentStack;
    BattleBoardWidget *m_board;
    PlayerHandDock *m_handDock;
    RoundTrackerWidget *m_roundTracker;
    QPushButton *m_restartButton;

    BoardColumnWidget *m_activeColumn = nullptr;
    int m_pendingAnimations = 0;
    bool m_actionInProgressIsDiscard = false;
};

#endif // CARDBATTLESCREEN_H
