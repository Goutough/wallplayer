#ifndef PLAYER_H
#define PLAYER_H

#include <QApplication>
#include <QDebug>
#include <QShowEvent>

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

                /*
                void paintEvent (QPaintEvent *event) {
                  if (hasFocus()) {
                    QPainter painter(this);
                    QRect playerRect = m_widget->geometry();
                    painter.setPen(Qt::red);
                    painter.drawRect(playerRect.adjusted(-3,-3,3,3));
                  }
                }
                */


        private slots:
                void stateChanged(int state) {
                  /*
                  if (state == QMPwidget::NotStartedState)
                    QApplication::exit();
                  */
                }

        protected:
                void showEvent(QShowEvent *event) {
                  /* FIXME: wait for all players to be ready */
                  qDebug() << "show; state=" << state() << ", sptn=" << event->spontaneous();
                  if (!event->spontaneous() && state() == QMPwidget::IdleState) {
                    QMPwidget::load(m_url);
                  }
                }

        private:
                QString m_url;
};

#endif

