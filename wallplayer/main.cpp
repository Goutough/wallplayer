#include <QApplication>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QShowEvent>
#include <QSlider>
#include <iostream>

#include "qmpwidget.h"


class Player : public QMPwidget
{
        Q_OBJECT

        public:
                Player(const QStringList &args, const QString &url, QWidget *parent = 0)
                        : QMPwidget(parent), m_url(url)
                {
                        connect(this, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
                        QMPwidget::start(args);
                }
                
        protected:
                void keyPressEvent(QKeyEvent *event) {
                  /* Ctrl invokes global behavior */
                  if (event->modifiers() & Qt::ControlModifier) {
                    event->setAccepted(false);
                    return;
                  }

                  /* pass on everything else to parent -- for now */
                  QMPwidget::keyPressEvent(event);
                }

                void paintEvent (QPaintEvent *event) {
                  if (hasFocus()) {
                    QPainter painter(this);
                    QRect playerRect = m_widget->geometry();
                    painter.setPen(Qt::red);
                    painter.drawRect(playerRect.adjusted(-3,-3,3,3));
                  }
                }


        private slots:
                void stateChanged(int state) {
                        if (state == QMPwidget::NotStartedState)
                          QApplication::exit();
                }

        protected:
                void showEvent(QShowEvent *event) {
                  /* FIXME: wait for all players to be ready */
                  if (!event->spontaneous() && state() == QMPwidget::IdleState) {
                    QMPwidget::load(m_url);
                  }
                }

        private:
                QString m_url;
};

class PlayerPanel : public QWidget
{
  Q_OBJECT

  public:
    PlayerPanel(QStringList& playerArgs, QString& file, QWidget* parent)
      : m_positionLabel(this), m_volumeLabel(this)
    {
      setParent(parent);

      m_player = new Player(playerArgs, file, this);
      m_layout.addWidget(m_player, 0, 0);

      m_positionLabel.setText("foo");
      m_layout.addWidget(&m_positionLabel, 1, 0);

      setLayout(&m_layout);
    }

    Player* player() { return m_player; }

  protected:
    QGridLayout m_layout;
    Player* m_player;
    QLabel m_positionLabel;
    QLabel m_volumeLabel;

  private:
    PlayerPanel();
};

class WallPlayerMainWindow : public QWidget
{
        Q_OBJECT

  public:
          //WallPlayerMainWindow() {}
          QList<PlayerPanel*> m_playerpanels;


  protected:
      void keyPressEvent(QKeyEvent *event) {
        // pause/resume all
        switch (event->key()) {
          case Qt::Key_Escape:
            QApplication::exit();
            break;

          case Qt::Key_Space:
          case Qt::Key_P:
            bool atLeastOnePlaying = false;

            for (int i=0; i<m_playerpanels.size(); ++i)
              if (m_playerpanels[i]->player()->state() == QMPwidget::PlayingState)
                atLeastOnePlaying = true;

            if (atLeastOnePlaying)
              for (int i=0; i<m_playerpanels.size(); ++i)
                m_playerpanels[i]->player()->pause();
            else
              for (int i=0; i<m_playerpanels.size(); ++i)
                m_playerpanels[i]->player()->play();
            break;
        }
      }
};


int main (int argc, char **argv) {
        QApplication app(argc, argv);

        WallPlayerMainWindow widget;

        QGridLayout layout(&widget);
        widget.setLayout(&layout);

        QStringList appArgs = QApplication::arguments();
        QStringList playerArgs;
        playerArgs.append("-vo");
#ifdef Q_OS_WIN
        if (QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA)
          playerArgs.append("direct3d,");
        else
          playerArgs.append("directx,");
#else
        playerArgs.append("xv");
#endif
        playerArgs.append("-idx");
        playerArgs.append("-softvol");

        playerArgs.append("-nosound"); // DEBUG

        for (int i=0; i<4; ++i) {
          PlayerPanel* playerPanel = new PlayerPanel(playerArgs, appArgs[i+1], &widget);
          widget.m_playerpanels.append(playerPanel); // FIXME registerPlayer
        }

        layout.addWidget(widget.m_playerpanels[0], 0, 0);
        layout.addWidget(widget.m_playerpanels[1], 0, 1);
        layout.addWidget(widget.m_playerpanels[2], 1, 0);
        layout.addWidget(widget.m_playerpanels[3], 1, 1);

        /* TODO color picker */
        QPalette palette = widget.palette();
        palette.setColor(QPalette::Window, Qt::black);
        widget.setPalette(palette);

        widget.show();

        return app.exec();
}


#include "main.moc"

