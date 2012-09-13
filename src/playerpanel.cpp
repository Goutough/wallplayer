#include <QProcess>

#include "playerpanel.h"
#include "minislider.h"
#include "player.h"

void PlayerPanel::stateChanged (int state) {
  qDebug() << "state changed to" << state;

  if (state == QMPwidget::IdleState) {
    m_player->load(playlistNext());
  }

  if (state == QMPwidget::StoppedState) {
    m_player->hide();
    m_controlpanel->show();
  }

  if (state == QMPwidget::PlayingState && mediaInfo().ok) {
    m_streamPositionSlider->setMaxValue(mediaInfo().length);
    m_streamPositionSlider->setEnabled(mediaInfo().seekable);
  }
}

PlayerPanel::PlayerPanel(QWidget* parent)
  : m_playlistPosition(0)
{
  setParent(parent);

  QPalette palette = this->palette();
  palette.setColor(QPalette::Window, Qt::black);
  palette.setColor(QPalette::WindowText, Qt::gray);

  palette.setColor(QPalette::Text, Qt::gray);
  setPalette(palette);

  setFrameStyle(QFrame::Plain | QFrame::Box);
  setLineWidth(3);

  m_controlpanel = new ControlPanel(this, this);

  m_controlpanel->installEventFilter(this);

  m_layout.addWidget(m_controlpanel);

  setLayout(&m_layout);
}

bool PlayerPanel::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() != QEvent::FocusIn && event->type() != QEvent::FocusOut)
    return false;

  QPalette palette = this->palette();
  if (event->type() == QEvent::FocusIn)
    palette.setColor(QPalette::WindowText, Qt::yellow);
  else
    palette.setColor(QPalette::WindowText, Qt::gray);
  setPalette(palette);

  return false;
}

Player* PlayerPanel::player() { return m_player; }

void PlayerPanel::setStatus(QString status) { m_controlpanel->setStatus(status); }

int PlayerPanel::playlistCount() { return m_playlist.size(); }

void PlayerPanel::shufflePlaylist() {
  Q_ASSERT_X(m_playlist.size()>1,"PlayerPanel","shufflePlaylist");
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
      connect(m_player->process(), SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));

      m_playerstatus = new QFrame(this);
      QGridLayout* playerstatuslayout = new QGridLayout;
      playerstatuslayout->setColumnStretch(0,1);
      m_playerstatus->setLayout(playerstatuslayout);

      m_streamPositionSlider = new StreamSlider(this);
      connect(m_streamPositionSlider, SIGNAL(valueChanged(int)), m_player, SLOT(seek(int)));
      connect(m_player->process(), SIGNAL(streamPositionChanged(double)), m_streamPositionSlider, SLOT(setValue(double)));
      playerstatuslayout->addWidget(m_streamPositionSlider, 0, 0);

      m_volumeSlider = new VolumeSlider(this);
      m_player->setVolume(100);
      m_volumeSlider->setValue(100);
      connect(m_volumeSlider, SIGNAL(valueChanged(int)), m_player, SLOT(setVolume(int)));
      playerstatuslayout->addWidget(m_volumeSlider, 0, 1);

      m_layout.addWidget(m_playerstatus);

      m_layout.addWidget(m_player);

      m_player->installEventFilter(this);

      return;
}

void PlayerPanel::queueMediaFile (QString& filename)
{
  m_playlist.append(filename);

  m_controlpanel->setPlayEnabled(true);

  if (m_playlist.size()>1)
    m_controlpanel->setShuffleEnabled(true);
}

void PlayerPanel::queuePlaylistFile (QString& filename)
{
  qDebug() << "loading playlist" << filename;

  QFile playlistFile(filename);

  if (!playlistFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "couldn't open playlist file" << filename;
    return;
  }

  while (!playlistFile.atEnd()) {
    QString line(playlistFile.readLine());
    line = line.left(line.size()-1);
    //qDebug() << "loading playlist entry" << line;
    this->queueMediaFile(line);
  }
}

void PlayerPanel::queueFile (QString& filename)
{
  if (filename.contains(QRegExp("txt$")))
    this->queuePlaylistFile(filename);
  else
    this->queueMediaFile(filename);

  setStatus(QString("%2 files total in playlist now.").arg(this->playlistCount()));
}

int PlayerPanel::queueFiles (QStringList& filenames)
{
  int oldPlaylistSize = this->playlistCount();

  for (int i=0; i<filenames.size(); ++i) {
    QString filename(filenames[i]);
    this->queueFile(filename);
  }

  int newPlaylistSize = this->playlistCount();

  int nloaded = newPlaylistSize - oldPlaylistSize;

  setStatus(QString("Loaded %1 files. %2 files total in playlist now.")
      .arg(nloaded).arg(newPlaylistSize));

  return nloaded;
}

