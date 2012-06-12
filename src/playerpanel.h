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
    void setStatus(QString& status);
    void queueFile(QString& filename);
    int playlistCount();
    void shufflePlaylist();
    QStringList& playlist();
    void startPlayer();
    QString playlistNext();

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
