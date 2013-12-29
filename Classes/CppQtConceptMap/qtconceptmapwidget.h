#ifndef QTCONCEPTMAPCONCEPTMAPWIDGET_H
#define QTCONCEPTMAPCONCEPTMAPWIDGET_H

#include "qtconceptmapfwd.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"

#include <boost/shared_ptr.hpp>
#include <QWidget>

#include "qtkeyboardfriendlygraphicsview.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///QtConceptMapWidget implements a higher-level interface of
///QtConceptMap, driven by Commands
///
///
struct QtConceptMapWidget : public ribi::QtKeyboardFriendlyGraphicsView
{
  QtConceptMapWidget(const boost::shared_ptr<QtConceptMap> qtconceptmap);

  bool CanDoCommand(const boost::shared_ptr<const Command> command) const noexcept;
  void DoCommand(const boost::shared_ptr<Command> command) noexcept;

  private:
  ///Displaying the concept map, the View
  ///This display is connected to the Widget/Model by signals
  ///and listening, instead of acted upon
  boost::shared_ptr<QtConceptMap> m_qtconceptmap;

  ///The nonvisual concept map widget, the Model
  ///The Widget is worked upon, the signal it emits are listened to by the QtConceptMap/View
  boost::shared_ptr<Widget> m_widget;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCONCEPTMAPWIDGET_H
