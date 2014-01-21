#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "qttestimagecanvasmaindialog.h"

#include <cassert>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "fileio.h"
#include "imagecanvas.h"
#include "trace.h"
#include "ui_qttestimagecanvasmaindialog.h"
#pragma GCC diagnostic pop

ribi::QtTestImageCanvasMainDialog::QtTestImageCanvasMainDialog(QWidget *parent) :
  QtHideAndShowDialog(parent),
  ui(new Ui::QtTestImageCanvasMainDialog),
  m_canvas(CreateCanvas())
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);

  m_canvas->m_signal_changed.connect(
    boost::bind(
      &ribi::QtTestImageCanvasMainDialog::ShowCanvas,this,
      boost::lambda::_1)
    );

  ShowCanvas(0);
}

ribi::QtTestImageCanvasMainDialog::~QtTestImageCanvasMainDialog() noexcept
{
  delete ui;
}

const boost::shared_ptr<ribi::ImageCanvas> ribi::QtTestImageCanvasMainDialog::CreateCanvas()
{
  const int n_cols = 40;
  const std::string filename { fileio::GetTempFileName() };
  QFile file(":/ToolTestImageCanvas/images/R.png");
  file.copy(filename.c_str());
  assert(fileio::IsRegularFile(filename));
  boost::shared_ptr<ImageCanvas> canvas(new ImageCanvas(filename,n_cols));
  fileio::DeleteFile(filename);
  assert(!fileio::IsRegularFile(filename));
  return canvas;
}

ribi::CanvasColorSystem ribi::QtTestImageCanvasMainDialog::GetColorSystem() const noexcept
{
  const CanvasColorSystem color_system
    = ui->box_color_system->currentIndex() == 0
    ? CanvasColorSystem::normal
    : CanvasColorSystem::invert;
  return color_system;
}

ribi::CanvasCoordinatSystem ribi::QtTestImageCanvasMainDialog::GetCoordinatSystem() const noexcept
{
  const CanvasCoordinatSystem coordinat_system
    = ui->box_coordinat_system->currentIndex() == 0
    ? CanvasCoordinatSystem::screen
    : CanvasCoordinatSystem::graph;
  return coordinat_system;
}

int ribi::QtTestImageCanvasMainDialog::GetNcols() const noexcept
{
  return ui->box_n_cols->value();
}

void ribi::QtTestImageCanvasMainDialog::on_box_color_system_currentIndexChanged(int )
{
  this->m_canvas->SetColorSystem(GetColorSystem());
  //Should redraw automatically
}

void ribi::QtTestImageCanvasMainDialog::on_box_coordinat_system_currentIndexChanged(int )
{
  this->m_canvas->SetCoordinatSystem(GetCoordinatSystem());
  //Should redraw automatically
}

void ribi::QtTestImageCanvasMainDialog::ShowCanvas(const ribi::ImageCanvas * const)
{
  //Display the image
  std::stringstream s;
  s << (*m_canvas);
  ui->text->setPlainText(s.str().c_str());

}

#ifndef NDEBUG
void ribi::QtTestImageCanvasMainDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::QtTestImageCanvasMainDialog::Test");
  QtTestImageCanvasMainDialog d;
  d.on_button_image_clicked();
  TRACE("Finished ribi::QtTestImageCanvasMainDialog::Test successfully");
}
#endif

void ribi::QtTestImageCanvasMainDialog::on_button_image_clicked()
{
  QFile qfile(":/ToolTestImageCanvas/images/R.png");
  const std::string filename { fileio::GetTempFileName() };
  qfile.copy(filename.c_str());
  assert(fileio::IsRegularFile(filename));
  m_canvas.reset(
    new ImageCanvas(
      filename,
      GetNcols(),
      GetColorSystem(),
      GetCoordinatSystem()
    )
  );
  fileio::DeleteFile(filename);
  assert(!fileio::IsRegularFile(filename));
  this->ShowCanvas(m_canvas.get());
}

void ribi::QtTestImageCanvasMainDialog::on_box_n_cols_valueChanged(int )
{
  on_button_image_clicked();
}