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
private slots:
    void on_LOGIN_clicked();

    void on_LOGOUT_clicked();

    void on_INFO_clicked();

    void on_GET_MESSAGE_clicked();

    void on_AUTO_START_clicked();

    void on_AUTO_LOGIN_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                /
