
#include "controlpanel.h"
#include "playerpanel.h"


ControlPanel::ControlPanel(QWidget* parent, PlayerPanel* playerPanel)
{
  setParent(parent);
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

  m_playerPanel = playerPanel;

  setStatus("Ready.");
  m_layout.addWidget(&m_statusLabel);

  QPushButton* addFileButton = new QPushButton("Add files...", this);
  addFileButton->setToolTip("Add media files and playlists (*.txt)");
  connect(addFileButton, SIGNAL(clicked()), this, SLOT(launchAddFileDialog()));
  m_layout.addWidget(addFileButton);

  QPushButton* addDirectoryButton = new QPushButton("Add directory...", this);
  addDirectoryButton->setToolTip("Add a directory and all files in it and its subdirectories");
  addDirectoryButton->setEnabled(false);
  m_layout.addWidget(addDirectoryButton);

  m_shuffleButton = new QPushButton("Shuffle playlist", this);
  m_shuffleButton->setToolTip("Shuffle the current playlist");
  m_shuffleButton->setEnabled(false);
  connect(m_shuffleButton, SIGNAL(clicked()), this, SLOT(shufflePlaylist()));
  m_layout.addWidget(m_shuffleButton);

  m_playButton = new QPushButton("Play!", this);
  m_playButton->setToolTip("Start playing the current playlist");
  m_playButton->setEnabled(false);
  connect(m_playButton, SIGNAL(clicked()), this, SLOT(startPlaying()));
  m_layout.addWidget(m_playButton);

  setLayout(&m_layout);
}

void ControlPanel::launchAddFileDialog ()
{
  QStringList filenames = QFileDialog::getOpenFileNames(this, "Select one or more files to open");

  m_playerPanel->queueFiles(filenames);

  return;
}

void ControlPanel::shufflePlaylist()
{
  m_playerPanel->shufflePlaylist();
  setStatus(QString("Shuffled %1 files.").arg(m_playerPanel->playlistCount()));
}

void ControlPanel::startPlaying()
{
  qDebug() << "starting play of" << m_playerPanel->playlist()[0];

  setStatus("Initializing player...");

  m_playerPanel->startPlayer();

  return;
}

