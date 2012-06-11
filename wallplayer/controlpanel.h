
#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "playerpanel.h"
#include "player.h"

class PlayerPanel;

class ControlPanel : public QWidget
{
  Q_OBJECT

  public:
    ControlPanel(QWidget* parent, PlayerPanel* playerPanel);

    QSize sizeHint () const { return QSize(200,-1); }

    void setStatus(QString& status) {
      m_statusLabel.setText(status);
    }

  protected slots:
    void launchAddFileDialog();
    void shufflePlaylist();
    void startPlaying ();

  protected:
    QVBoxLayout m_layout;

    QLabel m_statusLabel;
    QPushButton* m_shuffleButton;
    QPushButton* m_playButton;
    PlayerPanel* m_playerPanel;
};

#endif
