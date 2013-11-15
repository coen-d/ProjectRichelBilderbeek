#ifndef QTAMINOACIDFIGHTERMAINDIALOG_H
#define QTAMINOACIDFIGHTERMAINDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <QDialog>
#pragma GCC diagnostic pop

struct QGraphicsScene;
struct QGraphicsPixmapItem;

namespace Ui {
  class QtAminoAcidFighterMainDialog;
}

namespace ribi {
namespace aaf {

class QtAminoAcidFighterMainDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QtAminoAcidFighterMainDialog(QWidget *parent = 0);
<<<<<<< HEAD
  QtAminoAcidFighterMainDialog(const QtAminoAcidFighterMainDialog&);
  QtAminoAcidFighterMainDialog operator=(const QtAminoAcidFighterMainDialog&);
=======
  QtAminoAcidFighterMainDialog(const QtAminoAcidFighterMainDialog&) = delete;
  QtAminoAcidFighterMainDialog& operator=(const QtAminoAcidFighterMainDialog&) = delete;
>>>>>>> 72dc2cc2513f4d68edad78fda1f5b48d89c718fb
  ~QtAminoAcidFighterMainDialog();

private:
  Ui::QtAminoAcidFighterMainDialog *ui;
  QTimer * const m_timer;
  QGraphicsScene * const m_scene;
  QGraphicsPixmapItem * const m_sprite;
  double m_angle;

  void resizeEvent(QResizeEvent*);
  void keyPressEvent(QKeyEvent* e);

private slots:
  void onTick();
  void onCheck();
  void onButtonLeft();
  void onButtonRight();
};

} //~namespace aaf
} //~namespace ribi

#endif // QTAMINOACIDFIGHTERMAINDIALOG_H
