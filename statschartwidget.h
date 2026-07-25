//
// Created by Максим Сустаев on 21.07.2026.
//

#ifndef POMODOROQT_STATSCHARTWIDGET_H
#define POMODOROQT_STATSCHARTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QString>

class StatsChartWidget : public QWidget {
    Q_OBJECT

public:
    StatsChartWidget(QWidget* parent = nullptr);
    void setData(const QVector<int>& counts, const QVector<QString>& labels);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<int> dayCounts;
    QVector<QString> dayLabels;
};

#endif //POMODOROQT_STATSCHARTWIDGET_H
