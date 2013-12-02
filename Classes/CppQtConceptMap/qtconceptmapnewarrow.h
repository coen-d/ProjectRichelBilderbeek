#ifndef QTPVDBNEWARROW_H
#define QTPVDBNEWARROW_H

#include <string>
#include <vector>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <QBrush>
#include "qtarrowitem.h"
#include "qtconceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///QtConceptMapNewArrow is the arrow shown before a new one is added
struct QtConceptMapNewArrow : public QtArrowItem
{
  QtConceptMapNewArrow(
    QtConceptMapNodeItem * const from,
    const QPointF& current_to);
  QtConceptMapNewArrow(const QtConceptMapNewArrow&) = delete;
  QtConceptMapNewArrow& operator=(const QtConceptMapNewArrow&) = delete;
  ///Obtain the source node
  const QtConceptMapNodeItem * GetFrom() const { return m_from; }
        QtConceptMapNodeItem * GetFrom()       { return m_from; }

  private:
  ///Must be suppplied
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option = 0, QWidget* widget = 0);

  ///The source node
  ///Cannot be const as the user might want to edit it
  QtConceptMapNodeItem * const m_from;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTPVDBNEWARROW_H