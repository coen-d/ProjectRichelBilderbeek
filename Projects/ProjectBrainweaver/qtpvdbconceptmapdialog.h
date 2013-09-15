#ifndef QTPVDBCONCEPTMAPDIALOG_H
#define QTPVDBCONCEPTMAPDIALOG_H

#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"

#include "pvdbfwd.h"

namespace Ui { class QtPvdbConceptMapDialog; }

namespace ribi {

namespace pvdb {

class QtPvdbConceptMapDialog : public ::ribi::QtHideAndShowDialog
{
  Q_OBJECT
    
  public:
  explicit QtPvdbConceptMapDialog(const boost::shared_ptr<File> file, QWidget* parent = 0);
  QtPvdbConceptMapDialog(const QtPvdbConceptMapDialog&) = delete;
  QtPvdbConceptMapDialog& operator=(const QtPvdbConceptMapDialog&) = delete;
  ~QtPvdbConceptMapDialog();

  #ifndef NDEBUG
  ///Do random stuff, for example, add a node and an edge (used for debugging)
  void DoRandomStuff();
  #endif

  ///Obtain the widget
  const QtPvdbConceptMapWidget * GetWidget() const;
  QtPvdbConceptMapWidget * GetWidget();

  ///Does the user need to go back to the student menu?
  bool GoBackToMenu() const { return m_back_to_menu; }

  ///Save to m_file
  void Save() const;

  ///Save to disk
  void Save(const std::string& filename) const;

  #ifndef NDEBUG
  ///Shuffle to concepts (used for debugging)
  void Shuffle();
  #endif

  private slots:

  void keyPressEvent(QKeyEvent *);
  void on_button_save_clicked();
  void on_button_print_clicked();

  //void on_button_test_clicked();

private:
  Ui::QtPvdbConceptMapDialog *ui;

  ///Must we go back to the student menu?
  bool m_back_to_menu;

  ///The file
  boost::shared_ptr<pvdb::File> m_file;

  ///The concept map widget
  QtPvdbConceptMapEditWidget * const m_widget;

  //static QtPvdbConceptMapEditWidget * CreateWidget(const boost::shared_ptr<ribi::pvdb::ConceptMap> concept_map);
  ///DON'T USE: puts a new ConceptMap in file
  static QtPvdbConceptMapEditWidget * CreateWidget(const boost::shared_ptr<pvdb::File> file);

  ///NEW 2013-01-07
  void OnConceptMapItemRequestsEdit(QtPvdbConceptMapItem* const item);

  #ifndef NDEBUG
  ///Test this class
  static void Test();
  #endif
};

} //~namespace pvdb

} //~namespace ribi

#endif // QTPVDBCONCEPTMAPDIALOG_H
