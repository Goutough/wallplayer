#include <QApplication>
#include <QFileDialog>
#include <QGridLayout>
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

	private slots:
		void stateChanged(int state)
		{
			if (state == QMPwidget::NotStartedState) {
				QApplication::exit();
			} else if (state == QMPwidget::PlayingState && mediaInfo().ok) {
				if (parentWidget()) {
					parentWidget()->resize(mediaInfo().size.width(), mediaInfo().size.height());
				} else {
					resize(mediaInfo().size.width(), mediaInfo().size.height());
				}
			}
		}

	protected:
		void showEvent(QShowEvent *event)
		{
			if (!event->spontaneous() && state() == QMPwidget::IdleState) {
				QMPwidget::load(m_url);
			}
		}

	private:
		QString m_url;
};


// Program entry point
int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QWidget widget;

	QGridLayout layout(&widget);
	widget.setLayout(&layout);

	QStringList appArgs = QApplication::arguments();
        QStringList playerArgs;
        playerArgs.append("-vo");
        playerArgs.append("xv");
        playerArgs.append("-nosound");
        playerArgs.append("-idx");
	Player player1(playerArgs, appArgs[1], &widget);
	Player player2(playerArgs, appArgs[2], &widget);
	layout.addWidget(&player1, 0, 0);
	layout.addWidget(&player2, 0, 1);
	widget.show();

	return app.exec();
}


#include "main.moc"
