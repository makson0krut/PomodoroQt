//
// Created by Максим Сустаев on 19.07.2026.
//

#include "circulartimerwidget.h"
#include <QPainter>

CircularTimerWidget::CircularTimerWidget(QWidget* parent) : QWidget(parent) {
    remainingSeconds = 0;
    totalSeconds = 1; //Для избежания деления на ноль
}

void CircularTimerWidget::setTime(int remaining, int total) {
    remainingSeconds = remaining;
    totalSeconds = total;
    update();
}
double CircularTimerWidget::calculateProgress()  const {
    if (totalSeconds <= 0) return 0.0;
    return static_cast<double>(remainingSeconds) / totalSeconds;
}
void CircularTimerWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    int diameter = side - 20;
    QRect square((width() - diameter) / 2, (height() - diameter) / 2, diameter, diameter);

    //Фоновое кольцо
    QPen backgroundPen(QColor("#313244"), 12);
    painter.setPen(backgroundPen);
    painter.drawArc(square, 0, 360 * 16);

    //Цыетная дуга
    QPen progressPen(QColor("#f9e2af"), 12);
    progressPen.setCapStyle(Qt::RoundCap);
    painter.setPen(progressPen);
    int angle = static_cast<int>(calculateProgress() * 360 * 16);
    painter.drawArc(square, 90 * 16, -angle);
}