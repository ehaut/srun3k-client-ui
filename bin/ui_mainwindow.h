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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
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
    QSpinBox *ACID;
    QLabel *LACID;
    QLabel *ME;
    QCheckBox *SAVE_INFO;
    QCheckBox *AUTO_LOGIN;
    QCheckBox *checkBox;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(400, 280);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        TOP = new QLabel(centralWidget);
        TOP->setObjectName(QStringLiteral("TOP"));
        TOP->setEnabled(true);
        TOP->setGeometry(QRect(100, 10, 211, 61));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(26);
        font.setBold(false);
        font.setWeight(50);
        TOP->setFont(font);
        LNAME = new QLabel(centralWidget);
        LNAME->setObjectName(QStringLiteral("LNAME"));
        LNAME->setGeometry(QRect(40, 90, 70, 25));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\346\226\260\345\256\213\344\275\223"));
        font1.setPointSize(14);
        LNAME->setFont(font1);
        LPASSWD = new QLabel(centralWidget);
        LPASSWD->setObjectName(QStringLiteral("LPASSWD"));
        LPASSWD->setGeometry(QRect(40, 130, 70, 25));
        LPASSWD->setFont(font1);
        INPUT_NAME = new QLineEdit(centralWidget);
        INPUT_NAME->setObjectName(QStringLiteral("INPUT_NAME"));
        INPUT_NAME->setGeometry(QRect(130, 90, 211, 25));
        INPUT_PASSWD = new QLineEdit(centralWidget);
        INPUT_PASSWD->setObjectName(QStringLiteral("INPUT_PASSWD"));
        INPUT_PASSWD->setGeometry(QRect(130, 130, 211, 25));
        INPUT_PASSWD->setEchoMode(QLineEdit::Password);
        LOGIN = new QPushButton(centralWidget);
        LOGIN->setObjectName(QStringLiteral("LOGIN"));
        LOGIN->setGeometry(QRect(40, 230, 80, 30));
        LOGOUT = new QPushButton(centralWidget);
        LOGOUT->setObjectName(QStringLiteral("LOGOUT"));
        LOGOUT->setGeometry(QRect(149, 230, 80, 30));
        INFO = new QPushButton(centralWidget);
        INFO->setObjectName(QStringLiteral("INFO"));
        INFO->setGeometry(QRect(260, 230, 80, 30));
        ACID = new QSpinBox(centralWidget);
        ACID->setObjectName(QStringLiteral("ACID"));
        ACID->setGeometry(QRect(130, 170, 42, 25));
        ACID->setMinimum(1);
        ACID->setMaximum(2);
        LACID = new QLabel(centralWidget);
        LACID->setObjectName(QStringLiteral("LACID"));
        LACID->setGeometry(QRect(44, 170, 70, 25));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\346\226\260\345\256\213\344\275\223"));
        font2.setPointSize(12);
        LACID->setFont(font2);
        ME = new QLabel(centralWidget);
        ME->setObjectName(QStringLiteral("ME"));
        ME->setGeometry(QRect(130, 66, 381, 20));
        SAVE_INFO = new QCheckBox(centralWidget);
        SAVE_INFO->setObjectName(QStringLiteral("SAVE_INFO"));
        SAVE_INFO->setGeometry(QRect(60, 200, 70, 25));
        AUTO_LOGIN = new QCheckBox(centralWidget);
        AUTO_LOGIN->setObjectName(QStringLiteral("AUTO_LOGIN"));
        AUTO_LOGIN->setGeometry(QRect(160, 200, 70, 25));
        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(260, 200, 70, 25));
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
        INPUT_NAME->setPlaceholderText(QApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\347\224\250\346\210\267\345\220\215", Q_NULLPTR));
        INPUT_PASSWD->setPlaceholderText(QApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\345\257\206\347\240\201", Q_NULLPTR));
        LOGIN->setText(QApplication::translate("MainWindow", "\347\231\273\351\231\206", Q_NULLPTR));
        LOGOUT->setText(QApplication::translate("MainWindow", "\346\263\250\351\224\200", Q_NULLPTR));
        INFO->setText(QApplication::translate("MainWindow", "\350\216\267\345\217\226\347\212\266\346\200\201", Q_NULLPTR));
        LACID->setText(QApplication::translate("MainWindow", "A C I D:", Q_NULLPTR));
        ME->setText(QApplication::translate("MainWindow", "Made by CHN-STUDENT V0.0.2", Q_NULLPTR));
        SAVE_INFO->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\351\205\215\347\275\256", Q_NULLPTR));
        AUTO_LOGIN->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\347\231\273\351\231\206", Q_NULLPTR));
        checkBox->setText(QApplication::translate("MainWindow", "\345\274\200\346\234\272\350\207\252\345\220\257", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
