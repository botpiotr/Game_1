#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QLabel>

#include "../game/element.h"

/**
 * @brief Représente visuellement une carte : soit un élément (face visible),
 *        soit une face cachée, soit un emplacement vide (aucune carte).
 *        Cliquable lorsqu'elle affiche un élément en face visible.
 */
class CardWidget : public QLabel
{
    Q_OBJECT

public:
    explicit CardWidget(QWidget *parent = nullptr);

    // Affiche l'élément donné, face visible.
    void setElement(Element element);

    // Bascule l'affichage entre face cachée (dos de carte) et face visible.
    // N'a d'effet que si un élément a été défini via setElement().
    void setFaceDown(bool faceDown);

    // Vide l'emplacement (aucune carte affichée, contour en pointillés).
    void setEmpty();

    Element element() const;
    bool isEmpty() const;

signals:
    // Émis au clic, uniquement si la carte est visible et non vide.
    void clicked(Element element);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void refreshPixmap();

    Element m_element = Element::Eau;
    bool m_faceDown = false;
    bool m_empty = true;
};

#endif // CARDWIDGET_H
