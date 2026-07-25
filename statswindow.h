//
// Created by Максим Сустаев on 19.07.2026.
//

#ifndef QTHELLOWORLD_STATSWINDOW_H
#define QTHELLOWORLD_STATSWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QDate>
#include <vector>
#include <QLabel>
#include "statschartwidget.h"

class StatsWindow : public QWidget {
    Q_OBJECT
public:
    StatsWindow(QWidget *parent = nullptr);
    void loadStats();
signals:
    void backToMenuRequested();
private slots:
    void onClearClicked();
private:
    QPushButton* clearButton;
    QPushButton* backButton;
    QString getStorageDirPath();

    //Парсинг статистики
    std::vector<QDate> parseStatsFile();

    QLabel* todayCountLabel;
    QLabel* weekCountLabel;
    QLabel* totalCountLabel;

    StatsChartWidget* chartWidget;
};


#endif //QTHELLOWORLD_STATSWINDOW_H
