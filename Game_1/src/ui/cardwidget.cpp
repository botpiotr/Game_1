#include "cardwidget.h"
#include "../game/cardvisuals.h"

#include <QMouseEvent>

namespace {
const QSize kCardSize(90, 130);
}

CardWidget::CardWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(kCardSize);
    setCursor(Qt::PointingHandCursor);
    setEmpty();
}

void CardWidget::setElement(Element element)
{
    m_element = element;
    m_empty = false;
    refreshPixmap();
}

void CardWidget::setFaceDown(bool faceDown)
{
    m_faceDown = faceDown;
    refreshPixmap();
}

void CardWidget::setEmpty()
{
    m_empty = true;
    m_faceDown = false;
    setPixmap(CardVisuals::instance().emptySlotPixmap(kCardSize));
    setCursor(Qt::ArrowCursor);
}

Element CardWidget::element() const
{
    return m_element;
}

bool CardWidget::isEmpty() const
{
    return m_empty;
}

void CardWidget::refreshPixmap()
{
    if (m_empty) {
        setPixmap(CardVisuals::instance().emptySlotPixmap(kCardSize));
        setCursor(Qt::ArrowCursor);
        return;
    }

    if (m_faceDown) {
        setPixmap(CardVisuals::instance().backPixmap(kCardSize));
    } else {
        setPixmap(CardVisuals::instance().pixmapFor(m_element, kCardSize));
    }
    setCursor(Qt::PointingHandCursor);
}

void CardWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_empty && !m_faceDown) {
        emit clicked(m_element);
    }
    QLabel::mousePressEvent(event);
}
