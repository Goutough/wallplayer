#include "controlpanel.h"
#include "playerpanel.h"


ControlPanel::ControlPanel(QWidget* parent, PlayerPanel* playerPanel)
{
  setParent(parent);

  m_playerPanel = playerPanel;

  m_statusLabel.setText("Ready.");
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
  int nloaded = 0;
  QStringList filenames = QFileDialog::getOpenFileNames(this, "Select one or more files to open");

  for (int i=0; i<filenames.size(); ++i) {
    QString filename(filenames[i]);
    if (filename.contains(QRegExp("txt$"))) {
      qDebug() << "loading playlist" << filename;

      QFile playlistFile(filename);

      if (!playlistFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "couldn't open playlist file" << filename;
        continue;
      }

      while (!playlistFile.atEnd()) {
        QString line(playlistFile.readLine());
        line = line.left(line.size()-1);
        qDebug() << "loading playlist entry" << line;
        ++nloaded;
        m_playerPanel->queueFile(line);
      }
    } else {
      qDebug() << "loading file" << filename;
      ++nloaded;
      m_playerPanel->queueFile(filename);
    }
  }

  if (nloaded) {
    m_shuffleButton->setEnabled(true);
    m_playButton->setEnabled(true);
  }

  m_statusLabel.setText(QString("Loaded %1 files. %2 files total in playlist now.")
      .arg(nloaded).arg(m_playerPanel->playlistCount()));

  return;
}

void ControlPanel::shufflePlaylist()
{
  m_playerPanel->shufflePlaylist();
  m_statusLabel.setText(QString("Shuffled %1 files.").arg(m_playerPanel->playlistCount()));
}

void ControlPanel::startPlaying()
{
  qDebug() << "starting play of" << m_playerPanel->playlist()[0];

  m_statusLabel.setText("Initializing player...");

  m_playerPanel->startPlayer();

  return;
}
