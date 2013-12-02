#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapitemhighlighter.h"

#include <boost/math/constants/constants.hpp>

#include <QGraphicsItem>
#include <QTimer>
//#include "qtconceptmapconceptitem.h"
#include "qtconceptmapnodeitem.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::cmap::QtConceptMapItemHighlighter::QtConceptMapItemHighlighter(QObject *parent)
  : QObject(parent),
    m_cnt(0),
    m_item(nullptr),
    m_timer(new QTimer(this))
{
  m_timer->setInterval(10); //ms
  QObject::connect(
    m_timer,SIGNAL(timeout()),
    this,SLOT(OnTimer()));
}

void ribi::cmap::QtConceptMapItemHighlighter::SetItem(QtConceptMapNodeItem* const item)
{
  if (m_item == item) return;

  if (item)
  {
    this->m_timer->start();
  }
  else
  {
    ///Set the current item in its normal rotation
    if (m_item) m_item->setRotation(0.0);
    this->m_timer->stop();
  }
  m_item = item;
}


void ribi::cmap::QtConceptMapItemHighlighter::OnTimer()
{
  if (m_item)
  {
    ++m_cnt;
    #define USE_ROTATION_ANIMATION_675982734653425297529
    #ifdef USE_MOVING_POSITION_ANIMATION_723497235973450873659360843975308
    QPointF pos = m_item->pos();
    switch (m_cnt % 4)
    {
      case 0: pos.setX(pos.x() + 2.0); break;
      case 1: pos.setY(pos.y() + 2.0); break;
      case 2: pos.setX(pos.x() - 2.0); break;
      case 3: pos.setY(pos.y() - 2.0); break;
      default: assert(!"Should not get here");
    }
    m_item->setPos(pos);
    #endif
    #ifdef USE_ROTATION_ANIMATION_675982734653425297529
    const double pi = boost::math::constants::pi<double>();
    const double angle = 2.0 * pi * static_cast<double>(m_cnt) / 360.0;
    const double speed_up = 5.0;
    const double r = std::sin(angle * speed_up) * 5.0;
    m_item->setRotation(r);
    #endif
    m_item->update();
  }
}