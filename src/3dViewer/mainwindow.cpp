#include "mainwindow.h"

#include <QDebug>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->openGLWidget->setFixedSize(511, 431);
  set_default_frame();
  set_mainwindow_attributes();
  bind_slots();
  settings = new QSettings("21school", "3D_Viewer", this);
  load_settings();
}

MainWindow::~MainWindow() {
  save_settings();
  data_free(&ui->openGLWidget->data);
  delete settings;
  delete ui;
}

void MainWindow::set_mainwindow_attributes() { timer = new QTimer(0); }

void MainWindow::set_default_frame() {
  ui->frameVertices->setVisible(true);
  ui->frameEdges->setVisible(false);
  ui->frameBackground->setVisible(false);
  ui->verticesButton->setStyleSheet("background-color: #2196F3;");
  ui->edgesButton->setStyleSheet("background-color: #424242;");
  ui->backgroundButton->setStyleSheet("background-color: #424242;");
}

void MainWindow::save_settings() {
  settings->setValue("are_settings", 1);
  settings->setValue("projection_type", ui->openGLWidget->projection_type);
  settings->setValue("v_display_method", ui->openGLWidget->v_display_method);
  settings->setValue("vertices_size", ui->openGLWidget->vertices_size);
  settings->setValue("v_red", ui->openGLWidget->v_red);
  settings->setValue("v_green", ui->openGLWidget->v_green);
  settings->setValue("v_blue", ui->openGLWidget->v_blue);
  settings->setValue("edges_type", ui->openGLWidget->edges_type);
  settings->setValue("edges_thickness", ui->openGLWidget->edges_thickness);
  settings->setValue("e_red", ui->openGLWidget->e_red);
  settings->setValue("e_green", ui->openGLWidget->e_green);
  settings->setValue("e_blue", ui->openGLWidget->e_blue);
  settings->setValue("bg_red", ui->openGLWidget->bg_red);
  settings->setValue("bg_green", ui->openGLWidget->bg_green);
  settings->setValue("bg_blue", ui->openGLWidget->bg_blue);
}

void MainWindow::restore_default_values() {
  ui->scaleSlider->setValue(50);
  ui->rotateXSlider->setValue(0);
  ui->rotateYSlider->setValue(0);
  ui->rotateZSlider->setValue(0);
  ui->translateXSlider->setValue(0);
  ui->translateYSlider->setValue(0);
  ui->translateZSlider->setValue(0);
}

void MainWindow::load_settings() {
  if (settings->value("are_settings").toInt()) {
    char rgba_color[40];
    ui->projection_types->setCurrentIndex(
        settings->value("projection_type").toInt());
    int v_mode = settings->value("v_display_method").toInt();
    if (v_mode == 0) {
      ui->noneVertices->setChecked(true);
      ui->openGLWidget->v_display_method = 0;
    } else if (v_mode == 1) {
      ui->circleVertices->setChecked(true);
      ui->openGLWidget->v_display_method = 1;
    } else if (v_mode == 2) {
      ui->squareVertices->setChecked(true);
      ui->openGLWidget->v_display_method = 2;
    }
    ui->sizeVerticesSlider->setValue(
        settings->value("vertices_size").toDouble());
    ui->openGLWidget->v_red = settings->value("v_red").toDouble();
    ui->openGLWidget->v_green = settings->value("v_green").toDouble();
    ui->openGLWidget->v_blue = settings->value("v_blue").toDouble();
    sprintf(rgba_color, "background-color: rgb(%d,%d,%d)",
            (int)(ui->openGLWidget->v_red * 255),
            (int)(ui->openGLWidget->v_green * 255),
            (int)(ui->openGLWidget->v_blue * 255));
    ui->edgesColorButton->setStyleSheet(rgba_color);  // --> исправить
    int edges_mode = settings->value("edges_type").toInt();
    if (edges_mode == 0) {
      ui->solidEdges->setChecked(true);
      ui->openGLWidget->edges_type = 0;
    } else if (edges_mode == 1) {
      ui->dashedEdges->setChecked(true);
      ui->openGLWidget->edges_type = 1;
    }
    ui->thikEdgesSlider->setValue(settings->value("edges_thickness").toInt() *
                                  10);
    ui->openGLWidget->e_red = settings->value("e_red").toDouble();
    ui->openGLWidget->e_green = settings->value("e_green").toDouble();
    ui->openGLWidget->e_blue = settings->value("e_blue").toDouble();
    sprintf(rgba_color, "background-color: rgb(%d,%d,%d)",
            (int)(ui->openGLWidget->e_red * 255),
            (int)(ui->openGLWidget->e_green * 255),
            (int)(ui->openGLWidget->e_blue * 255));
    ui->verticesColorButton->setStyleSheet(rgba_color);  // --> исправить
    ui->openGLWidget->bg_red = settings->value("bg_red").toDouble();
    ui->openGLWidget->bg_green = settings->value("bg_green").toDouble();
    ui->openGLWidget->bg_blue = settings->value("bg_blue").toDouble();
    sprintf(rgba_color, "background-color: rgb(%d,%d,%d)",
            (int)(ui->openGLWidget->bg_red * 255),
            (int)(ui->openGLWidget->bg_green * 255),
            (int)(ui->openGLWidget->bg_blue * 255));
    ui->backgroundColorButton->setStyleSheet(rgba_color);  // --> исправить
  }
}

void MainWindow::on_loadModelButton_clicked() {
  QString QString_filename = QFileDialog::getOpenFileName(
      this, tr("Откройте .obj файл:"), "~/", tr("Obj Files (*.obj)"));
  restore_default_values();
  std::string std_filename = QString_filename.toStdString();
  ui->openGLWidget->filename = (char *)std_filename.c_str();
  ui->openGLWidget->parse_object_file();
  if (ui->openGLWidget->filename[0] != '\0') {
    isObjFile = 1;
    char *start = strrchr((char *)std_filename.c_str(), '/') + 1;
    char *ext = strrchr((char *)std_filename.c_str(), '.');
    *ext = '\0';
    ui->filenameLabel->setText(start);
  } else {
    isObjFile = 0;
    ui->filenameLabel->setText("-");
  }

  ui->verticesLabel->setText(
      QString::number(ui->openGLWidget->data.vertices_count));
  ui->edgesLabel->setText(
      QString::number(ui->openGLWidget->data.vertex_indices_count));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
  new_pos = QPoint(event->globalPosition().toPoint() - cur_pos);
  move(x() + new_pos.x(), y() + new_pos.y());
  cur_pos = event->globalPosition().toPoint();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  cur_pos = event->globalPosition().toPoint();
}

void MainWindow::on_projection_types_currentIndexChanged(int index) {
  if (index == 0) {
    ui->openGLWidget->projection_type = PARALLEL;
  } else if (index == 1) {
    ui->openGLWidget->projection_type = CENTRAL;
  }
  ui->openGLWidget->update();
}

void MainWindow::on_noneVertices_clicked() {
  ui->openGLWidget->v_display_method = NONE;
  ui->openGLWidget->update();
}

void MainWindow::on_circleVertices_clicked() {
  ui->openGLWidget->v_display_method = CIRCLE;
  ui->openGLWidget->update();
}

void MainWindow::on_squareVertices_clicked() {
  ui->openGLWidget->v_display_method = SQUARE;
  ui->openGLWidget->update();
}

void MainWindow::on_sizeVerticesSlider_valueChanged(int value) {
  ui->openGLWidget->vertices_size = value;
  ui->openGLWidget->update();
}

void MainWindow::on_solidEdges_clicked() {
  ui->openGLWidget->edges_type = SOLID;
  ui->openGLWidget->update();
}

void MainWindow::on_dashedEdges_clicked() {
  ui->openGLWidget->edges_type = DASHED;
  ui->openGLWidget->update();
}

void MainWindow::on_thikEdgesSlider_valueChanged(int value) {
  ui->openGLWidget->edges_thickness = value / 10;
  ui->openGLWidget->update();
}

void MainWindow::on_scaleSlider_valueChanged(int value) {
  double val = (double)value / ui->openGLWidget->scale_v;
  scale(&ui->openGLWidget->data, val);
  ui->openGLWidget->scale_v = value;
  ui->openGLWidget->update();
}

void MainWindow::on_makePhotoButton_clicked() {
  if (isObjFile == 1) {
    QFileDialog dialogConnectImage(this);
    QDateTime current_date = QDateTime::currentDateTime();
    QString formattedTime = current_date.toString("yyyy-MM-dd hh.mm.ss");
    QString name_pattern = "Screen Shot " + formattedTime + ".jpeg";
    QString image_name = dialogConnectImage.getSaveFileName(
        this, tr("Сохраните снимок экрана"), name_pattern,
        tr("image (*.jpeg *.bmp)"));
    if (image_name.length() >= 1) {
      QImage img = ui->openGLWidget->grabFramebuffer();
      img.save(image_name);
      QMessageBox messageBoxImage;
      messageBoxImage.information(0, "", "Снимок экрана успешлно сохранён");
    }
  } else {
    QMessageBox warning = QMessageBox();
    warning.setWindowTitle("Error");
    warning.setText(
        "Пожалуйста откройте .obj файл для того чтобы сделать снимок экрана");
    warning.setIcon(QMessageBox::Warning);
    warning.exec();
  }
}

void MainWindow::on_makeGifButton_clicked() {
  if (isObjFile == 1) {
    QDateTime current_date = QDateTime::currentDateTime();
    QString formattedTime = current_date.toString("yyyy-MM-dd hh.mm.ss");
    QString name_pattern = "Screen Cast " + formattedTime + ".gif";
    gif_name = QFileDialog::getSaveFileName(this, tr("Сохранить gif анимацию"),
                                            name_pattern, tr("gif (*.gif)"));
    if (gif_name != "") {
      ui->makeGifButton->setDisabled(true);
      gif_frame = new QGifImage;
      gif_frame->setDefaultDelay(10);
      timer->setInterval(100);
      timer->start();
    }
  } else {
    QMessageBox warning = QMessageBox();
    warning.setWindowTitle("Error");
    warning.setText(
        "Пожалуйста откройте .obj файл для того чтобы сделать gif анимацию");
    warning.setIcon(QMessageBox::Warning);
    warning.exec();
  }
}

void MainWindow::make_gif() {
  QImage image = ui->openGLWidget->grabFramebuffer();
  QSize image_size(640, 480);
  QImage scaled_image = image.scaled(image_size);
  gif_frame->addFrame(scaled_image);
  if (frames_counter == 50) {
    timer->stop();
    gif_frame->save(gif_name);
    frames_counter = 0;
    QMessageBox messageBoxGif;
    messageBoxGif.information(0, "", "Gif анимация сохранена");
    delete gif_frame;
    ui->makeGifButton->setText("");
    ui->makeGifButton->setEnabled(true);
  }
  frames_counter++;
  if (!ui->makeGifButton->isEnabled()) {
    ui->makeGifButton->setText(QString::number(frames_counter / 10) + "s");
  } else {
    ui->makeGifButton->setText("GIF");
  }
}

void MainWindow::bind_slots() {
  QPushButton *closeButton = findChild<QPushButton *>("qt_close_button");
  if (closeButton) {
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
  }
  connect(timer, SIGNAL(timeout()), this, SLOT(make_gif()));
}

void MainWindow::on_backgroundColorButton_clicked() {
  QColor bg_color = QColorDialog::getColor(Qt::white, this->ui->frameBackground,
                                           "Выберите цвет");
  if (bg_color.isValid()) {
    ui->openGLWidget->bg_red = bg_color.redF();
    ui->openGLWidget->bg_green = bg_color.greenF();
    ui->openGLWidget->bg_blue = bg_color.blueF();
    char rgba_color[40];
    sprintf(rgba_color, "background-color: rgb(%d,%d,%d)", bg_color.red(),
            bg_color.green(), bg_color.blue());
    ui->backgroundColorButton->setStyleSheet(rgba_color);
    ui->openGLWidget->update();
  }
}

void MainWindow::on_edgesColorButton_clicked() {
  QColor edge_color =
      QColorDialog::getColor(Qt::white, this->ui->frameEdges, "Выберите цвет");
  if (edge_color.isValid()) {
    ui->openGLWidget->e_red = edge_color.redF();
    ui->openGLWidget->e_green = edge_color.greenF();
    ui->openGLWidget->e_blue = edge_color.blueF();
    char rgba_color[40];
    sprintf(rgba_color, "background-color: rgb(%d,%d,%d)", edge_color.red(),
            edge_color.green(), edge_color.blue());
    ui->edgesColorButton->setStyleSheet(rgba_color);
    ui->openGLWidget->update();
  }
}

void MainWindow::on_verticesColorButton_clicked() {
  QColor vert_color = QColorDialog::getColor(Qt::white, this->ui->frameVertices,
                                             "Выберите цвет");
  if (vert_color.isValid()) {
    ui->openGLWidget->v_red = vert_color.redF();
    ui->openGLWidget->v_green = vert_color.greenF();
    ui->openGLWidget->v_blue = vert_color.blueF();
    char rgba_color[40];
    sprintf(rgba_color, "background-color: rgb(%d,%d,%d)", vert_color.red(),
            vert_color.green(), vert_color.blue());
    ui->verticesColorButton->setStyleSheet(rgba_color);
    ui->openGLWidget->update();
  }
}

void MainWindow::on_verticesButton_clicked() { set_default_frame(); }

void MainWindow::on_edgesButton_clicked() {
  ui->frameVertices->setVisible(false);
  ui->frameEdges->setVisible(true);
  ui->frameBackground->setVisible(false);
  ui->verticesButton->setStyleSheet("background-color: #424242;");
  ui->edgesButton->setStyleSheet("background-color: #2196F3;");
  ui->backgroundButton->setStyleSheet("background-color: #424242;");
}

void MainWindow::on_backgroundButton_clicked() {
  ui->frameVertices->setVisible(false);
  ui->frameEdges->setVisible(false);
  ui->frameBackground->setVisible(true);
  ui->verticesButton->setStyleSheet("background-color: #424242;");
  ui->edgesButton->setStyleSheet("background-color: #424242;");
  ui->backgroundButton->setStyleSheet("background-color: #2196F3;");
}

void MainWindow::on_rotateXSlider_valueChanged(int value) {
  rotate_X(&ui->openGLWidget->data, (value - ui->openGLWidget->rotate_x));
  ui->openGLWidget->rotate_x = value;
  ui->openGLWidget->update();
}

void MainWindow::on_rotateYSlider_valueChanged(int value) {
  rotate_Y(&ui->openGLWidget->data, (value - ui->openGLWidget->rotate_y));
  ui->openGLWidget->rotate_y = value;
  ui->openGLWidget->update();
}

void MainWindow::on_rotateZSlider_valueChanged(int value) {
  rotate_Z(&ui->openGLWidget->data, (value - ui->openGLWidget->rotate_z));
  ui->openGLWidget->rotate_z = value;
  ui->openGLWidget->update();
}

void MainWindow::on_translateXSlider_valueChanged(int value) {
  move_X(&ui->openGLWidget->data, (value - ui->openGLWidget->translate_x) *
                                      ui->openGLWidget->normalize_coef / 100);
  ui->openGLWidget->translate_x = value;
  ui->openGLWidget->update();
}

void MainWindow::on_translateYSlider_valueChanged(int value) {
  move_Y(&ui->openGLWidget->data, (value - ui->openGLWidget->translate_y) *
                                      ui->openGLWidget->normalize_coef / 100);
  ui->openGLWidget->translate_y = value;
  ui->openGLWidget->update();
}

void MainWindow::on_translateZSlider_valueChanged(int value) {
  move_Z(&ui->openGLWidget->data, (value - ui->openGLWidget->translate_z) *
                                      ui->openGLWidget->normalize_coef / 100);
  ui->openGLWidget->translate_z = value;
  ui->openGLWidget->update();
}

void MainWindow::on_scale_increment_clicked() {
  ui->scaleSlider->setValue(ui->scaleSlider->value() + 10);
}

void MainWindow::on_scale_decrement_clicked() {
  ui->scaleSlider->setValue(ui->scaleSlider->value() - 10);
}
