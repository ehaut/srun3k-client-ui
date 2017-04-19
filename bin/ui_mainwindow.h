/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *TOP;
    QLabel *LNAME;
    QLabel *LPASSWD;
    QLineEdit *INPUT_NAME;
    QLineEdit *INPUT_PASSWD;
    QPushButton *LOGIN;
    QPushButton *LOGOUT;
    QPushButton *INFO;
    QCheckBox *AUTO_LOGIN;
    QCheckBox *AUTO_START;
    QPushButton *GET_MESSAGE;
    QLabel *GITHUB;
    QLabel *WELCOME;
    QLabel *label;
    QFrame *line_2;
    QFrame *line;
    QFrame *line_3;
    QLabel *SERVICE;
    QLabel *label_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->setEnabled(true);
        MainWindow->resize(380, 220);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(380, 220));
        MainWindow->setMaximumSize(QSize(380, 220));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        TOP = new QLabel(centralWidget);
        TOP->setObjectName(QStringLiteral("TOP"));
        TOP->setEnabled(true);
        TOP->setGeometry(QRect(80, 0, 381, 51));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(TOP->sizePolicy().hasHeightForWidth());
        TOP->setSizePolicy(sizePolicy1);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(28);
        font.setBold(false);
        font.setWeight(50);
        TOP->setFont(font);
        LNAME = new QLabel(centralWidget);
        LNAME->setObjectName(QStringLiteral("LNAME"));
        LNAME->setGeometry(QRect(20, 80, 70, 25));
        sizePolicy1.setHeightForWidth(LNAME->sizePolicy().hasHeightForWidth());
        LNAME->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setFamily(QString::fromUtf8("\346\226\260\345\256\213\344\275\223"));
        font1.setPointSize(14);
        LNAME->setFont(font1);
        LPASSWD = new QLabel(centralWidget);
        LPASSWD->setObjectName(QStringLiteral("LPASSWD"));
        LPASSWD->setGeometry(QRect(20, 120, 70, 25));
        sizePolicy1.setHeightForWidth(LPASSWD->sizePolicy().hasHeightForWidth());
        LPASSWD->setSizePolicy(sizePolicy1);
        LPASSWD->setFont(font1);
        INPUT_NAME = new QLineEdit(centralWidget);
        INPUT_NAME->setObjectName(QStringLiteral("INPUT_NAME"));
        INPUT_NAME->setGeometry(QRect(100, 80, 161, 25));
        INPUT_PASSWD = new QLineEdit(centralWidget);
        INPUT_PASSWD->setObjectName(QStringLiteral("INPUT_PASSWD"));
        INPUT_PASSWD->setGeometry(QRect(100, 120, 161, 25));
        INPUT_PASSWD->setEchoMode(QLineEdit::Password);
        LOGIN = new QPushButton(centralWidget);
        LOGIN->setObjectName(QStringLiteral("LOGIN"));
        LOGIN->setGeometry(QRect(20, 160, 75, 25));
        LOGOUT = new QPushButton(centralWidget);
        LOGOUT->setObjectName(QStringLiteral("LOGOUT"));
        LOGOUT->setGeometry(QRect(110, 160, 75, 25));
        INFO = new QPushButton(centralWidget);
        INFO->setObjectName(QStringLiteral("INFO"));
        INFO->setGeometry(QRect(200, 160, 75, 25));
        AUTO_LOGIN = new QCheckBox(centralWidget);
        AUTO_LOGIN->setObjectName(QStringLiteral("AUTO_LOGIN"));
        AUTO_LOGIN->setGeometry(QRect(290, 100, 80, 25));
        sizePolicy1.setHeightForWidth(AUTO_LOGIN->sizePolicy().hasHeightForWidth());
        AUTO_LOGIN->setSizePolicy(sizePolicy1);
        AUTO_START = new QCheckBox(centralWidget);
        AUTO_START->setObjectName(QStringLiteral("AUTO_START"));
        AUTO_START->setGeometry(QRect(290, 125, 80, 25));
        sizePolicy1.setHeightForWidth(AUTO_START->sizePolicy().hasHeightForWidth());
        AUTO_START->setSizePolicy(sizePolicy1);
        GET_MESSAGE = new QPushButton(centralWidget);
        GET_MESSAGE->setObjectName(QStringLiteral("GET_MESSAGE"));
        GET_MESSAGE->setGeometry(QRect(290, 160, 75, 25));
        GITHUB = new QLabel(centralWidget);
        GITHUB->setObjectName(QStringLiteral("GITHUB"));
        GITHUB->setGeometry(QRect(350, 200, 31, 16));
        sizePolicy1.setHeightForWidth(GITHUB->sizePolicy().hasHeightForWidth());
        GITHUB->setSizePolicy(sizePolicy1);
        GITHUB->setOpenExternalLinks(true);
        WELCOME = new QLabel(centralWidget);
        WELCOME->setObjectName(QStringLiteral("WELCOME"));
        WELCOME->setGeometry(QRect(10, 200, 131, 16));
        sizePolicy1.setHeightForWidth(WELCOME->sizePolicy().hasHeightForWidth());
        WELCOME->setSizePolicy(sizePolicy1);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(120, 50, 381, 21));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(0, 190, 381, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(260, 80, 21, 65));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(319, 200, 41, 15));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        SERVICE = new QLabel(centralWidget);
        SERVICE->setObjectName(QStringLiteral("SERVICE"));
        SERVICE->setGeometry(QRect(290, 200, 41, 16));
        sizePolicy1.setHeightForWidth(SERVICE->sizePolicy().hasHeightForWidth());
        SERVICE->setSizePolicy(sizePolicy1);
        SERVICE->setOpenExternalLinks(true);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(280, 80, 91, 16));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        TOP->setText(QApplication::translate("MainWindow", "\346\240\241\345\233\255\347\275\221\347\231\273\351\231\206\345\231\250", Q_NULLPTR));
        LNAME->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267\345\220\215:", Q_NULLPTR));
        LPASSWD->setText(QApplication::translate("MainWindow", "\345\257\206  \347\240\201:", Q_NULLPTR));
        INPUT_NAME->setPlaceholderText(QApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\347\224\250\346\210\267\345\220\215...", Q_NULLPTR));
        INPUT_PASSWD->setPlaceholderText(QApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\345\257\206\347\240\201...", Q_NULLPTR));
        LOGIN->setText(QApplication::translate("MainWindow", "\347\231\273\351\231\206", Q_NULLPTR));
        LOGOUT->setText(QApplication::translate("MainWindow", "\346\263\250\351\224\200", Q_NULLPTR));
        INFO->setText(QApplication::translate("MainWindow", "\347\231\273\351\231\206\347\212\266\346\200\201", Q_NULLPTR));
        AUTO_LOGIN->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\347\231\273\351\231\206", Q_NULLPTR));
        AUTO_START->setText(QApplication::translate("MainWindow", "\345\274\200\346\234\272\350\207\252\345\220\257", Q_NULLPTR));
        GET_MESSAGE->setText(QApplication::translate("MainWindow", "\350\216\267\345\217\226\345\205\254\345\221\212", Q_NULLPTR));
        GITHUB->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"https://github.com/CHN-STUDENT/SRUN-3K-UI\"><span style=\" text-decoration: underline; color:#0000ff;\">\345\205\263\344\272\216</span></a></p></body></html>", Q_NULLPTR));
        WELCOME->setText(QApplication::translate("MainWindow", "\346\254\242\350\277\216\344\275\277\347\224\250\346\240\241\345\233\255\347\275\221\347\231\273\351\231\206\345\231\250!", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Made by CHN-STUDENT V0.0.3", Q_NULLPTR));
        SERVICE->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://172.16.154.130:8800/\"><span style=\" text-decoration: underline; color:#0000ff;\">\350\207\252\346\234\215\345\212\241</span></a></p></body></html>", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\347\231\273\351\231\206\346\210\220\345\212\237\345\220\216\347\224\237\346\225\210:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
