#ifdef _WIN32
//See http://www.richelbilderbeek.nl/CppCompileErrorSwprintfHasNotBeenDeclared.htm
#undef __STRICT_ANSI__
#endif

//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "qtpvdbconceptmapdialog.h"

#include <cassert>
#include <boost/lambda/lambda.hpp>

#include <QDesktopWidget>
#include <QFileDialog>
#include <QKeyEvent>
#include <QLayout>
#include <QLabel>
#include <QMessageBox>

#if (QT_VERSION >= QT_VERSION_CHECK(5,5,0))
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#else
#include <QPrintDialog>
#include <QPrinter>
#endif

#include "pvdbcluster.h"
#include "pvdbconcept.h"
#include "pvdbconceptmapfactory.h"
#include "pvdbconceptmap.h"
#include "pvdbedge.h"
#include "pvdbfile.h"
#include "pvdbnode.h"
#include "qtpvdbconcepteditdialog.h"
#include "qtpvdbconceptmapitem.h"
#include "qtpvdbedgeitem.h"
#include "qtpvdbprintconceptmapdialog.h"
#include "qtpvdbnodeitem.h"
#include "qtpvdbconceptmapeditwidget.h"
#include "qtpvdbconceptmapwidget.h"
#include "qtscopeddisable.h"
#include "trace.h"
#include "pvdbnodefactory.h"
#include "ui_qtpvdbconceptmapdialog.h"

///Collect all QGraphicsItems with class T in an unorderly way
template <class T>
std::vector<T*> Collect(const QGraphicsScene* const scene)
{
  std::vector<T*> v;
  const QList<QGraphicsItem *> items = scene->items();
  std::transform(items.begin(),items.end(),std::back_inserter(v),
    [](QGraphicsItem* const item)
    {
      assert(item);
      return dynamic_cast<T*>(item);
    }
  );
  v.erase(std::remove(v.begin(),v.end(),nullptr),v.end());
  assert(std::count(v.begin(),v.end(),nullptr)==0);
  return v;
}

QtPvdbConceptMapDialog::QtPvdbConceptMapDialog(
  const boost::shared_ptr<pvdb::File> file,
  QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtPvdbConceptMapDialog),
    m_back_to_menu(false),
    m_file(file),
    m_widget(CreateWidget(file->GetConceptMap()))
{
  ui->setupUi(this);
  #ifndef NDEBUG
  Test();
  assert(m_file);
  assert(m_file->GetConceptMap());
  assert(file == m_file);
  assert(m_widget);
  assert(m_widget->GetConceptMap() == m_file->GetConceptMap());
  assert(this->layout());
  #endif

  this->layout()->addWidget(m_widget);

  assert(!Collect<QtPvdbNodeItem>(m_widget->scene()).empty());

  //Center the dialog
  {
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->setGeometry(screen.adjusted(64,64,-64,-64));
    this->move( screen.center() - this->rect().center() );
  }

  m_widget->m_signal_conceptmapitem_requests_edit.connect(
    boost::bind(
      &QtPvdbConceptMapDialog::OnConceptMapItemRequestsEdit,
      this,boost::lambda::_1));
}

QtPvdbConceptMapDialog::~QtPvdbConceptMapDialog()
{
  delete ui;
}

QtPvdbConceptMapEditWidget * QtPvdbConceptMapDialog::CreateWidget(const boost::shared_ptr<pvdb::ConceptMap> concept_map)
{
  assert(concept_map);
  QtPvdbConceptMapEditWidget * const widget = new QtPvdbConceptMapEditWidget(concept_map);
  assert(widget);
  return widget;
}


/*
QtPvdbConceptMapEditWidget * QtPvdbConceptMapDialog::CreateWidget(const boost::shared_ptr<pvdb::File> file)
{
  assert(file);
  boost::shared_ptr<pvdb::ConceptMap> concept_map;

  if (!file->GetCluster()->Empty())
  {
    //TRACE("User supplied a filled-in cluster");
    assert(file->GetCluster());
    concept_map = pvdb::ConceptMapFactory::CreateFromCluster(
        file->GetConceptMap()->GetQuestion(),
        file->GetCluster()
      );
    assert((file->GetCluster()->Get().size() + 1)
      == (concept_map->GetNodes().size())
      && "As much cluster items as nodes + focus question");
  }
  else
  {
    //TRACE("User starts constructing a concept map from scratch or already filled in one");
    if (file->GetConceptMap()->GetNodes().empty())
    {
      const boost::shared_ptr<pvdb::Node > node = pvdb::NodeFactory::Create(file->GetQuestion());
      assert(node);
      const std::vector<boost::shared_ptr<pvdb::Node> > nodes = { node };
      const std::vector<boost::shared_ptr<pvdb::Edge> > edges = { };
      concept_map = pvdb::ConceptMapFactory::Create(nodes,edges);
    }
    else
    {
      concept_map = file->GetConceptMap();
    }
  }
  assert(concept_map);
  file->SetConceptMap(concept_map);

  assert(file->GetConceptMap());
  assert(file->GetConceptMap()->GetNodes().size() >= 1);

  QtPvdbConceptMapEditWidget * const widget = new QtPvdbConceptMapEditWidget(concept_map);
  assert(widget);
  return widget;
}
*/


void QtPvdbConceptMapDialog::DoRandomStuff()
{
  //Do random stuff
  #ifndef NDEBUG
  assert(m_file);
  assert(m_file->GetConceptMap());
  const int n_edges_before = boost::numeric_cast<int>(m_file->GetConceptMap()->GetEdges().size());
  const int n_nodes_before = boost::numeric_cast<int>(m_file->GetConceptMap()->GetNodes().size());
  #endif
  this->GetWidget()->DoRandomStuff();
  #ifndef NDEBUG
  const int n_edges_after = boost::numeric_cast<int>(m_file->GetConceptMap()->GetEdges().size());
  const int n_nodes_after = boost::numeric_cast<int>(m_file->GetConceptMap()->GetNodes().size());
  assert(n_edges_after > n_edges_before);
  assert(n_nodes_after > n_nodes_before);
  #endif
}

const QtPvdbConceptMapWidget * QtPvdbConceptMapDialog::GetWidget() const
{
  assert(m_widget);
  return m_widget;
}

QtPvdbConceptMapWidget * QtPvdbConceptMapDialog::GetWidget()
{
  //Calls the const version of GetWidget
  //To avoid duplication in const and non-const member functions [1]
  return const_cast<QtPvdbConceptMapWidget*>(
    const_cast<const QtPvdbConceptMapDialog*>(this)->GetWidget());
}

void QtPvdbConceptMapDialog::keyPressEvent(QKeyEvent* e)
{
  if (e->key()  == Qt::Key_Escape) { close(); return; }
  if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_S) { on_button_save_clicked(); return; }
  if ((e->modifiers() & Qt::ControlModifier)
    && !(e->modifiers() & Qt::ShiftModifier)
    && e->key() == Qt::Key_Z)
  {
    QMessageBox box;
    box.setText("TODO: Undo");
    box.exec();
    return;
  }
  if ( (e->modifiers() & Qt::ControlModifier)
    && (e->modifiers() & Qt::ShiftModifier)
    && e->key() == Qt::Key_Z)
  {
    QMessageBox box;
    box.setText("TODO: Redo");
    box.exec();
    return;
  }

  QDialog::keyPressEvent(e);
}

void QtPvdbConceptMapDialog::on_button_print_clicked()
{
  Save();
  QtPvdbPrintConceptMapDialog d(this->m_file);
  this->ShowChild(&d);
}

void QtPvdbConceptMapDialog::OnConceptMapItemRequestsEdit(QtPvdbConceptMapItem* const item)
{
  assert(item);
  {
    assert(this);
    assert(item);
    assert(item->GetConcept());
    QtPvdbConceptEditDialog d(item->GetConcept());
    this->ShowChild(&d);
  }
  //item->GetConcept()->m_signal_examples_changed(item->GetConcept().get()); //NEW 2013-01-08 21:40
  item->m_signal_item_has_updated(item);
  item->update();
}

void QtPvdbConceptMapDialog::on_button_save_clicked()
{
  //Temporarily disable to widget, otherwise saving cannot succeed
  const QtScopedDisable<QtPvdbConceptMapWidget> scoped_disable1(GetWidget());
  const QtScopedDisable<QtPvdbConceptMapDialog> scoped_disable2(this);
  this->hide();
  const std::string filter_str = std::string("*.") + pvdb::File::GetFilenameExtension();
  const std::string filename_raw = QFileDialog::getSaveFileName(0,"Sla de concept map op",
    QString(),
    filter_str.c_str()).toStdString();
  if (!filename_raw.empty())
  {
    const std::string filename
      =  (filename_raw.size() < pvdb::File::GetFilenameExtension().size()
        || filename_raw.substr( filename_raw.size() - 3, 3 ) != pvdb::File::GetFilenameExtension()
       ? filename_raw + std::string(".") + pvdb::File::GetFilenameExtension()
       : filename_raw);
    assert(filename.size() > 3
      && filename.substr( filename.size() - 3, 3 ) == pvdb::File::GetFilenameExtension()
      && "File must have correct file extension name");
    Save(filename);
    //this->m_back_to_menu = true; //2013-04-19 Request by client
    //close(); //2013-04-19 Request by client
  }
  else
  {
    this->show();
  }
}

void QtPvdbConceptMapDialog::Save() const
{
  //const boost::shared_ptr<pvdb::ConceptMap> concept_map = GetWidget()->GetConceptMap();
  //assert(concept_map);
  assert(m_file->GetConceptMap() == GetWidget()->GetConceptMap());
  //m_file->SetConceptMap(concept_map);
}

void QtPvdbConceptMapDialog::Save(const std::string& filename) const
{
  assert(filename.size() > 3
    && filename.substr( filename.size() - 3, 3 ) == pvdb::File::GetFilenameExtension()
    && "File must have correct file extension name");
  Save();
  m_file->Save(filename);
}

void QtPvdbConceptMapDialog::Shuffle()
{
  this->GetWidget()->Shuffle();
}

