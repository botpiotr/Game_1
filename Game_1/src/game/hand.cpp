#include "hand.h"

Hand::Hand()
{
    reset();
}

void Hand::reset()
{
    m_remaining = allElements();
}

QVector<Element> Hand::remainingElements() const
{
    return m_remaining;
}

bool Hand::hasElement(Element element) const
{
    return m_remaining.contains(element);
}

void Hand::playElement(Element element)
{
    m_remaining.removeOne(element);
}
