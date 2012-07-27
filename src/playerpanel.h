#ifndef PLAYERPANEL_H
#define PLAYERPANEL_H

#include <QDebug>

#include "controlpanel.h"
#include "player.h"

class ControlPanel;

class PlayerPanel : public QWidget
{
  Q_OBJECT

  public:
    PlayerPanel(QWidget* parent);
    Player* player();
    void setStatus(QString status);

    int queueFiles(QStringList& filenames);
    void queueFile(QString& filename);
    void queueMediaFile(QString& filename);
    void queuePlaylistFile(QString& filename);

    int playlistCount();
    void shufflePlaylist();
    QStringList& playlist();
    QString playlistNext();

    void startPlayer();

  protected slots:
    void stateChanged(int);

  protected:
    QStringList m_playlist;
    int m_playlistPosition;
    QGridLayout m_layout;
    Player* m_player;
    ControlPanel* m_controlpanel;

  private:
    PlayerPanel();
};

#endif
