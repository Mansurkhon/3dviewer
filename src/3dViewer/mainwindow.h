#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <glwidget.h>
#include <qgifimage.h>

#include <QColorDialog>
#include <QDateTime>
#include <QDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);

  /* VARIABLES FOR OPENGL LOGIC */
  bool isObjFile = 0;
  int vertices_count;
  int edges_count;

  // ENUM MODES
  enum display_method { NONE, CIRCLE, SQUARE };
  enum projection_type { PARALLEL, CENTRAL };
  enum edges_type { SOLID, DASHED };

  ~MainWindow();

 private slots:
  void on_loadModelButton_clicked();
  void save_settings();
  void load_settings();
  void on_projection_types_currentIndexChanged(int index);
  void on_noneVertices_clicked();
  void on_circleVertices_clicked();
  void on_squareVertices_clicked();
  void on_sizeVerticesSlider_valueChanged(int value);
  void on_solidEdges_clicked();
  void on_dashedEdges_clicked();
  void on_thikEdgesSlider_valueChanged(int value);
  void on_scaleSlider_valueChanged(int value);
  void on_makePhotoButton_clicked();
  void on_makeGifButton_clicked();
  void make_gif();
  void on_backgroundColorButton_clicked();
  void on_verticesColorButton_clicked();
  void on_edgesColorButton_clicked();
  void on_verticesButton_clicked();
  void on_edgesButton_clicked();
  void on_backgroundButton_clicked();
  void on_rotateXSlider_valueChanged(int value);
  void on_rotateYSlider_valueChanged(int value);
  void on_rotateZSlider_valueChanged(int value);
  void on_translateXSlider_valueChanged(int value);
  void on_translateYSlider_valueChanged(int value);
  void on_translateZSlider_valueChanged(int value);
  void on_scale_increment_clicked();
  void on_scale_decrement_clicked();

 private:
  void set_mainwindow_attributes();
  void bind_slots();
  void restore_default_values();
  void set_default_frame();

  Ui::MainWindow *ui;
  QSettings *settings;
  QString gif_name;
  QGifImage *gif_frame;
  // QGifImage *gif_frame;
  int frames_counter = 0;
  QTimer *timer;

  QPoint cur_pos;
  QPoint new_pos;
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
};
#endif  // MAINWINDOW_H
