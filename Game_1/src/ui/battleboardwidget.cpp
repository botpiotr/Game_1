#include "battleboardwidget.h"
#include "boardcolumnwidget.h"

#include <QHBoxLayout>

BattleBoardWidget::BattleBoardWidget(int totalRounds, QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);

    m_discardColumn = new BoardColumnWidget("Défausse");
    layout->addWidget(m_discardColumn);

    layout->addSpacing(16);

    for (int round = 1; round <= totalRounds; ++round) {
        auto *column = new BoardColumnWidget(QString("Round %1").arg(round));
        layout->addWidget(column);
        m_roundColumns.append(column);
    }
}

BoardColumnWidget *BattleBoardWidget::discardColumn() const
{
    return m_discardColumn;
}

BoardColumnWidget *BattleBoardWidget::roundColumn(int roundNumber) const
{
    const int index = roundNumber - 1;
    if (index < 0 || index >= m_roundColumns.size()) {
        return nullptr;
    }
    return m_roundColumns.at(index);
}

void BattleBoardWidget::reset()
{
    m_discardColumn->reset();
    for (BoardColumnWidget *column : std::as_const(m_roundColumns)) {
        column->reset();
    }
}
