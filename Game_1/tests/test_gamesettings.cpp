#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include "settings/gamesettings.h"

class TestGameSettings : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void volumeIsClampedToValidRange();
    void fullscreenTogglesAndEmitsSignal();
    void saveDirectoryChangesAndEmitsSignal();
    void settingsArePersistedToDisk();
    void settingsAreReloadedFromDisk();

private:
    QString m_settingsPath;
};

void TestGameSettings::initTestCase()
{
    // Redirige les emplacements standards vers un dossier temporaire :
    // les tests ne touchent jamais les vrais réglages de l'utilisateur.
    QStandardPaths::setTestModeEnabled(true);
    m_settingsPath = GameSettings::settingsFilePath();
}

void TestGameSettings::init()
{
    // Repart d'un fichier propre avant chaque test.
    QFile::remove(m_settingsPath);
}

void TestGameSettings::volumeIsClampedToValidRange()
{
    GameSettings::instance().setVolume(150);
    QCOMPARE(GameSettings::instance().volume(), 100);

    GameSettings::instance().setVolume(-20);
    QCOMPARE(GameSettings::instance().volume(), 0);

    GameSettings::instance().setVolume(42);
    QCOMPARE(GameSettings::instance().volume(), 42);
}

void TestGameSettings::fullscreenTogglesAndEmitsSignal()
{
    GameSettings::instance().setFullscreen(false);

    QSignalSpy spy(&GameSettings::instance(), &GameSettings::fullscreenChanged);
    GameSettings::instance().setFullscreen(true);

    QCOMPARE(GameSettings::instance().fullscreen(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
}

void TestGameSettings::saveDirectoryChangesAndEmitsSignal()
{
    QSignalSpy spy(&GameSettings::instance(), &GameSettings::saveDirectoryChanged);

    const QString newPath = "/tmp/game1_test_saves";
    GameSettings::instance().setSaveDirectory(newPath);

    QCOMPARE(GameSettings::instance().saveDirectory(), newPath);
    QCOMPARE(spy.count(), 1);
}

void TestGameSettings::settingsArePersistedToDisk()
{
    GameSettings::instance().setVolume(37);
    GameSettings::instance().setFullscreen(true);
    GameSettings::instance().setSaveDirectory("/tmp/persisted_saves");

    // Lit le fichier directement, sans passer par GameSettings, pour
    // vérifier que save() a bien écrit sur disque à chaque appel de setter.
    QFile file(m_settingsPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    const QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();

    QCOMPARE(obj.value("volume").toInt(), 37);
    QCOMPARE(obj.value("fullscreen").toBool(), true);
    QCOMPARE(obj.value("saveDirectory").toString(), QString("/tmp/persisted_saves"));
}

void TestGameSettings::settingsAreReloadedFromDisk()
{
    // Écrit un fichier JSON "à la main", simulant un fichier déjà présent
    // au démarrage, puis vérifie que GameSettings le charge correctement.
    QJsonObject obj;
    obj["volume"] = 55;
    obj["fullscreen"] = true;
    obj["saveDirectory"] = "/tmp/reloaded_saves";

    QDir().mkpath(QFileInfo(m_settingsPath).absolutePath());
    QFile file(m_settingsPath);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
    file.write(QJsonDocument(obj).toJson());
    file.close();

    GameSettings::instance().reload();

    QCOMPARE(GameSettings::instance().volume(), 55);
    QCOMPARE(GameSettings::instance().fullscreen(), true);
    QCOMPARE(GameSettings::instance().saveDirectory(), QString("/tmp/reloaded_saves"));
}

QTEST_MAIN(TestGameSettings)
#include "test_gamesettings.moc"
