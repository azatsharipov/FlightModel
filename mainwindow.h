#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    double alpha = 1, beta = 1, v0 = 1, Lmax = 0, alphaBest = 0, g = 9.81, pi = 3.14;

    double getX(double t);
    double getY(double t);
    void draw();

public slots:
    void generate();
};

#endif // MAINWINDOW_H
