#ifndef QTPVDBCONCEPTMAPEDITWIDGET_H
#define QTPVDBCONCEPTMAPEDITWIDGET_H


#include "qtconceptmapfwd.h"

#include "qtconceptmapwidget.h"

namespace ribi {

namespace cmap {

///QtConceptMapWidget for creation and editing of a ConceptMap
struct QtConceptMapEditWidget : public QtConceptMapWidget
{
  typedef QtConceptMapEditWidget This_t;
  QtConceptMapEditWidget(
    const boost::shared_ptr<ribi::cmap::ConceptMap> concept_map,
    QWidget* parent = 0);
  ~QtConceptMapEditWidget() noexcept;
  QtConceptMapEditWidget(const QtConceptMapEditWidget& other) = delete;
  QtConceptMapEditWidget& operator=(const QtConceptMapEditWidget& other) = delete;

  #ifndef NDEBUG
  ///Creates a new derived class
  ///A simpler alternative to Clone (see above)
  std::unique_ptr<QtConceptMapWidget> CreateNewDerived() const;
  #endif

  ///Delete an EdgeConcept
  void DeleteEdge(QtConceptMapEdgeItem * const edge);

  ///Delete a Node
  void DeleteNode(QtConceptMapNodeItem * const node);

  #ifndef NDEBUG
  ///Do something random
  void DoRandomStuff();
  #endif

  ///Signal emitted when a concept map item requests to be edited
  boost::signals2::signal<void(QtConceptMapItem*)> m_signal_conceptmapitem_requests_edit;

protected:

  void keyPressEvent(QKeyEvent* event) noexcept;
  void mouseMoveEvent(QMouseEvent * event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);

private:

  ///The arrow used to create a new arrow
  ///Is nullptr when not active
  QtConceptMapNewArrow * m_arrow;

  ///The item highlighter, used when creating a new relation
  QtConceptMapItemHighlighter * const m_highlighter;

  ///The item showing the tools
  QtConceptMapToolsItem * m_tools;

  ///Adds an Edge and connects (some of) its signals to slots present in the derived classes
  void AddEdge(const boost::shared_ptr<ribi::cmap::Edge> edge);

  ///Add a new edge
  void AddEdge(QtConceptMapNodeItem * const from, QtConceptMapNodeItem* const to);

  ///Adds a node and connects (some of) its signals to slots present in the derived classes
  QtConceptMapNodeItem * AddNode(const boost::shared_ptr<ribi::cmap::Node> node);

  ///Remove all Qt and non-Qt items
  void CleanMe();

  ///Obtain the read-and-write Qt edge items
  ///The read-only Qt edge items is already supplied by QtConceptMapWidget
  const std::vector<QtConceptMapEdgeItem *> GetQtEdges();

  ///Obtain the read-and-write Qt node items
  ///The read-only Qt node items is already supplied by QtConceptMapWidget
  const std::vector<QtConceptMapNodeItem *> GetQtNodes();

  ///Called when an item wants to be edited
  void OnConceptMapItemRequestsEdit(QtConceptMapItem * const item);

  ///Called whenever a concept is clicked or moved
  ///If item is nullptr, the last item might be deleted
  ///Use QGraphicsItem* due to QtKeyboardFriendlyGraphicsView working on QGraphicsItems
  ///Is implemented by OnItemUpdateRequestImpl
  void OnItemRequestUpdateImpl(const QGraphicsItem* const item);

  ///Called whenever the tools item is clicked
  void OnToolsClicked();

  ///Test this class
  static void Test();
};

} //~namespace cmap

} //~namespace ribi

#endif // QTPVDBCONCEPTMAPEDITWIDGET_H