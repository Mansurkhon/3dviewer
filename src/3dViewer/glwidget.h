#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

#include <QMessageBox>
#include <QOpenGLWidget>
#include <QWheelEvent>
#include <QWidget>
#include <QtOpenGL>
#include <iostream>

extern "C" {
#include "../modules/model_load.h"
#include "../modules/transformations.h"
}

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  explicit GLWidget(QWidget *parent = Q_NULLPTR);

  // MODEL DATA START
  double rotate_x;
  double rotate_y;
  double rotate_z;
  double translate_x;
  double translate_y;
  double translate_z;
  double scale_v = 50;
  double vertices_size = 1;
  double edges_thickness = 1;
  int projection_type = 0;
  int v_display_method = 0;
  int edges_type = 0;
  double v_red = 1, v_green = 1, v_blue = 1;
  double e_red = 1, e_green = 1, e_blue = 1;
  double bg_red = 0, bg_green = 0, bg_blue = 0;

  // OBJECT MODEL
  char *filename;
  obj_t data = {0, NULL, 0, NULL};

  GLfloat normalize_coef;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

  // OPENGL
  void initializeGL() override;
  void paintGL() override;

  void parse_object_file();

 private:
  ~GLWidget() override;
  QPoint cur_pos;
  QPoint new_pos;
  void set_normalize_coef();
  void render_ui_stats();
  void draw_lines();
  void draw_points();

 signals:
};

#endif  // GLWIDGET_H
