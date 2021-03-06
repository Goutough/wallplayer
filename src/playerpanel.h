#ifndef PLAYERPANEL_H
#define PLAYERPANEL_H

#include <QDebug>

#include "controlpanel.h"
#include "minislider.h"
#include "player.h"

class ControlPanel;

class PlayerPanel : public QFrame
{
  Q_OBJECT

  public:
    PlayerPanel(QWidget* parent);
    Player* player();
    void setStatus(QString status);
    bool eventFilter(QObject *obj, QEvent *event);

    int queueFiles(QStringList& filenames);
    void queueFile(QString& filename);
    void queueMediaFile(QString& filename);
    void queuePlaylistFile(QString& filename);

    int playlistCount();
    void shufflePlaylist();
    QStringList& playlist();
    QString playlistNext();

    void startPlayer();

    QMPwidget::MediaInfo mediaInfo() { return m_player->mediaInfo(); }

  protected slots:
    void stateChanged(int);

  protected:
    QStringList m_playlist;
    int m_playlistPosition;
    QGridLayout m_layout;
    Player* m_player;
    ControlPanel* m_controlpanel;
    QWidget* m_playerstatus;
    StreamSlider* m_streamPositionSlider;
    VolumeSlider* m_volumeSlider;

  private:
    PlayerPanel();
};

#endif
