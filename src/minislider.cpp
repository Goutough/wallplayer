#include <QMouseEvent>
#include <QPainter>
#include <QSizePolicy>

#include "minislider.h"

MiniSlider::MiniSlider (QWidget* parent)
  : QWidget(parent), m_value(0), m_maxValue(1)
{
  setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));
}

void MiniSlider::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  
  QPainter painter(this);

  int sliderPos = width()*((float)m_value/m_maxValue);

  /* background */
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);
  painter.drawRect(0,0,width(),height()); 

  /* bar */
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::darkGray);
  painter.drawRect(0,0,sliderPos,height()); 

  /* border */
  /*
  QPen borderPen;
  borderPen.setWidth(2);
  painter.setPen(borderPen);
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(1,1,width()-1,height()-1); 
  */
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


void VolumeSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
      setValue(((float)event->x()/width())*m_maxValue);

    MiniSlider::mousePressEvent(event);
}

void VolumeSlider::mouseMoveEvent(QMouseEvent *event)
{
    setValue(((float)event->x()/width())*m_maxValue);
    MiniSlider::mouseMoveEvent(event);
}
