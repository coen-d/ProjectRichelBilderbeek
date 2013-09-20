#ifndef QTPVDBCONCEPTMAPITEM_H
#define QTPVDBCONCEPTMAPITEM_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/signals2.hpp>
#include "pvdbfwd.h"
#include "qtroundededitrectitem.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace pvdb {

///Either a Node or Edge of a ConceptMap
///QtRoundedTextRectItem: single line
///QtRoundedEditRectItem: multiple lines
struct QtPvdbConceptMapItem : public QtRoundedEditRectItem //NEW 2013-09-15
{
  virtual ~QtPvdbConceptMapItem() noexcept {}

  virtual void DisableAll() = 0;
  virtual void EnableAll() = 0;

  ///Obtain the Concept from either a Node or an Edge
  virtual const boost::shared_ptr<const ribi::pvdb::Concept>  GetConcept() const = 0;
  virtual const boost::shared_ptr<      ribi::pvdb::Concept>  GetConcept()       = 0;

  virtual const boost::shared_ptr<const QtPvdbConceptItem> GetConceptItem() const = 0;
  virtual const boost::shared_ptr<      QtPvdbConceptItem> GetConceptItem()       = 0;

  ///Set the name of the concept
  virtual void SetName(const std::string& name) = 0;

  ///Set the position
  void SetPos(const double x, const double y) { SetX(x); SetY(y); }

  ///Set the X coordinat
  virtual void SetX(const double x) = 0;

  ///Set the Y coordinat
  virtual void SetY(const double y) = 0;

  ///A more specific signal: a Concept requests an edit, this is passed to
  ///OnConceptRequestsEdit, which lets this QtPvdbConceptMapItem request for an edit
  boost::signals2::signal<void(QtPvdbConceptMapItem*)> m_signal_conceptmapitem_requests_edit;

  ///Slot for a Concept its signal to be edited, all it does is add the ConceptMapItem
  ///the Concept is a member of
  void OnConceptRequestsEdit();

  protected:
  ///Remove this member function, let the client call the virual SetX and SetY member functions
  void setPos(const QPointF &pos) { QtRoundedEditRectItem::setPos(pos); }
  ///Remove this member function, let the client call the virual SetX and SetY member functions
  void setPos(qreal x, qreal y) { QtRoundedEditRectItem::setPos(x,y); }

  void hoverMoveEvent(QGraphicsSceneHoverEvent *) final;

  private:
  virtual void SetConcept(const boost::shared_ptr<ribi::pvdb::Concept> concept) = 0;

};

} //~namespace pvdb

} //~namespace ribi

#endif // QTPVDBCONCEPTMAPITEM_H
