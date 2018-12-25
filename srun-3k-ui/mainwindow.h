#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include "network.h"
#include "version.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    Ui::MainWindow *ui;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private slots:
    void on_showMainAction();
    void Close();
    void Min();
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void on_checkUpdateButton_clicked();
    void anchorClickedSlot(const QUrl &url);

private:
    bool mDrag;
    QPoint mDragPos;
    QRect mLocation;
    QFont mIconFont;
    QPushButton *closeButton;
    QPushButton *minButton;
    QSystemTrayIcon *mSysTrayIcon;
    QMenu *mMenu=nullptr;
    QAction *mShowMainAction;
    QAction *mExitAppAction;
    QAction *mServiceAction;
    QAction *mAboutAction;
    QPushButton *AboutButton;
    QPushButton *AdvancedButton;
    network *n;
};

#endif // MAINWINDOW_H
