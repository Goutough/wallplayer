#include <QMouseEvent>
#include <QPainter>
#include <QSizePolicy>

#include "minislider.h"

MiniSlider::MiniSlider (QWidget* parent)
  : QFrame(parent), m_value(0), m_maxValue(1)
{
  setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));
  QPalette palette = this->palette();
  palette.setColor(QPalette::WindowText, Qt::darkGray);
  setPalette(palette);

  setFrameStyle(QFrame::Plain | QFrame::Box);
  setLineWidth(1);
}

void MiniSlider::paintEvent(QPaintEvent* event)
{
  QFrame::paintEvent(event);
  
  QPainter painter(this);

  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::darkBlue);
  qDebug() << "w: " << width() << ", v:" << m_value << ", maxv:"<<m_maxValue;
  int sliderPos = width()*((float)m_value/m_maxValue);
  qDebug() << "sliderPos: " << sliderPos;
  painter.drawRect(0,0,sliderPos,30); 
}

QSize MiniSlider::minimumSizeHint() const
{
  return QSize(20, 5);
}

void MiniSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
      emit valueChanged(((float)event->x()/width())*m_maxValue);
}

void MiniSlider::mouseMoveEvent(QMouseEvent *event)
{
      emit valueChanged(((float)event->x()/width())*m_maxValue);
}

void MiniSlider::setValue (double value)
{
  setValue((int)value);
}

void MiniSlider::setValue (int value)
{
  Q_ASSERT(value>=0);
  Q_ASSERT(value<=m_maxValue);

  m_value = value;
  update();
}
