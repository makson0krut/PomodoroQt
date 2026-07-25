//
// Created by Максим Сустаев on 19.07.2026.
//

#include "statswindow.h"
#include <QVBoxLayout>
#include <fstream>
#include <sstream>
#include <QCoreApplication>
#include <QDir>
#include <QDate>
#include <vector>

StatsWindow::StatsWindow(QWidget* parent) : QWidget(parent) {
    todayCountLabel = new QLabel(this);
    todayCountLabel->setObjectName("statsCounter");

    weekCountLabel = new QLabel(this);
    weekCountLabel->setObjectName("statsCounter");

    totalCountLabel = new QLabel(this);
    totalCountLabel->setObjectName("statsCounter");

    chartWidget = new StatsChartWidget(this);

    clearButton = new QPushButton("Очистить статистику", this);
    backButton = new QPushButton("В меню", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    layout->addWidget(todayCountLabel);
    layout->addWidget(weekCountLabel);
    layout->addWidget(totalCountLabel);
    layout->addSpacing(10);
    layout->addWidget(chartWidget);
    layout->addSpacing(10);
    layout->addWidget(clearButton);
    layout->addWidget(backButton);

    setLayout(layout);

    connect(clearButton, &QPushButton::clicked, this, &StatsWindow::onClearClicked);
    connect(backButton, &QPushButton::clicked, this, &StatsWindow::backToMenuRequested);
}
void StatsWindow::loadStats() {
    std::vector<QDate> dates = parseStatsFile();

    QDate today = QDate::currentDate();
    int todayCount = 0;
    int weekCount = 0;
    int totalCount = static_cast<int>(dates.size());

    QVector<int> dayCounts(7, 0);
    QVector<QString> dayLabels;
    static const QString shortDayNames[] = {"Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс"};

    for (int i = 0; i < 7; ++i) {
        QDate day = today.addDays(-6 + i);
        dayLabels.append(shortDayNames[day.dayOfWeek() - 1]);
    }

    for (const QDate& date : dates) {
        if (date == today) {
            todayCount++;
        }
        if (date >= today.addDays(-6) && date <= today) {
            weekCount++;
            int dayIndex = static_cast<int>(date.daysTo(today));
            dayCounts[6 - dayIndex]++;
        }
    }

    todayCountLabel->setText("Сегодня: " + QString::number(todayCount));
    weekCountLabel->setText("За неделю: " + QString::number(weekCount));
    totalCountLabel->setText("Всего: " + QString::number(totalCount));

    chartWidget->setData(dayCounts, dayLabels);
}
void StatsWindow::onClearClicked() {
    std::string path = getStorageDirPath().toStdString() + "/pomodoro_stats.txt";
    std::ofstream file(path, std::ios::trunc);
    file.close();
    loadStats();
}
QString StatsWindow::getStorageDirPath() {
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    return dir.absolutePath();
}
std::vector<QDate> StatsWindow::parseStatsFile() {
    std::vector<QDate> dates;
    std::string path = getStorageDirPath().toStdString() + "/pomodoro_stats.txt";
    std::ifstream file(path);

    if (!file.is_open()) {
        return dates;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        QDate date = QDate::fromString(QString::fromStdString(line), "yyyy-MM-dd");
        if (date.isValid()) {
            dates.push_back(date);
        }
    }

    file.close();
    return dates;
}