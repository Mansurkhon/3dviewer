#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

GLWidget::~GLWidget() { data_free(&data); }

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
}

void GLWidget::paintGL() {
  glClearColor(bg_red, bg_green, bg_blue, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Кол-во координат для вершины
  // Тип данных  в массиве
  // Расстояния между вершинами в массиве
  // Указатель на массив
  glVertexPointer(3, GL_DOUBLE, 0, data.vertices_arr);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (this->projection_type == 0) {
    glFrustum(-1 * normalize_coef, 1 * normalize_coef, -1 * normalize_coef,
              1 * normalize_coef, normalize_coef, 1000 * normalize_coef);
    glTranslatef(0, 0, -2 * normalize_coef);
    glRotatef(30, 1, 0, 0);
  } else {
    glOrtho(-1 * normalize_coef, 1 * normalize_coef, -1 * normalize_coef,
            1 * normalize_coef, -1 * normalize_coef, 1000 * normalize_coef);
    glTranslatef(0, -normalize_coef / 2, 0);
  }
  glEnableClientState(GL_VERTEX_ARRAY);  // enable open gl state
  if (this->v_display_method != 0) {
    draw_points();
  }
  draw_lines();
  glDisableClientState(GL_VERTEX_ARRAY);
}

void GLWidget::parse_object_file() {
  data_free(&this->data);
  data = {0, NULL, 0, NULL};
  if (this->filename[0] != '\0') {
    parse_obj(this->filename, &this->data);
    set_normalize_coef();
    update();
  } else {
    QMessageBox warning = QMessageBox();
    warning.setWindowTitle("Error");
    warning.setText("Пожалуйста выберите .obj файл");
    warning.setIcon(QMessageBox::Warning);
    warning.exec();
  }
}

void GLWidget::set_normalize_coef() {
  normalize_coef = -10;

  for (size_t i = 0; i < data.vertices_count * 3; i++) {
    if (abs(data.vertices_arr[i]) > normalize_coef) {
      normalize_coef = abs(data.vertices_arr[i]);
    }
  }
}

void GLWidget::draw_lines() {
  if (this->edges_type == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  }
  glLineWidth(this->edges_thickness);
  glColor3f(this->e_red, this->e_green, this->e_blue);
  glDrawElements(GL_LINES, data.vertex_indices_count * 2, GL_UNSIGNED_INT,
                 data.vertex_indices_arr);
  if (this->edges_type == 0) {
    glDisable(GL_LINE_STIPPLE);
  }
}

void GLWidget::draw_points() {
  if (this->v_display_method == 1) {
    glEnable(GL_POINT_SMOOTH);
  }
  glPointSize(this->vertices_size);
  glColor3f(this->v_red, this->v_green, this->v_blue);
  glDrawArrays(GL_POINTS, 0, data.vertices_count);
  if (this->v_display_method == 1) {
    glDisable(GL_POINT_SMOOTH);
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  new_pos = QPoint(event->globalPosition().toPoint() - cur_pos);

  if (event->buttons() & Qt::LeftButton) {
    move_X(&this->data, new_pos.x() * this->normalize_coef / 5000);
    move_Y(&this->data, -new_pos.y() * this->normalize_coef / 5000);
    update();
  } else if (event->buttons() & Qt::RightButton) {
    rotate_X(&this->data, -new_pos.y() * 0.005);
    rotate_Y(&this->data, new_pos.x() * 0.005);
    update();
  }
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  QPoint numDegrees = event->angleDelta() / 120;
  double step = normalize_coef / 10;
  double scale_tmp = scale_v;
  if ((int)(scale_v + numDegrees.y() * step) > 0) {
    scale_v += numDegrees.y() * step;
    scale(&this->data, scale_v / scale_tmp);
    update();
  }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  cur_pos = event->globalPosition().toPoint();
}
