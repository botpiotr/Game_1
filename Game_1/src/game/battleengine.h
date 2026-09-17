#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H

#include <QObject>

#include "element.h"
#include "hand.h"

/**
 * @brief Orchestre une partie de bataille de cartes élémentaire.
 *
 * Règles : chaque joueur commence avec les 5 éléments en main (un seul
 * exemplaire de chacun), aucune pioche n'a lieu.
 *
 * Déroulement :
 *  1. Écart initial : chaque joueur écarte une carte, face visible
 *     (discardInitialCard()). Il reste alors 4 cartes en main.
 *  2. 3 rounds : à chaque round, le joueur choisit une carte à jouer face
 *     cachée (playPlayerCard()), l'adversaire (IA) en fait de même, puis
 *     les deux cartes sont comparées selon le cycle de contrôle.
 *
 * Le joueur avec le plus de victoires au bout des 3 rounds remporte la
 * partie. Une carte reste inutilisée dans chaque main à la fin (5 - 1
 * écartée - 3 jouées = 1).
 */
class BattleEngine : public QObject
{
    Q_OBJECT

public:
    static constexpr int TotalRounds = 3;

    explicit BattleEngine(QObject *parent = nullptr);

    // Réinitialise complètement la partie : mains pleines, scores à zéro,
    // écart à refaire, premier round. À appeler à chaque nouvelle partie.
    void startNewGame();

    // Écarte la carte correspondant à cet élément, face visible. Déclenche
    // aussitôt l'écart de l'adversaire (IA) et émet initialDiscardResolved().
    // Ne fait rien si l'écart a déjà été fait, ou si l'élément n'est pas
    // disponible dans la main du joueur.
    void discardInitialCard(Element element);

    bool isDiscardDone() const;
    Element playerDiscardedCard() const;
    Element opponentDiscardedCard() const;

    // Le joueur joue la carte correspondant à cet élément. Déclenche
    // automatiquement le choix de l'adversaire, résout le round, et émet
    // roundResolved() puis, si la partie est terminée, gameOver().
    // Ne fait rien si l'écart initial n'a pas encore été fait, si la
    // partie est déjà terminée, ou si l'élément n'est plus disponible
    // dans la main du joueur.
    void playPlayerCard(Element element);

    const Hand &playerHand() const;
    const Hand &opponentHand() const;

    int playerScore() const;
    int opponentScore() const;

    // Round en cours, entre 1 et TotalRounds.
    int currentRound() const;

    bool isGameOver() const;

signals:
    void gameStarted();

    // Émis dès que les deux joueurs ont écarté leur carte initiale.
    void initialDiscardResolved(Element playerCard, Element opponentCard);

    // Émis après résolution de chaque round.
    void roundResolved(int roundNumber, Element playerCard, Element opponentCard, RoundOutcome outcome);

    // Émis une fois les TotalRounds joués. finalOutcome indique le
    // vainqueur global (PlayerWins/OpponentWins/Draw en cas d'égalité de score).
    void gameOver(RoundOutcome finalOutcome, int playerScore, int opponentScore);

private:
    Element pickRandomElement(const Hand &hand) const;

    Hand m_playerHand;
    Hand m_opponentHand;
    int m_playerScore = 0;
    int m_opponentScore = 0;
    int m_roundsPlayed = 0;

    bool m_discardDone = false;
    Element m_playerDiscardedCard = Element::Eau;   // valide seulement si m_discardDone
    Element m_opponentDiscardedCard = Element::Eau; // valide seulement si m_discardDone
};

#endif // BATTLEENGINE_H
