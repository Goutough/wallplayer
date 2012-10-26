
#ifndef MINISLIDER_H
#define MINISLIDER_H

#include <QFrame>
#include <QDebug>

#define WP_SLIDER_SMALL_HEIGHT 5

class MiniSlider : public QWidget
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
    QSize sizeHint() const { return QSize(200, WP_SLIDER_SMALL_HEIGHT); }

    void setMaxValue(int value) { Q_ASSERT(m_maxValue>0); m_maxValue = value; };
};

class VolumeSlider : public MiniSlider
{
  public:
    VolumeSlider(QWidget* parent) : MiniSlider(parent) { m_maxValue = 100; }
    QSize sizeHint() const { return QSize(150, WP_SLIDER_SMALL_HEIGHT); }

  protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif

