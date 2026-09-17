#include "battleengine.h"

#include <QRandomGenerator>

BattleEngine::BattleEngine(QObject *parent)
    : QObject(parent)
{
}

void BattleEngine::startNewGame()
{
    m_playerHand.reset();
    m_opponentHand.reset();
    m_playerScore = 0;
    m_opponentScore = 0;
    m_roundsPlayed = 0;
    m_discardDone = false;

    emit gameStarted();
}

void BattleEngine::discardInitialCard(Element element)
{
    if (m_discardDone) {
        return;
    }
    if (!m_playerHand.hasElement(element)) {
        return;
    }

    m_playerDiscardedCard = element;
    m_playerHand.playElement(element);

    m_opponentDiscardedCard = pickRandomElement(m_opponentHand);
    m_opponentHand.playElement(m_opponentDiscardedCard);

    m_discardDone = true;

    emit initialDiscardResolved(m_playerDiscardedCard, m_opponentDiscardedCard);
}

bool BattleEngine::isDiscardDone() const
{
    return m_discardDone;
}

Element BattleEngine::playerDiscardedCard() const
{
    return m_playerDiscardedCard;
}

Element BattleEngine::opponentDiscardedCard() const
{
    return m_opponentDiscardedCard;
}

void BattleEngine::playPlayerCard(Element element)
{
    if (!m_discardDone) {
        return; // il faut d'abord écarter une carte
    }
    if (isGameOver()) {
        return;
    }
    if (!m_playerHand.hasElement(element)) {
        return;
    }

    const Element opponentElement = pickRandomElement(m_opponentHand);
    const RoundOutcome outcome = resolveElements(element, opponentElement);

    m_playerHand.playElement(element);
    m_opponentHand.playElement(opponentElement);

    if (outcome == RoundOutcome::PlayerWins) {
        ++m_playerScore;
    } else if (outcome == RoundOutcome::OpponentWins) {
        ++m_opponentScore;
    }

    ++m_roundsPlayed;

    emit roundResolved(m_roundsPlayed, element, opponentElement, outcome);

    if (isGameOver()) {
        RoundOutcome finalOutcome;
        if (m_playerScore > m_opponentScore) {
            finalOutcome = RoundOutcome::PlayerWins;
        } else if (m_opponentScore > m_playerScore) {
            finalOutcome = RoundOutcome::OpponentWins;
        } else {
            finalOutcome = RoundOutcome::Draw;
        }
        emit gameOver(finalOutcome, m_playerScore, m_opponentScore);
    }
}

Element BattleEngine::pickRandomElement(const Hand &hand) const
{
    // IA volontairement basique : choix aléatoire parmi les cartes
    // restantes. Point d'extension facile pour une IA plus élaborée plus tard.
    const QVector<Element> remaining = hand.remainingElements();
    const int index = QRandomGenerator::global()->bounded(remaining.size());
    return remaining.at(index);
}

const Hand &BattleEngine::playerHand() const
{
    return m_playerHand;
}

const Hand &BattleEngine::opponentHand() const
{
    return m_opponentHand;
}

int BattleEngine::playerScore() const
{
    return m_playerScore;
}

int BattleEngine::opponentScore() const
{
    return m_opponentScore;
}

int BattleEngine::currentRound() const
{
    return qMin(m_roundsPlayed + 1, TotalRounds);
}

bool BattleEngine::isGameOver() const
{
    return m_roundsPlayed >= TotalRounds;
}
