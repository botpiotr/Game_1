#include "gamesettings.h"

#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

GameSettings &GameSettings::instance()
{
    static GameSettings singleton;
    return singleton;
}

GameSettings::GameSettings()
    : m_volume(80)
    , m_fullscreen(false)
    , m_saveDirectory(defaultSaveDirectory())
{
    load();
}

QString GameSettings::defaultSaveDirectory()
{
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
           + "/Game_1/Saves";
}

QString GameSettings::settingsFilePath()
{
    const QString configDir =
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    return configDir + "/settings.json";
}

void GameSettings::reload()
{
    load();
    emit volumeChanged(m_volume);
    emit fullscreenChanged(m_fullscreen);
    emit saveDirectoryChanged(m_saveDirectory);
}

int GameSettings::volume() const
{
    return m_volume;
}

void GameSettings::setVolume(int volume)
{
    const int clamped = qBound(0, volume, 100);
    if (m_volume == clamped) {
        return;
    }
    m_volume = clamped;
    emit volumeChanged(m_volume);
    save();
}

bool GameSettings::fullscreen() const
{
    return m_fullscreen;
}

void GameSettings::setFullscreen(bool fullscreen)
{
    if (m_fullscreen == fullscreen) {
        return;
    }
    m_fullscreen = fullscreen;
    emit fullscreenChanged(m_fullscreen);
    save();
}

QString GameSettings::saveDirectory() const
{
    return m_saveDirectory;
}

void GameSettings::setSaveDirectory(const QString &path)
{
    if (m_saveDirectory == path) {
        return;
    }
    m_saveDirectory = path;
    emit saveDirectoryChanged(m_saveDirectory);
    save();
}

void GameSettings::load()
{
    QFile file(settingsFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        // Pas de fichier existant (premier lancement) : on garde les
        // valeurs par défaut définies dans le constructeur.
        return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        return;
    }

    const QJsonObject obj = doc.object();
    if (obj.contains("volume")) {
        m_volume = qBound(0, obj.value("volume").toInt(m_volume), 100);
    }
    if (obj.contains("fullscreen")) {
        m_fullscreen = obj.value("fullscreen").toBool(m_fullscreen);
    }
    if (obj.contains("saveDirectory")) {
        m_saveDirectory = obj.value("saveDirectory").toString(m_saveDirectory);
    }
}

void GameSettings::save() const
{
    QJsonObject obj;
    obj["volume"] = m_volume;
    obj["fullscreen"] = m_fullscreen;
    obj["saveDirectory"] = m_saveDirectory;

    const QString path = settingsFilePath();

    // S'assure que le dossier de config existe avant d'y écrire.
    QDir().mkpath(QFileInfo(path).absolutePath());

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return;
    }
    file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    file.close();
}
