#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event);

private slots:
    void Close();
    void on_pushButton_clicked();

    void on_ABOUT_clicked();

    void on_SERVICE_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
