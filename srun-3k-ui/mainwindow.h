#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtNetwork/QNetworkAccessManager"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void GET_INFO_Finished(QNetworkReply *);
    void TimeSlot();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QTimer *meTimer;
};

#endif // MAINWINDOW_H
