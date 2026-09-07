#include "cardstackwidget.h"
#include "../game/cardvisuals.h"

#include <QPainter>

namespace {
const QSize kCardSize(70, 100);
const int kMaxVisibleLayers = 4; // au-delà, l'effet visuel n'ajoute rien
const int kLayerOffset = 4;      // décalage en pixels entre chaque carte de la pile
}

CardStackWidget::CardStackWidget(QWidget *parent)
    : QWidget(parent)
{
    const int extra = kLayerOffset * (kMaxVisibleLayers - 1);
    setFixedSize(kCardSize.width() + extra, kCardSize.height() + extra + 20); // +20 pour le texte du compteur
}

void CardStackWidget::setCount(int count)
{
    m_count = count;
    update();
}

int CardStackWidget::count() const
{
    return m_count;
}

void CardStackWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (m_count <= 0) {
        painter.setPen(QColor(150, 150, 150));
        painter.drawText(rect(), Qt::AlignCenter, "Vide");
        return;
    }

    const QPixmap back = CardVisuals::instance().backPixmap(kCardSize);
    const int layers = qMin(m_count, kMaxVisibleLayers);

    for (int i = 0; i < layers; ++i) {
        painter.drawPixmap(i * kLayerOffset, i * kLayerOffset, back);
    }

    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    const QRect countRect(0, height() - 18, width(), 18);
    painter.drawText(countRect, Qt::AlignCenter, QString("%1 carte(s)").arg(m_count));
}
