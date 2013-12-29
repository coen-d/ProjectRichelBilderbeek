#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qtpvdboverviewdialog.h"

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QIcon>
#include <QTimer>
#include <QWidget>
#include "qtpvdboverviewwidget.h"

#include "pvdbfile.h"
#include "pvdbfilefactory.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapfactory.h"
#include "qtconceptmapconcepteditdialog.h"
#include "qtconceptmaprateconcepttallydialognewname.h"
#include "qtpvdbprintconceptmapdialog.h"
#include "qtpvdbprintratingdialog.h"
#include "qttesteditconceptmapdialog.h"
#include "qttestrateconceptmapdialog.h"
#include "qtpvdbassessormenudialog.h"
#include "qtconceptmaprateconceptdialognewname.h"
#include "qtpvdbrateconceptmapdialog.h"
#include "qtconceptmaprateexamplesdialognewname.h"
#include "qtpvdbclusterdialog.h"
#include "qtpvdbconceptmapdialog.h"
#include "qtpvdbcreateassessmentcompletedialog.h"
#include "qtpvdbcreateassessmentmenudialog.h"
#include "qtpvdbcreateassessmentpartialdialog.h"
#include "qtpvdbratingdialog.h"
#include "qtpvdbmenudialog.h"
#include "qtpvdbstudentmenudialog.h"
#include "qtpvdbstudentstartcompletedialog.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::pvdb::QtPvdbOverviewWidget::QtPvdbOverviewWidget(QWidget* parent)
  : QGraphicsView(new QGraphicsScene,parent),
    m_dialogs(GetAllDialogs())
{
  assert(scene());
  assert(std::count_if(m_dialogs.begin(),m_dialogs.end(),[](QtHideAndShowDialog* p) { return !p; } ) == 0);
  //assert(std::all_of(m_dialogs.begin(),m_dialogs.end(),[](QtHideAndShowDialog* p) { return p; } ));
  

  double y_sum = 0.0;
  double max_width = 0.0;

  const int sz = m_dialogs.size();
  for (int i = 0; i!=sz; ++i)
  {
    //Convert dialog to widget explicitly for Wine
    assert(m_dialogs[i]);
    QWidget * const widget = m_dialogs[i];
    assert(widget);

    QGraphicsProxyWidget * const proxy = scene()->addWidget(widget,Qt::Dialog);
    assert(proxy);

    proxy->setPos(32,y_sum);

    y_sum += proxy->rect().height();
    y_sum += 64; //Added by window height
    max_width = std::max(max_width,proxy->rect().width());

    const std::string window_title
      = m_dialogs[i]->windowTitle().toStdString()
      + " ("
      + m_dialogs[i]->objectName().toStdString()
      + ")";
    proxy->setWindowTitle(window_title.c_str());
  }
  this->setGeometry(0,0,max_width,y_sum);
}


const std::vector<ribi::QtHideAndShowDialog* > ribi::pvdb::QtPvdbOverviewWidget::GetAllDialogs()
{
  std::vector<QtHideAndShowDialog* > v;
  {
    QtHideAndShowDialog* p(new QtPvdbAssessorMenuDialog);
    assert(p);
    v.push_back(p);
  }
  {
    const boost::shared_ptr<pvdb::File> file(pvdb::FileFactory::GetTests().at(3));
    assert(file);
    assert(file->GetCluster());
    QtHideAndShowDialog* p(new QtPvdbClusterDialog(file));
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new cmap::QtConceptMapConceptEditDialog(cmap::ConceptFactory::GetTests().at(4)));
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbConceptMapDialog(pvdb::FileFactory::GetTests().at(2)));
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbCreateAssessmentCompleteDialog);
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbCreateAssessmentMenuDialog);
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbMenuDialog);
    assert(p);
    v.push_back(p);
  }
  {
    const boost::shared_ptr<pvdb::File> file = pvdb::FileFactory::GetTests().at(4);
    assert(file);
    QtHideAndShowDialog* p(new QtPvdbPrintConceptMapDialog(file));
    assert(p);
    v.push_back(p);
  }
  {
    const boost::shared_ptr<pvdb::File> file = pvdb::FileFactory::GetTests().at(4);
    assert(file);
    QtHideAndShowDialog* p(new QtPvdbPrintConceptMapDialog(file));
    assert(p);
    v.push_back(p);
  }
  {
    const boost::shared_ptr<ribi::cmap::ConceptMap> concept_map
      = ribi::cmap::ConceptMapFactory::GetHeteromorphousTestConceptMaps().at(18);
    assert(concept_map);
    QtHideAndShowDialog* p(new cmap::QtRateConceptDialogNewName(concept_map));
    assert(p);
    v.push_back(p);
  }
  {
    const boost::shared_ptr<ribi::cmap::ConceptMap> concept_map
      = ribi::cmap::ConceptMapFactory::GetHeteromorphousTestConceptMaps().at(18);
    assert(concept_map);
    QtHideAndShowDialog* p(new cmap::QtRateConceptTallyDialogNewName(concept_map));
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbRateConceptMapDialog(pvdb::FileFactory::GetTests().at(2)));
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new cmap::QtRateExamplesDialogNewName(cmap::ConceptFactory::GetTests().at(4)));
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbRatingDialog(pvdb::FileFactory::GetTests().at(4)));
    assert(p);
    v.push_back(p);
  }
  {
    #ifdef REALLY_DEMONSTRATE_INFINITE_RECURSION_7236834589746034
    QtHideAndShowDialog*(new QtPvdbOverviewDialog);
    assert(p);
    v.push_back(p);
    #endif
  }
  {
    QtHideAndShowDialog* p(new QtPvdbStudentMenuDialog(pvdb::FileFactory::GetTests().at(2)));
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbStudentStartCompleteDialog(pvdb::FileFactory::GetTests().at(2)));
    assert(p);
    v.push_back(p);
  }
  #ifdef PVDB_ALSO_SHOW_TEST_DIALOGS
  {
    QtHideAndShowDialog* p(new QtPvdbTestConceptItemDialog);
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbTestNodeItemDialog);
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbTestEdgeItemDialog);
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbTestConceptMapEditWidgetDialog);
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbTestConceptMapRateWidgetDialog);
    assert(p);
    v.push_back(p);
  }
  {
    QtHideAndShowDialog* p(new QtPvdbViewTestsDialog);
    assert(p);
    v.push_back(p);
  }
  #endif

  assert(std::count_if(v.begin(),v.end(),[](QtHideAndShowDialog* p) { return !p; } ) == 0);
  //assert(std::all_of(v.begin(),v.end(),[](QtHideAndShowDialog* p) { return p; } )); //MAJ
  return v;
}

void ribi::pvdb::QtPvdbOverviewWidget::mouseDoubleClickEvent(QMouseEvent *)
{
  QImage image(scene()->sceneRect().size().toSize(), QImage::Format_ARGB32); // Create the image with the exact size of the shrunk scene
  image.fill(Qt::transparent);                                               // Start all pixels transparent
  QPainter painter(&image);
  scene()->render(&painter);
  image.save("file_name.png");
}
