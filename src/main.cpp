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

  public:
          //WallPlayerMainWindow() {}
          QList<PlayerPanel*> m_playerpanels;


  protected:
      bool eventFilter(QObject *obj, QEvent *event)
      {
        if (event->type() == QEvent::KeyPress) {
          QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
          if (keyEvent->modifiers() & Qt::ControlModifier) {
            handleKeyPress(keyEvent);
            return true;
          }
        }

        return false;
      }


      void keyPressEvent(QKeyEvent *event) { handleKeyPress(event); }

      void handleKeyPress(QKeyEvent* event)
      {
        // pause/resume all
        switch (event->key()) {
          case Qt::Key_Escape:
            if (event->modifiers() & Qt::ControlModifier)
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

        WallPlayerMainWindow mainWindow;

        QGridLayout layout(&mainWindow);
        mainWindow.setLayout(&layout);

        QStringList appArgs = QApplication::arguments(); // LATER

        for (int i=0; i<4; ++i) {
          PlayerPanel* playerPanel = new PlayerPanel(&mainWindow);
          playerPanel->installEventFilter(&mainWindow);
          mainWindow.m_playerpanels.append(playerPanel); // FIXME registerPlayer
        }

        layout.addWidget(mainWindow.m_playerpanels[0], 0, 0);
        layout.addWidget(mainWindow.m_playerpanels[1], 0, 1);
        layout.addWidget(mainWindow.m_playerpanels[2], 1, 0);
        layout.addWidget(mainWindow.m_playerpanels[3], 1, 1);

        /* TODO color picker */
        QPalette palette = mainWindow.palette();
        //palette.setColor(QPalette::Window, Qt::black);
        mainWindow.setPalette(palette);

        mainWindow.show();

        return app.exec();
}


#include "main.moc"
