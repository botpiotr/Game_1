#include <QtTest/QtTest>

// Ce premier test est volontairement trivial : il sert surtout à valider
// que la chaîne de test (CMake + QTest + CI) fonctionne bien de bout en bout.
// Remplace-le/complète-le au fur et à mesure que tu ajoutes de la logique
// (règles du jeu, classes de plateau, etc.) à tester.

class TestBasic : public QObject
{
    Q_OBJECT

private slots:
    void additionFonctionne();
};

void TestBasic::additionFonctionne()
{
    QCOMPARE(1 + 1, 2);
}

QTEST_MAIN(TestBasic)
#include "test_basic.moc"
