#include "playerpanel.h"
#include "player.h"

void PlayerPanel::stateChanged (int state) {
  qDebug() << "state changed to" << state;

  if (state == QMPwidget::StoppedState) {
    m_player->hide();
    m_controlpanel->show();
  }
}

PlayerPanel::PlayerPanel(QWidget* parent)
  : m_playlistPosition(0)
{
  setParent(parent);

  m_controlpanel = new ControlPanel(this, this);

  m_layout.addWidget(m_controlpanel);

  setLayout(&m_layout);
}

Player* PlayerPanel::player() { return m_player; }

void PlayerPanel::setStatus(QString& status) { m_controlpanel->setStatus(status); }

void PlayerPanel::queueFile(QString& filename) { m_playlist.append(filename); }

int PlayerPanel::playlistCount() { return m_playlist.size(); }

void PlayerPanel::shufflePlaylist() {
  std::random_shuffle(m_playlist.begin(), m_playlist.end());
}

QStringList& PlayerPanel::playlist() {
  return m_playlist;
}

QString PlayerPanel::playlistNext() {
  Q_ASSERT_X(!m_playlist.isEmpty(),"PlayerPanel","playlistNext");

  if (m_playlistPosition>=playlistCount())
    m_playlistPosition = 0;

  return playlist()[m_playlistPosition++];
}

void PlayerPanel::startPlayer()
{
      m_controlpanel->hide();

      QStringList playerArgs;

      playerArgs.append("-vo");
#ifdef Q_OS_WIN
      if (QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA)
        playerArgs.append("direct3d,");
      else
        playerArgs.append("directx,");
#else
      playerArgs.append("gl2,gl,xv,x11");
#endif
      playerArgs.append("-zoom"); // e.g. vo=x11
      playerArgs.append("-idx");
      playerArgs.append("-softvol");

      playerArgs.append("-nosound"); // DEBUG

      m_player = new Player(playerArgs, playlistNext(), this);
      connect(m_player, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
      m_layout.addWidget(m_player);

      return;
}

