#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qcustomplot.h>
#include "nmlib.hpp"
#include <fstream>
#include <qvalidator.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->plot->xAxis->setLabel("T");
    this->ui->plot->yAxis->setLabel("X,Y");

    this->ui->plot->xAxis->setRange(-10, 10);
    this->ui->plot->yAxis->setRange(-7, 7);

    this->ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    func = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_button_plot_clicked()
{
    QVector<double> x, y, v;

    x_begin = this->ui->lineEdit_x_start->text().toDouble();
    x_end = this->ui->lineEdit_x_end->text().toDouble();
    h = this->ui->lineEdit_step->text().toDouble();
    precision = this->ui->lineEdit_precision->text().toDouble();

    for (long long i = 0; i < res1.size(); i++)
    {
        x.push_back(res1.at(i).xi);
        y.push_back(res1.at(i).yi);
        v.push_back(res1.at(i).vi);
    }

    this->ui->plot->addGraph();

    QPen pen;
    pen.setWidth(2);
    pen.setColor(col);
    this->ui->plot->graph(count_plot)->setPen(pen);

    // this->ui->plot->graph(count_plot)->addData(x, y);
    // this->ui->plot->replot();
    // count_plot++;
    this->ui->plot->addGraph();
    pen.setWidth(2);
    pen.setColor(col);
    this->ui->plot->graph(count_plot)->setPen(pen);

    this->ui->plot->graph(count_plot)->addData(x, v);
    this->ui->plot->replot();
    count_plot++;
}


void MainWindow::on_button_clear_clicked()
{
    for (int i = 0; i< count_plot; i++) {
        this->ui->plot->graph(i)->data()->clear();
    }
    count_plot = 0;
    this->ui->plot->replot();
    this->ui->plot->update();

    
}


void MainWindow::on_button_save_points_clicked()
{
    
}



void MainWindow::on_button_plot_from_file_clicked()
{
    std::ofstream os("table.txt");

    os << "x\tu1\tu2\tu1_2\tu2_2\tu1-u1_2\tu2-u2_2\tОЛП\tОЛП/ОЛПП\th"
       << "C1\tC2\tu1-U1\tu2-U2\n";
    dumpTableInFile(res1, os);
}


void MainWindow::on_exit_button_clicked()
{
    QMessageBox::StandardButton exit = QMessageBox::question(this, " ", "you wanna exit?", QMessageBox::Yes | QMessageBox::No);
    if (exit == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}


void MainWindow::on_getdata_buttom_clicked()
{
    x_begin = this->ui->lineEdit_x_start->text().toDouble();
    x_end = this->ui->lineEdit_x_end->text().toDouble();
    h = this->ui->lineEdit_step->text().toDouble();
    precision = this->ui->lineEdit_precision->text().toDouble();
    x_start = this->ui->lineEdit_start_x->text().toDouble();
    y_start = this->ui->lineEdit_start_y->text().toDouble();
    N = this->ui->lineEdit_n->text().toInt();
    A = this->ui->lineEdit_a->text().toDouble();
    B = this->ui->lineEdit_b->text().toDouble();
    C = this->ui->lineEdit_c->text().toDouble();


    config cfg = {x_begin, x_end, x_start, y_start, du, h, N, LEC, precision, A, B, C};

    res1 = task_rk4([&](const double &x, const double &u){return -B * (u - C);}, cfg);

    // switch (func) {
    // case 0:
    //     res1 = task_rk4(test_rhs, cfg);
    //     break;
    // case 1:
    //     static auto task1_rhs1 = [&](double x, double u, double du){ return((-A-B)*u+B*du); };
    //     static auto task1_ths2 = [&](double x, double u, double du){ return(B*u-(-C-B)*du); };
    //     res1 = utils::RK4_SOE(task1_rhs1,task1_ths2,cfg);
    //     break;
    // case 2:
    //     static auto task2_rhs1 = [&](double x, double u, double du){ return(-500.005*u+499.995*du); };
    //     static auto task2_ths2 = [&](double x, double u, double du){ return(499.995*u-500.005*du); };
    //     res1 = utils::RK4_SOE(task2_rhs1,task2_ths2,cfg);
    //     break;
    // default:
    //     break;
    // }

 
    auto [min_u, min_u_x] = find_min_u(res1);
    auto [max_LE, max_LE_x] = find_max_LE(res1);
    auto [max_step, max_step_x] = find_max_h(res1);
    auto [min_step, min_step_x] = find_min_h(res1);
    auto [max_uvi, max_uvi_x] = find_max_uvi(res1);
    long long steps_num = res1.size() - 1;
    long long total_augs = res1.back().C2;
    long long total_dims = res1.back().C1;

   
    this->ui->max_le_txt->setText(QString::number(max_LE));
    this->ui->max_le_txt_x->setText(QString::number(max_LE_x));
    this->ui->C2->setText(QString::number(total_augs));
    this->ui->C1->setText(QString::number(total_dims));

    this->ui->min_u1u2->setText(QString::number(min_u));
    this->ui->min_u1u2_x->setText(QString::number(min_u_x));
    this->ui->max_h_txt->setText(QString::number(max_step));
    this->ui->max_h_txt_x->setText(QString::number(max_step_x));
    this->ui->min_h_txt->setText(QString::number(min_step));
    this->ui->min_h_txt_x->setText(QString::number(min_step_x));
    this->ui->max_glob_txt->setText(QString::number(max_uvi));
    this->ui->max_glob_txt_x->setText(QString::number(max_uvi_x));
    this->ui->max_le_txt->setText(QString::number(max_LE));
    this->ui->max_le_txt_x->setText(QString::number(max_LE_x));
    this->ui->step_num_txt->setText(QString::number(steps_num));
}

//void MainWindow::on_Help_buttom_clicked(){};

void MainWindow::on_radioButton_blue_clicked(bool checked)
{
    if(checked)
    {
        col = QColor(0, 0, 255);
    }
}


void MainWindow::on_radioButton_red_clicked(bool checked)
{
    if(checked)
    {
        col = QColor(255, 0, 0);
    }
}


void MainWindow::on_radioButton_green_clicked(bool checked)
{
    if(checked)
    {
        col = QColor(0, 255, 0);
    }
}


void MainWindow::on_radioButton_violet_clicked(bool checked)
{
    if(checked)
    {
        col = QColor(105, 0, 198);
    }
}


// void MainWindow::on_radioButton_mistake_clicked(bool checked)
// {
//     LEC = checked;
// }


void MainWindow::on_button_table_clicked()
{
    ui->tableWidget->clear();

    ui->tableWidget->setRowCount(res1.size());
    ui->tableWidget->setColumnCount(10);

    std::cout<<res1.size()<<std::endl;

    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"x"<<"u1"<<"u1_2"<<"u1-u1_2"<<"ОЛП"<<"ОЛП/ОЛПП"<<"h"<<"C1"<<"C2"<<"u1-U1");
    
    for (int i = 0; i < res1.size(); i++) {
            auto row_tuple = res1.at(i).get_tuple();
            int j = 0;
            int b = 0;
            apply_elemwise([&](const auto& elem){ 
                if (j == 2 || j == 4 || j == 6 || j == 13) {
                j++;
                b++;
            }
            else {
                QTableWidgetItem *item = new QTableWidgetItem(QString::number(elem));
                ui->tableWidget->setItem(i, j-b, item);
                j++;
                }
            }, 
            row_tuple, 
            std::make_index_sequence<std::tuple_size<decltype(row_tuple)>::value>{});
    }
}


// void MainWindow::on_comboBox_activated(int index)
// {
//     func = index;
// }


void MainWindow::on_HelpButton_clicked()
{
    form.show();
}
