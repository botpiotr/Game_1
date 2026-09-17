#include "roundtrackerwidget.h"

#include <QVBoxLayout>
#include <QLabel>

namespace {
const int kSlotSize = 32;

QString neutralStyle()
{
    return "QLabel { border: 2px solid #90A4AE; border-radius: 16px; "
           "color: #90A4AE; background: transparent; }";
}

QString winStyle()
{
    return "QLabel { border: 2px solid #2E7D32; border-radius: 16px; "
           "color: #2E7D32; background: #E8F5E9; font-weight: bold; }";
}

QString lossStyle()
{
    return "QLabel { border: 2px solid #C62828; border-radius: 16px; "
           "color: #C62828; background: #FFEBEE; font-weight: bold; }";
}

QString drawStyle()
{
    return "QLabel { border: 2px solid #757575; border-radius: 16px; "
           "color: #757575; background: #F5F5F5; font-weight: bold; }";
}
}

RoundTrackerWidget::RoundTrackerWidget(int totalRounds, QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    auto *title = new QLabel("Rounds");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    for (int i = 0; i < totalRounds; ++i) {
        auto *slot = new QLabel();
        slot->setFixedSize(kSlotSize, kSlotSize);
        slot->setAlignment(Qt::AlignCenter);
        slot->setStyleSheet(neutralStyle());
        layout->addWidget(slot);
        m_slots.append(slot);
    }

    layout->addStretch();
}

void RoundTrackerWidget::reset()
{
    for (QLabel *slot : std::as_const(m_slots)) {
        slot->clear();
        slot->setStyleSheet(neutralStyle());
    }
}

void RoundTrackerWidget::setRoundResult(int roundNumber, RoundOutcome outcome)
{
    const int index = roundNumber - 1;
    if (index < 0 || index >= m_slots.size()) {
        return;
    }

    QLabel *slot = m_slots.at(index);
    switch (outcome) {
    case RoundOutcome::PlayerWins:
        slot->setText("✓");
        slot->setStyleSheet(winStyle());
        break;
    case RoundOutcome::OpponentWins:
        slot->setText("✗");
        slot->setStyleSheet(lossStyle());
        break;
    case RoundOutcome::Draw:
        slot->setText("–");
        slot->setStyleSheet(drawStyle());
        break;
    }
}
