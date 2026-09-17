#include <QtTest/QtTest>
#include <QSignalSpy>

#include "../src/game/element.h"
#include "../src/game/battleengine.h"

class TestBattleEngine : public QObject
{
    Q_OBJECT

private slots:
    // --- Cycle de contrôle (règle de combo) ---
    void controlCycleMatchesExpectedRules();
    void sameElementIsADraw();

    // --- Écart initial ---
    void handStartsWithAllFiveElements();
    void discardRemovesOneCardFromHand();
    void playingBeforeDiscardIsIgnored();

    // --- Déroulement des rounds ---
    void playingACardRemovesItFromHand();
    void gameEndsAfterThreeRounds();
    void winnerIsPlayerWithMostRoundsWon();
};

void TestBattleEngine::controlCycleMatchesExpectedRules()
{
    // Cycle de contrôle : Métal -> Bois -> Terre -> Eau -> Feu -> Métal
    QCOMPARE(resolveElements(Element::Metal, Element::Bois), RoundOutcome::PlayerWins);
    QCOMPARE(resolveElements(Element::Bois, Element::Terre), RoundOutcome::PlayerWins);
    QCOMPARE(resolveElements(Element::Terre, Element::Eau), RoundOutcome::PlayerWins);
    QCOMPARE(resolveElements(Element::Eau, Element::Feu), RoundOutcome::PlayerWins);
    QCOMPARE(resolveElements(Element::Feu, Element::Metal), RoundOutcome::PlayerWins);

    // Exemple donné explicitement : Terre vs Bois → Bois (l'adversaire) gagne.
    QCOMPARE(resolveElements(Element::Terre, Element::Bois), RoundOutcome::OpponentWins);
}

void TestBattleEngine::sameElementIsADraw()
{
    QCOMPARE(resolveElements(Element::Feu, Element::Feu), RoundOutcome::Draw);
}

void TestBattleEngine::handStartsWithAllFiveElements()
{
    BattleEngine engine;
    engine.startNewGame();

    const QVector<Element> hand = engine.playerHand().remainingElements();
    QCOMPARE(hand.size(), 5);
    for (Element element : allElements()) {
        QVERIFY(hand.contains(element));
    }
}

void TestBattleEngine::discardRemovesOneCardFromHand()
{
    BattleEngine engine;
    engine.startNewGame();

    QSignalSpy discardSpy(&engine, &BattleEngine::initialDiscardResolved);

    engine.discardInitialCard(Element::Feu);

    QVERIFY(engine.isDiscardDone());
    QCOMPARE(engine.playerDiscardedCard(), Element::Feu);
    QVERIFY(!engine.playerHand().hasElement(Element::Feu));
    QCOMPARE(engine.playerHand().remainingElements().size(), 4);
    QCOMPARE(discardSpy.count(), 1);

    // Un deuxième écart ne doit rien changer (déjà fait).
    engine.discardInitialCard(Element::Eau);
    QCOMPARE(engine.playerDiscardedCard(), Element::Feu);
    QCOMPARE(discardSpy.count(), 1);
}

void TestBattleEngine::playingBeforeDiscardIsIgnored()
{
    BattleEngine engine;
    engine.startNewGame();

    QSignalSpy roundSpy(&engine, &BattleEngine::roundResolved);

    engine.playPlayerCard(Element::Eau);

    QVERIFY(!engine.isGameOver());
    QCOMPARE(roundSpy.count(), 0);
    QCOMPARE(engine.playerHand().remainingElements().size(), 5); // rien n'a bougé
}

void TestBattleEngine::playingACardRemovesItFromHand()
{
    BattleEngine engine;
    engine.startNewGame();
    engine.discardInitialCard(Element::Feu);

    engine.playPlayerCard(Element::Eau);

    QVERIFY(!engine.playerHand().hasElement(Element::Eau));
    QCOMPARE(engine.playerHand().remainingElements().size(), 3);
}

void TestBattleEngine::gameEndsAfterThreeRounds()
{
    BattleEngine engine;
    engine.startNewGame();
    engine.discardInitialCard(Element::Feu); // reste : Métal, Bois, Terre, Eau

    QSignalSpy roundSpy(&engine, &BattleEngine::roundResolved);
    QSignalSpy gameOverSpy(&engine, &BattleEngine::gameOver);

    QVERIFY(!engine.isGameOver());

    engine.playPlayerCard(Element::Metal);
    QVERIFY(!engine.isGameOver());

    engine.playPlayerCard(Element::Bois);
    QVERIFY(!engine.isGameOver());

    engine.playPlayerCard(Element::Terre);
    QVERIFY(engine.isGameOver());

    QCOMPARE(roundSpy.count(), BattleEngine::TotalRounds);
    QCOMPARE(gameOverSpy.count(), 1);

    // Une carte reste inutilisée (Eau) : 5 - 1 écartée - 3 jouées = 1.
    QCOMPARE(engine.playerHand().remainingElements().size(), 1);

    // Une fois la partie terminée, jouer une carte supplémentaire ne fait rien.
    engine.playPlayerCard(Element::Eau);
    QCOMPARE(roundSpy.count(), BattleEngine::TotalRounds);
}

void TestBattleEngine::winnerIsPlayerWithMostRoundsWon()
{
    BattleEngine engine;
    engine.startNewGame();
    engine.discardInitialCard(Element::Feu);

    QSignalSpy gameOverSpy(&engine, &BattleEngine::gameOver);

    // Joue les 3 rounds ; le résultat dépend de l'IA (aléatoire), donc on
    // vérifie seulement la cohérence du résultat final avec les scores.
    engine.playPlayerCard(Element::Metal);
    engine.playPlayerCard(Element::Bois);
    engine.playPlayerCard(Element::Terre);

    QCOMPARE(gameOverSpy.count(), 1);
    const auto args = gameOverSpy.takeFirst();
    const RoundOutcome finalOutcome = args.at(0).value<RoundOutcome>();
    const int playerScore = args.at(1).toInt();
    const int opponentScore = args.at(2).toInt();

    if (playerScore > opponentScore) {
        QCOMPARE(finalOutcome, RoundOutcome::PlayerWins);
    } else if (opponentScore > playerScore) {
        QCOMPARE(finalOutcome, RoundOutcome::OpponentWins);
    } else {
        QCOMPARE(finalOutcome, RoundOutcome::Draw);
    }
    QCOMPARE(playerScore, engine.playerScore());
    QCOMPARE(opponentScore, engine.opponentScore());
}

QTEST_MAIN(TestBattleEngine)
#include "test_battleengine.moc"
