//
// Created by Максим Сустаев on 16.07.2026.
//

#include "mainmenuwindow.h"

#include <QVBoxLayout>
#include <QMessageBox>

MainMenuWindow::MainMenuWindow(QWidget* parent) : QWidget(parent) {
    titleLabel = new QLabel("Pomodoro", this);
    titleLabel->setObjectName("titleLabel");

    timerButton = new QPushButton("Таймер", this);
    timerButton->setObjectName("primaryButton");
    tasksButton = new QPushButton("Задачи", this);
    statsButton = new QPushButton("Статистика", this);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 40, 30, 30);
    layout->setSpacing(15);
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addWidget(timerButton);
    layout->addWidget(tasksButton);
    layout->addWidget(statsButton);
    layout->addSpacing(20);
    layout->addWidget(volumeSlider);
    setLayout(layout);

    connect(timerButton, &QPushButton::clicked, this, &MainMenuWindow::timerButtonClicked);
    connect(statsButton, &QPushButton::clicked, this, &MainMenuWindow::statsButtonClicked);
    connect(tasksButton, &QPushButton::clicked, this, &MainMenuWindow::tasksButtonClicked);
    connect(volumeSlider, &QSlider::valueChanged, this, &MainMenuWindow::onVolumeSliderChanged);
}

void MainMenuWindow::onStubButtonClicked() {
    QMessageBox::information(this, "В разработке", "Эта функция еще не реализовона.");
}
void MainMenuWindow::onVolumeSliderChanged(int value) {
    emit volumeChanged(value);
}