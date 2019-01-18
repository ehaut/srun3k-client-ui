#include "radiusprogressbar.h"
#include <QPainter>

RadiusProgressBar::RadiusProgressBar(QWidget *parent) : QProgressBar(parent)
{
    setMinimum(0);
    setMaximum(100);
    setValue(0);
}

void RadiusProgressBar::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QRect rect = QRect(0, 0, width(), height());
    QRect textRect = QRect(0, 0, width(), height());

    const double k = (double)(value() - minimum()) / (maximum()-minimum());
    int x = (int)(rect.width() * k);
    QRect fillRect = rect.adjusted(0, 0, x-rect.width(), 0);

    QString valueStr = QString("正在更新中...%1 %").arg(QString::number(value()));
    QPixmap buttomMap = QPixmap(":/png/radius_back");
    QPixmap fillMap = QPixmap(":/png/radius_front");

    //画进度条
    p.drawPixmap(rect, buttomMap);
    p.drawPixmap(fillRect, fillMap, fillRect);

    //画文字
    QFont f = QFont("Microsoft YaHei",12, QFont::Normal);
    p.setFont(f);
    p.setPen(QColor("#ffffff"));
    p.drawText(textRect, Qt::AlignCenter, valueStr);
}

