#ifndef CARDVISUALS_H
#define CARDVISUALS_H

#include <QHash>
#include <QPixmap>
#include <QString>

#include "element.h"

/**
 * @brief Fournit le visuel (image) à afficher pour chaque carte.
 *
 * Par défaut, chaque élément est représenté par un rectangle coloré généré
 * à la volée (une couleur par élément). N'importe quel élément peut se voir
 * associer une image personnalisée via setImagePath() ; à partir de ce
 * moment, pixmapFor() renverra cette image (mise à l'échelle) plutôt que
 * le rectangle par défaut. Idem pour le dos des cartes (setBackImagePath).
 */
class CardVisuals
{
public:
    static CardVisuals &instance();

    // Associe une image personnalisée à un élément. Passer une chaîne vide
    // revient à retirer la personnalisation (retour au visuel par défaut).
    void setImagePath(Element element, const QString &path);
    QString imagePath(Element element) const;

    // Image personnalisée pour le dos des cartes (face cachée).
    void setBackImagePath(const QString &path);
    QString backImagePath() const;

    // Pixmap prêt à afficher pour un élément donné, à la taille demandée.
    QPixmap pixmapFor(Element element, const QSize &size) const;

    // Pixmap du dos de carte (face cachée), à la taille demandée.
    QPixmap backPixmap(const QSize &size) const;

    // Pixmap pour un emplacement vide (aucune carte jouée à cet endroit).
    QPixmap emptySlotPixmap(const QSize &size) const;

private:
    CardVisuals() = default;

    QPixmap placeholderFor(Element element, const QSize &size) const;
    QPixmap defaultBackPixmap(const QSize &size) const;

    QHash<Element, QString> m_imagePaths;
    QString m_backImagePath;
};

#endif // CARDVISUALS_H
