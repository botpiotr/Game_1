#ifndef CARDSTACKWIDGET_H
#define CARDSTACKWIDGET_H

#include <QWidget>

/**
 * @brief Visuel d'un tas de cartes empilées, faces cachées, avec un
 *        compteur indiquant combien de cartes restent dans le tas.
 *        Utilise CardVisuals::backPixmap() pour le dos des cartes, donc
 *        personnalisable via CardVisuals::setBackImagePath().
 */
class CardStackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CardStackWidget(QWidget *parent = nullptr);

    void setCount(int count);
    int count() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_count = 0;
};

#endif // CARDSTACKWIDGET_H
