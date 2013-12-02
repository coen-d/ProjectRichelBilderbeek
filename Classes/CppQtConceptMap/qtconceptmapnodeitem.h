#ifndef QTPVDBNODEITEM_H
#define QTPVDBNODEITEM_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "qtroundedtextrectitem.h"
#include "qtconceptmapitem.h"

#include "conceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {

namespace cmap {

///QtConceptMapNodeItem displays a Node
struct QtConceptMapNodeItem : public QtConceptMapItem
{
  virtual ~QtConceptMapNodeItem() noexcept {}

  ///Node cannot be const as it contains a Concept that the user might want to edit
  ///concept_item contains the Stategy to display (and respond to the concept)
  explicit QtConceptMapNodeItem(
    const boost::shared_ptr<ribi::cmap::Node> node,
    const boost::shared_ptr<QtConceptItem> concept_item);

  virtual QRectF boundingRect() const;

  QBrush brush() const;

  void DisableAll();
  void EnableAll();

  const boost::shared_ptr<const ribi::cmap::Concept>  GetConcept() const;
  const boost::shared_ptr<      ribi::cmap::Concept>  GetConcept()      ;

  const boost::shared_ptr<const QtConceptItem> GetConceptItem() const final { return m_concept_item; }
  const boost::shared_ptr<      QtConceptItem> GetConceptItem()       final { return m_concept_item; }

  const boost::shared_ptr<const cmap::Node>  GetNode() const { return m_node; }
  const boost::shared_ptr<      cmap::Node>& GetNode()       { return m_node; }

  void SetConcept(const boost::shared_ptr<ribi::cmap::Concept> concept);

  void SetName(const std::string& name);

  ///Set the X coordinat of the central concept
  void SetX(const double x);

  ///Set the Y coordinat of the central concept
  void SetY(const double y);

  ///m_signal_request_rate_node is emitted due to a m_signal_request_rate_node
  ///of the Node its QtRateConceptItem
  boost::signals2::signal<void (QtConceptMapNodeItem *)> m_signal_node_requests_rate_concept;
  boost::signals2::signal<void (QtConceptMapNodeItem *)> m_signal_node_requests_rate_examples;

  ///No other signals, these are present in the ConceptItems

protected:

  void focusInEvent(QFocusEvent *event) final;
  void focusOutEvent(QFocusEvent *event) final;
  void keyPressEvent(QKeyEvent *event) final;

  //const boost::shared_ptr<QtConceptMapItem>& GetConceptItem() { return m_concept_item; }

  ///Next to QtConceptMapConcept::paint, check for collision
  void paint(QPainter* painter, const QStyleOptionGraphicsItem *, QWidget *) final;


private:

  ///The Strategy for displaying a Concept
  const boost::shared_ptr<QtConceptItem> m_concept_item;

  const QPen m_contour_pen;
  const QPen m_focus_pen;

  ///The node being edited, or displayed and not changed, or rated
  const boost::shared_ptr<ribi::cmap::Node> m_node;

  void OnItemHasUpdated();

  ///The m_node has changed
  void OnNodeChanged(const cmap::Node * node);
  void OnRequestsSceneUpdate();

  ///The item
  void OnItemRequestsRateConcept();
  void OnItemRequestsRateExamples();

  #ifndef NDEBUG
  static void Test();
  #endif
};

} //~namespace cmap

} //~namespace ribi

#endif // QTPVDBNODEITEM_H