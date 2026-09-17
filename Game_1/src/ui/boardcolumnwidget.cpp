#include "boardcolumnwidget.h"
#include "cardwidget.h"

#include <QVBoxLayout>
#include <QLabel>

BoardColumnWidget::BoardColumnWidget(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    auto *titleLabel = new QLabel(title);
    titleLabel->setAlignment(Qt::AlignCenter);

    m_opponentCard = new CardWidget();
    m_opponentCard->setEmpty();

    m_playerCard = new CardWidget();
    m_playerCard->setEmpty();

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(m_opponentCard);
    layout->addSpacing(8);
    layout->addWidget(m_playerCard);
}

CardWidget *BoardColumnWidget::opponentCardWidget() const
{
    return m_opponentCard;
}

CardWidget *BoardColumnWidget::playerCardWidget() const
{
    return m_playerCard;
}

void BoardColumnWidget::reset()
{
    m_opponentCard->setEmpty();
    m_playerCard->setEmpty();
}
