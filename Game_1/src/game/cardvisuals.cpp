#include "cardvisuals.h"

#include <QPainter>
#include <QColor>

namespace {

QColor colorForElement(Element element)
{
    // Couleurs traditionnellement associées à chaque élément du Wu Xing.
    switch (element) {
    case Element::Eau:   return QColor("#1565C0"); // bleu
    case Element::Bois:  return QColor("#2E7D32"); // vert
    case Element::Terre: return QColor("#8D6E63"); // brun
    case Element::Feu:   return QColor("#C62828"); // rouge
    case Element::Metal: return QColor("#B0BEC5"); // gris argenté
    }
    return QColor(Qt::gray);
}

QPixmap drawRoundedCard(const QSize &size, const QColor &fillColor, const QString &text)
{
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF rect(1, 1, size.width() - 2, size.height() - 2);
    const qreal radius = 10.0;

    painter.setBrush(fillColor);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRoundedRect(rect, radius, radius);

    if (!text.isEmpty()) {
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(size.height() / 10);
        painter.setFont(font);
        painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, text);
    }

    return pixmap;
}

} // namespace

CardVisuals &CardVisuals::instance()
{
    static CardVisuals singleton;
    return singleton;
}

void CardVisuals::setImagePath(Element element, const QString &path)
{
    if (path.isEmpty()) {
        m_imagePaths.remove(element);
    } else {
        m_imagePaths.insert(element, path);
    }
}

QString CardVisuals::imagePath(Element element) const
{
    return m_imagePaths.value(element);
}

void CardVisuals::setBackImagePath(const QString &path)
{
    m_backImagePath = path;
}

QString CardVisuals::backImagePath() const
{
    return m_backImagePath;
}

QPixmap CardVisuals::pixmapFor(Element element, const QSize &size) const
{
    const QString customPath = m_imagePaths.value(element);
    if (!customPath.isEmpty()) {
        QPixmap custom(customPath);
        if (!custom.isNull()) {
            return custom.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        // Si le fichier est introuvable/invalide, on retombe sur le placeholder
        // plutôt que d'afficher une image vide.
    }
    return placeholderFor(element, size);
}

QPixmap CardVisuals::backPixmap(const QSize &size) const
{
    if (!m_backImagePath.isEmpty()) {
        QPixmap custom(m_backImagePath);
        if (!custom.isNull()) {
            return custom.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
    }
    return defaultBackPixmap(size);
}

QPixmap CardVisuals::emptySlotPixmap(const QSize &size) const
{
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen dashedPen(QColor(150, 150, 150));
    dashedPen.setStyle(Qt::DashLine);
    dashedPen.setWidth(2);
    painter.setPen(dashedPen);
    painter.setBrush(Qt::NoBrush);

    const QRectF rect(1, 1, size.width() - 2, size.height() - 2);
    painter.drawRoundedRect(rect, 10.0, 10.0);

    return pixmap;
}

QPixmap CardVisuals::placeholderFor(Element element, const QSize &size) const
{
    return drawRoundedCard(size, colorForElement(element), elementName(element));
}

QPixmap CardVisuals::defaultBackPixmap(const QSize &size) const
{
    return drawRoundedCard(size, QColor("#37474F"), "?");
}
