//
// Created by Максим Сустаев on 19.07.2026.
//

#ifndef QTHELLOWORLD_CIRCULARTIMERWIDGET_H
#define QTHELLOWORLD_CIRCULARTIMERWIDGET_H
#include <QWidget>


class CircularTimerWidget : public QWidget {
    Q_OBJECT
public:
    CircularTimerWidget(QWidget* parent = nullptr);
    void setTime(int remaining, int total);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    double calculateProgress() const;

    int remainingSeconds;
    int totalSeconds;
};


#endif //QTHELLOWORLD_CIRCULARTIMERWIDGET_H
