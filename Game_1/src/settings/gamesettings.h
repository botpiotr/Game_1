#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <QObject>
#include <QString>

/**
 * @brief Réglages du jeu, persistés automatiquement dans un fichier JSON.
 *
 * Singleton accessible via GameSettings::instance(). Chaque setter met à
 * jour la valeur en mémoire, émet le signal correspondant, puis sauvegarde
 * immédiatement l'ensemble des réglages sur disque — aucune action
 * explicite de sauvegarde n'est nécessaire ailleurs dans le code.
 */
class GameSettings : public QObject
{
    Q_OBJECT

public:
    static GameSettings &instance();

    // Non copiable : un seul point d'accès aux réglages dans toute l'appli.
    GameSettings(const GameSettings &) = delete;
    GameSettings &operator=(const GameSettings &) = delete;

    int volume() const;
    void setVolume(int volume); // 0-100

    bool fullscreen() const;
    void setFullscreen(bool fullscreen);

    QString saveDirectory() const;
    void setSaveDirectory(const QString &path);

    // Chemin absolu du fichier settings.json (utile pour debug/diagnostic)
    static QString settingsFilePath();

    // Relit le fichier settings.json et émet les signaux correspondants.
    // Utile pour les tests, ou si le fichier a pu être modifié en dehors
    // de l'application.
    void reload();

signals:
    void volumeChanged(int volume);
    void fullscreenChanged(bool fullscreen);
    void saveDirectoryChanged(const QString &path);

private:
    GameSettings();

    void load();
    void save() const;
    static QString defaultSaveDirectory();

    int m_volume;
    bool m_fullscreen;
    QString m_saveDirectory;
};

#endif // GAMESETTINGS_H
