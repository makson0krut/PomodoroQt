//
// Created by Максим Сустаев on 21.07.2026.
//

#include "statschartwidget.h"
#include <QPainter>
#include <algorithm>

StatsChartWidget::StatsChartWidget(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(150);
}

void StatsChartWidget::setData(const QVector<int>& counts, const QVector<QString>& labels) {
    dayCounts = counts;
    dayLabels = labels;
    update();
}

void StatsChartWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (dayCounts.isEmpty()) {
        return;
    }

    int maxValue = *std::max_element(dayCounts.begin(), dayCounts.end());
    if (maxValue == 0) {
        maxValue = 1; // избегаем деления на 0, если за все дни 0 циклов
    }

    int barCount = dayCounts.size();
    int chartHeight = height() - 30; // оставляем место снизу под подписи дней
    int barWidth = width() / (barCount * 2); // ширина столбца, с промежутками между ними
    int spacing = barWidth;

    for (int i = 0; i < barCount; ++i) {
        int barHeight = static_cast<int>((static_cast<double>(dayCounts[i]) / maxValue) * chartHeight);
        int x = spacing / 2 + i * (barWidth + spacing);
        int y = chartHeight - barHeight;

        QRect barRect(x, y, barWidth, barHeight);
        painter.setBrush(QColor("#89b4fa"));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(barRect, 4, 4);

        painter.setPen(QColor("#cdd6f4"));
        painter.drawText(QRect(x, chartHeight + 5, barWidth, 20), Qt::AlignCenter, dayLabels[i]);

        if (dayCounts[i] > 0) {
            painter.drawText(QRect(x, y - 20, barWidth, 20), Qt::AlignCenter, QString::number(dayCounts[i]));
        }
    }
}
