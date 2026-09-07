#include "playerhanddock.h"
#include "cardwidget.h"

#include <QHBoxLayout>

PlayerHandDock::PlayerHandDock(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QHBoxLayout(this);
    m_layout->setAlignment(Qt::AlignCenter);
}

void PlayerHandDock::setHandElements(const QVector<Element> &elements)
{
    // Repart de zéro à chaque mise à jour : plus simple et robuste que de
    // essayer de faire correspondre les anciens widgets aux nouveaux éléments.
    for (CardWidget *widget : std::as_const(m_cardWidgets)) {
        m_layout->removeWidget(widget);
        widget->deleteLater();
    }
    m_cardWidgets.clear();

    for (Element element : elements) {
        auto *card = new CardWidget(this);
        card->setElement(element);
        card->setEnabled(m_interactive);

        connect(card, &CardWidget::clicked, this, [this, card](Element clickedElement) {
            emit cardSelected(clickedElement, card);
        });

        m_layout->addWidget(card);
        m_cardWidgets.append(card);
    }
}

void PlayerHandDock::setInteractive(bool interactive)
{
    m_interactive = interactive;
    for (CardWidget *widget : std::as_const(m_cardWidgets)) {
        widget->setEnabled(interactive);
    }
}
