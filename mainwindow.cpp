#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include "qcustomplot.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::generate() {
    alpha = ui->doubleSpinBox->value();
    beta = ui->doubleSpinBox_2->value();
    v0 = ui->doubleSpinBox_3->value();
    draw();
}

double MainWindow::getX(double t) {
    return v0 * cos(alpha) * (1 - exp(-beta * t)) / beta;
}

double MainWindow::getY(double t) {
    return (v0 * sin(alpha) + g / beta) * (1 - exp(-beta * t)) / beta - g * t / beta;
}

void MainWindow::draw() {
    //Рисуем график y=x*x

    //Сгенерируем данные
    //Для этого создадим два массива точек:
    //один для созранения x координат точек,
    //а второй для y соответственно

    double a = 0; //Начало интервала, где рисуем график по оси Ox
    double b = 10000; //Конец интервала, где рисуем график по оси Ox
    double h = 0.001; //Шаг, с которым будем пробегать по оси Ox

    QVector<double> x, y; //Массивы координат точек
    //Вычисляем наши данные
    for (double t = a; t <= b; t += h) {
        x.push_back(getX(t));
        y.push_back(getY(t));
        if (y[y.size() - 1] < 0) {
            Lmax = x[x.size() - 1];
            ui->label_4->setText("Lmax = " + QString::number(Lmax));
            break;
        }
        qDebug() << x[x.size() - 1] << y[y.size() - 1];
    }
    alphaBest = alpha;
    double LmaxBest = Lmax;
    alpha = 0.01;
    double deltaAlpha = 0.01;
    while (alpha < pi / 2) {
        for (double t = a; t <= b; t += h) {
            double tmpX = getX(t);
            double tmpY = getY(t);
            if (tmpY < 0) {
                double tmpLmax = tmpX;
                if (tmpLmax > LmaxBest) {
                    LmaxBest = tmpLmax;
                    alphaBest = alpha;
                }
                break;
            }
        }
        alpha += deltaAlpha;
    }
    ui->label_5->setText("alphaBest = " + QString::number(alphaBest));

    ui->graph->clearGraphs();//Если нужно, но очищаем все графики
    //Добавляем один график в widget
//    ui->graph->addGraph();
    //Говорим, что отрисовать нужно график по нашим двум массивам x и y
    double minY = y[0], maxY = y[0];
    for (int i = 1; i < y.size(); i++) {
        if (y[i] < minY)
            minY = y[i];
        if (y[i] > maxY)
            maxY = y[i];
    }
    ui->graph->addGraph();
    ui->graph->graph(0)->setData(x, y);

    ui->graph->xAxis->setRange(a, x[x.size() - 1]);//Для оси Ox
    ui->graph->xAxis->setLabel(QString::fromUtf8("x"));
    ui->graph->yAxis->setLabel(QString::fromUtf8("y"));
/*    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    for (int i = 0; i < year; i++) {
        textTicker->addTick(i, QString::number(i));
    }
    ui->graph->xAxis->setTicker(textTicker);*/
    ui->graph->yAxis->setRange(minY, maxY);//Для оси Oy

    //И перерисуем график на нашем widget
    ui->graph->replot();
}
