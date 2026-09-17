#include "cardbattlescreen.h"
#include "../ui/cardwidget.h"
#include "../ui/cardstackwidget.h"
#include "../ui/playerhanddock.h"
#include "../ui/roundtrackerwidget.h"
#include "../ui/battleboardwidget.h"
#include "../ui/boardcolumnwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QTimer>
#include <QPropertyAnimation>
#include <QEasingCurve>

namespace {
// Durée minimale d'affichage du résultat d'un round avant d'enchaîner.
const int kResultDisplayDelayMs = 1000;
// Durée du vol d'une carte de son point de départ à son emplacement sur le plateau.
const int kCardFlightDurationMs = 450;
}

CardBattleScreen::CardBattleScreen(QWidget *parent)
    : AbstractScreen(parent)
{
    m_engine = new BattleEngine(this);
    connect(m_engine, &BattleEngine::initialDiscardResolved, this, &CardBattleScreen::onInitialDiscardResolved);
    connect(m_engine, &BattleEngine::roundResolved, this, &CardBattleScreen::onRoundResolved);

    // --- En-tête : titre, round en cours, scores ---
    auto *title = new QLabel("Bataille élémentaire");
    QFont titleFont = title->font();
    titleFont.setPointSize(titleFont.pointSize() + 4);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);

    m_roundLabel = new QLabel();
    m_roundLabel->setAlignment(Qt::AlignCenter);

    m_scoreLabel = new QLabel();
    m_scoreLabel->setAlignment(Qt::AlignCenter);

    // --- Tas de cartes restantes de l'adversaire ---
    m_opponentStack = new CardStackWidget();
    auto *opponentStackLayout = new QHBoxLayout();
    opponentStackLayout->addStretch();
    opponentStackLayout->addWidget(m_opponentStack);
    opponentStackLayout->addStretch();

    // --- Plateau : défausse + 3 rounds, reste affiché en permanence ---
    m_board = new BattleBoardWidget(BattleEngine::TotalRounds);
    auto *boardLayout = new QHBoxLayout();
    boardLayout->addStretch();
    boardLayout->addWidget(m_board);
    boardLayout->addStretch();

    // --- Résultat du dernier round joué ---
    m_resultLabel = new QLabel();
    m_resultLabel->setAlignment(Qt::AlignCenter);
    QFont resultFont = m_resultLabel->font();
    resultFont.setItalic(true);
    resultFont.setBold(true);
    m_resultLabel->setFont(resultFont);

    // --- Bouton pour relancer une partie une fois celle-ci terminée ---
    m_restartButton = new QPushButton("Rejouer");
    m_restartButton->setVisible(false);
    connect(m_restartButton, &QPushButton::clicked, this, &CardBattleScreen::resetForNewGame);

    // --- Tableau de bord : main du joueur ---
    m_handDock = new PlayerHandDock();
    connect(m_handDock, &PlayerHandDock::cardSelected, this, &CardBattleScreen::onHandCardClicked);

    auto *backButton = new QPushButton("Retour au menu");
    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit navigateTo(ScreenId::MainMenu);
    });

    auto *battleLayout = new QVBoxLayout();
    battleLayout->addWidget(title);
    battleLayout->addWidget(m_roundLabel);
    battleLayout->addWidget(m_scoreLabel);
    battleLayout->addLayout(opponentStackLayout);
    battleLayout->addLayout(boardLayout);
    battleLayout->addWidget(m_resultLabel);
    battleLayout->addWidget(m_restartButton, 0, Qt::AlignCenter);
    battleLayout->addStretch();
    battleLayout->addWidget(m_handDock);
    battleLayout->addWidget(backButton);

    // --- Suivi des rounds, affiché sur le côté ---
    m_roundTracker = new RoundTrackerWidget(BattleEngine::TotalRounds);

    auto *outerLayout = new QHBoxLayout(this);
    outerLayout->addLayout(battleLayout, /*stretch=*/1);
    outerLayout->addWidget(m_roundTracker);
}

void CardBattleScreen::onEnter()
{
    resetForNewGame();
}

void CardBattleScreen::resetForNewGame()
{
    m_engine->startNewGame();
    m_resultLabel->clear();
    m_board->reset();
    m_roundTracker->reset();
    m_restartButton->setVisible(false);
    m_activeColumn = nullptr;
    m_pendingAnimations = 0;
    m_actionInProgressIsDiscard = false;
    refreshHandAndStack();
    updateScoreLabels();
    m_handDock->setInteractive(true);
}

void CardBattleScreen::onHandCardClicked(Element element, QWidget *sourceCardWidget)
{
    if (!sourceCardWidget) {
        return;
    }

    m_handDock->setInteractive(false);

    m_actionInProgressIsDiscard = !m_engine->isDiscardDone();
    m_activeColumn = m_actionInProgressIsDiscard
                         ? m_board->discardColumn()
                         : m_board->roundColumn(m_engine->currentRound());

    const QRect startRect = widgetRectRelativeToScreen(sourceCardWidget);
    const QRect playerEndRect = widgetRectRelativeToScreen(m_activeColumn->playerCardWidget());

    m_pendingAnimations = 2; // carte du joueur + carte de l'adversaire

    // Le joueur voit toujours sa propre carte : jamais face cachée.
    animateCardMovement(startRect, playerEndRect, element, /*startFaceDown=*/false, /*endFaceDown=*/false,
                         [this, element]() {
                             m_activeColumn->playerCardWidget()->setElement(element);
                             m_activeColumn->playerCardWidget()->setFaceDown(false);
                             onCardAnimationFinished();
                         });

    // Déclenche la logique de jeu ; le signal correspondant (émis de façon
    // synchrone) nous donne la carte choisie par l'adversaire.
    if (m_actionInProgressIsDiscard) {
        m_engine->discardInitialCard(element);
    } else {
        m_engine->playPlayerCard(element);
    }
}

void CardBattleScreen::onInitialDiscardResolved(Element playerCard, Element opponentCard)
{
    Q_UNUSED(playerCard);
    // L'écart se fait face visible dès le départ, pas de secret à révéler.
    animateOpponentCardToColumn(m_activeColumn, opponentCard, /*startFaceDown=*/false);
}

void CardBattleScreen::onRoundResolved(int roundNumber, Element playerCard, Element opponentCard, RoundOutcome outcome)
{
    QString resultText;
    switch (outcome) {
    case RoundOutcome::PlayerWins:
        resultText = QString("Round %1 : vous gagnez ! (%2 bat %3)")
                          .arg(roundNumber)
                          .arg(elementName(playerCard), elementName(opponentCard));
        break;
    case RoundOutcome::OpponentWins:
        resultText = QString("Round %1 : l'adversaire gagne ! (%2 bat %3)")
                          .arg(roundNumber)
                          .arg(elementName(opponentCard), elementName(playerCard));
        break;
    case RoundOutcome::Draw:
        resultText = QString("Round %1 : égalité.").arg(roundNumber);
        break;
    }
    m_resultLabel->setText(resultText);
    m_roundTracker->setRoundResult(roundNumber, outcome);
    updateScoreLabels();

    // La carte de l'adversaire était secrète jusqu'ici : révélée à l'arrivée.
    animateOpponentCardToColumn(m_activeColumn, opponentCard, /*startFaceDown=*/true);
}

void CardBattleScreen::animateOpponentCardToColumn(BoardColumnWidget *column, Element opponentElement, bool startFaceDown)
{
    const QRect startRect = widgetRectRelativeToScreen(m_opponentStack);
    const QRect endRect = widgetRectRelativeToScreen(column->opponentCardWidget());

    animateCardMovement(startRect, endRect, opponentElement, startFaceDown, /*endFaceDown=*/false,
                         [this, column, opponentElement]() {
                             column->opponentCardWidget()->setElement(opponentElement);
                             column->opponentCardWidget()->setFaceDown(false);
                             onCardAnimationFinished();
                         });
}

void CardBattleScreen::animateCardMovement(const QRect &startRect, const QRect &endRect, Element element,
                                            bool startFaceDown, bool endFaceDown, std::function<void()> onFinished)
{
    auto *flyingCard = new CardWidget(this);
    flyingCard->setElement(element);
    flyingCard->setFaceDown(startFaceDown);
    flyingCard->setGeometry(startRect);
    flyingCard->show();
    flyingCard->raise();

    auto *animation = new QPropertyAnimation(flyingCard, "geometry", flyingCard);
    animation->setDuration(kCardFlightDurationMs);
    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    animation->setEasingCurve(QEasingCurve::InOutQuad);

    connect(animation, &QPropertyAnimation::finished, this, [flyingCard, endFaceDown, onFinished]() {
        flyingCard->setFaceDown(endFaceDown);
        // Laisse entrevoir la carte retournée un court instant avant de
        // nettoyer le widget volant et de révéler la vraie carte du plateau.
        QTimer::singleShot(150, flyingCard, [flyingCard, onFinished]() {
            flyingCard->deleteLater();
            if (onFinished) {
                onFinished();
            }
        });
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

QRect CardBattleScreen::widgetRectRelativeToScreen(QWidget *widget) const
{
    const QPoint topLeftGlobal = widget->mapToGlobal(QPoint(0, 0));
    const QPoint topLeftLocal = const_cast<CardBattleScreen *>(this)->mapFromGlobal(topLeftGlobal);
    return QRect(topLeftLocal, widget->size());
}

void CardBattleScreen::onCardAnimationFinished()
{
    --m_pendingAnimations;
    if (m_pendingAnimations > 0) {
        return; // on attend que les deux cartes (joueur + adversaire) soient arrivées
    }

    if (m_actionInProgressIsDiscard) {
        // L'écart n'a pas de vainqueur à afficher : on enchaîne directement
        // sur le premier round.
        refreshHandAndStack();
        updateScoreLabels();
        m_handDock->setInteractive(true);
        return;
    }

    // Laisse le résultat affiché au moins 1 seconde avant d'enchaîner, que
    // ce soit sur le round suivant ou sur l'affichage de fin de partie.
    QTimer::singleShot(kResultDisplayDelayMs, this, [this]() {
        if (m_engine->isGameOver()) {
            const int playerScore = m_engine->playerScore();
            const int opponentScore = m_engine->opponentScore();

            QString finalMessage;
            if (playerScore > opponentScore) {
                finalMessage = QString("Partie terminée : vous gagnez %1 à %2 !")
                                   .arg(playerScore)
                                   .arg(opponentScore);
            } else if (opponentScore > playerScore) {
                finalMessage = QString("Partie terminée : l'adversaire gagne %1 à %2.")
                                   .arg(opponentScore)
                                   .arg(playerScore);
            } else {
                finalMessage = QString("Partie terminée : égalité %1 à %2.")
                                   .arg(playerScore)
                                   .arg(opponentScore);
            }
            m_resultLabel->setText(finalMessage);
            m_restartButton->setVisible(true);
        } else {
            refreshHandAndStack();
            m_resultLabel->clear();
            m_handDock->setInteractive(true);
            updateScoreLabels();
        }
    });
}

void CardBattleScreen::refreshHandAndStack()
{
    m_handDock->setHandElements(m_engine->playerHand().remainingElements());
    m_opponentStack->setCount(m_engine->opponentHand().remainingElements().size());
}

void CardBattleScreen::updateScoreLabels()
{
    const int round = qMin(m_engine->currentRound(), BattleEngine::TotalRounds);
    m_roundLabel->setText(QString("Round %1 / %2").arg(round).arg(BattleEngine::TotalRounds));
    m_scoreLabel->setText(QString("Vous : %1  —  Adversaire : %2")
                               .arg(m_engine->playerScore())
                               .arg(m_engine->opponentScore()));
}
