
#ifndef MINISLIDER_H
#define MINISLIDER_H

#include <QFrame>
#include <QDebug>

class MiniSlider : public QFrame
{
  Q_OBJECT

  public:
    MiniSlider (QWidget* parent);

  protected:
    int m_value;
    int m_maxValue;

    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    QSize minimumSizeHint() const;

  public slots:
    void setValue(int);
    void setValue(double);

  signals:
    void valueChanged(int value);
};

class StreamSlider : public MiniSlider
{
  public:
    StreamSlider(QWidget* parent) : MiniSlider(parent) {}
    QSize sizeHint() const { return QSize(200, 15); }

    void setMaxValue(int value) { Q_ASSERT(m_maxValue>0); m_maxValue = value; };
};

class VolumeSlider : public MiniSlider
{
  public:
    VolumeSlider(QWidget* parent) : MiniSlider(parent) { m_maxValue = 100; }
    QSize sizeHint() const { return QSize(50, 15); }
};

#endif

