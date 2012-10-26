#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QRegExp>
#include <QGridLayout>

#include "controlpanel.h"
#include "playerpanel.h"

class WallPlayerMainWindow : public QWidget
{
  Q_OBJECT

  private:
      // modifier for shortcuts affecting the application, not individual panels
      const static int globalModifier = Qt::ControlModifier;
      // background vids are being lowered in volume by this factor when an
      // individual player is running fullscreen
      const static float fullscreenVolumeRatio = 0.5;

  public:
      //WallPlayerMainWindow() {}
      QList<PlayerPanel*> m_playerpanels;

  protected:
      void keyPressEvent(QKeyEvent *event) { eventFilter(this, event); }

      bool eventFilter(QObject *obj, QEvent *event)
      {
        if (event->type() != QEvent::KeyPress)
          return false;

        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        switch (keyEvent->key())
        {
          /* focus change 1-4 */
          case Qt::Key_1:
            m_playerpanels[0]->player()->setFocus(Qt::ShortcutFocusReason);
            break;

          case Qt::Key_2:
            m_playerpanels[1]->player()->setFocus(Qt::ShortcutFocusReason);
            break;

          case Qt::Key_3:
            m_playerpanels[2]->player()->setFocus(Qt::ShortcutFocusReason);
            break;

          case Qt::Key_4:
            m_playerpanels[3]->player()->setFocus(Qt::ShortcutFocusReason);
            break;

          /* fullscreen toggle */
          case Qt::Key_F:
            //if (keyEvent->modifiers() & globalModifier)
            {
              if (isFullScreen()) {
                showNormal();
              } else {
                showFullScreen();
              }
            }
            return true;
            break;

          /* pause toggle */
          case Qt::Key_Space:
          case Qt::Key_P:
            {
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
            }
            return true;
            break;

          /* quit */
          case Qt::Key_Escape:
            //if (keyEvent->modifiers() & globalModifier)
            QApplication::exit();
            return true;
            break;
        }

        return false;
      }
};

int main (int argc, char **argv) {
        QApplication app(argc, argv);

        WallPlayerMainWindow mainWindow;

        QGridLayout layout(&mainWindow);
        layout.setSpacing(0);
        layout.setMargin(0);
        mainWindow.setLayout(&layout);

        QStringList appArgs = QApplication::arguments();

        for (int i=0; i<4; ++i) {
          PlayerPanel* playerPanel = new PlayerPanel(&mainWindow);
          playerPanel->installEventFilter(&mainWindow);

          if (i<appArgs.size()) {
            qDebug() << "Queuing file from command line: " << appArgs[i+1];
            playerPanel->queueFile(appArgs[i+1]);
            playerPanel->startPlayer();
          }

          mainWindow.m_playerpanels.append(playerPanel); // FIXME registerPlayer
        }

        layout.addWidget(mainWindow.m_playerpanels[0], 0, 0);
        layout.addWidget(mainWindow.m_playerpanels[1], 0, 1);
        layout.addWidget(mainWindow.m_playerpanels[2], 1, 0);
        layout.addWidget(mainWindow.m_playerpanels[3], 1, 1);

        /* TODO color picker */
        QPalette palette = mainWindow.palette();
        palette.setColor(QPalette::Window, Qt::black);
        mainWindow.setPalette(palette);

        mainWindow.show();

        return app.exec();
}


#include "main.moc"

