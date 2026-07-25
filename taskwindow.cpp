#include "taskwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <fstream>
#include <algorithm>
#include <QFont>
#include <QCoreApplication>
#include <QDir>

TasksWindow::TasksWindow(QWidget* parent) : QWidget(parent) {
    taskList = new QListWidget(this);

    taskInput = new QLineEdit(this);
    taskInput->setPlaceholderText("Новая задача...");
    taskInput->setMinimumWidth(200);

    dateInput = new QDateEdit(this);
    dateInput->setDate(QDate::currentDate());
    dateInput->setCalendarPopup(true);
    dateInput->setFixedWidth(130);

    addButton = new QPushButton("Добавить", this);
    addButton->setFixedWidth(120);

    deleteButton = new QPushButton("Удалить", this);
    backButton = new QPushButton("В меню", this);

    QVBoxLayout* inputLayout = new QVBoxLayout();
    inputLayout->setSpacing(10);
    inputLayout->addWidget(taskInput);

    QHBoxLayout* dateAndButtonLayout = new QHBoxLayout();
    dateAndButtonLayout->setSpacing(10);
    dateAndButtonLayout->addWidget(dateInput);
    dateAndButtonLayout->addWidget(addButton);

    inputLayout->addLayout(dateAndButtonLayout);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);
    layout->addWidget(taskList);
    layout->addLayout(inputLayout);
    layout->addWidget(deleteButton);
    layout->addWidget(backButton);
    inputLayout->addLayout(dateAndButtonLayout);
    setLayout(layout);

    connect(addButton, &QPushButton::clicked, this, &TasksWindow::onAddClicked);
    connect(deleteButton, &QPushButton::clicked, this, &TasksWindow::onDeleteClicked);
    connect(taskList, &QListWidget::itemChanged, this, &TasksWindow::onItemChanged);
    connect(backButton, &QPushButton::clicked, this, &TasksWindow::backToMenuRequested);
}

void TasksWindow::sortTasksByDate() {
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.date < b.date;
    });
}

int TasksWindow::categorizeDate(const QDate& date, const QDate& today) {
    if (date <= today) return 0;
    if (date == today.addDays(1)) return 1;
    if (date <= today.addDays(7)) return 2;
    return 3;
}

QString TasksWindow::formatTaskDate(const QDate& date) {
    static const QString months[] = {
        "янв", "фев", "мар", "апр", "мая", "июн",
        "июл", "авг", "сен", "окт", "ноя", "дек"
    };
    return QString::number(date.day()) + " " + months[date.month() - 1];
}

void TasksWindow::addCategorySection(const QString& title, const QColor& color, int categoryIndex, const QDate& today) {
    bool hasAnyTask = false;
    for (const Task& task : tasks) {
        if (categorizeDate(task.date, today) == categoryIndex) {
            hasAnyTask = true;
            break;
        }
    }

    if (!hasAnyTask) {
        return; // не показываем заголовок, если в категории нет задач
    }

    QListWidgetItem* header = new QListWidgetItem(title, taskList);
    header->setFlags(header->flags() & ~Qt::ItemIsSelectable);
    header->setForeground(color);
    QFont headerFont = header->font();
    headerFont.setBold(true);
    headerFont.setPointSize(14);
    header->setFont(headerFont);
    rowToTaskIndex.push_back(-1);

    for (int i = 0; i < static_cast<int>(tasks.size()); ++i) {
        if (categorizeDate(tasks[i].date, today) != categoryIndex) continue;

        QListWidgetItem* item = new QListWidgetItem("   " + tasks[i].text, taskList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(tasks[i].done ? Qt::Checked : Qt::Unchecked);

        QFont itemFont = item->font();
        itemFont.setPointSize(16);
        item->setFont(itemFont);

        rowToTaskIndex.push_back(i);
    }
}

void TasksWindow::refreshList() {
    taskList->blockSignals(true);
    taskList->clear();
    rowToTaskIndex.clear();

    QDate today = QDate::currentDate();

    addCategorySection("СЕГОДНЯ", QColor("#89dceb"), 0, today);
    addCategorySection("ЗАВТРА", QColor("#89b4fa"), 1, today);
    addCategorySection("НА НЕДЕЛЕ", QColor("#9399b2"), 2, today);
    addCategorySection("ПОТОМ", QColor("#6c7086"), 3, today);

    taskList->blockSignals(false);
}

void TasksWindow::onAddClicked() {
    QString text = taskInput->text();
    if (text.isEmpty()) {
        return;
    }

    Task newTask;
    newTask.done = false;
    newTask.date = dateInput->date();
    newTask.text = text;

    tasks.push_back(newTask);
    sortTasksByDate();
    refreshList();
    saveTasks();

    taskInput->clear();
}

void TasksWindow::onDeleteClicked() {
    int row = taskList->currentRow();
    if (row < 0 || row >= static_cast<int>(rowToTaskIndex.size())) return;

    int taskIndex = rowToTaskIndex[row];
    if (taskIndex == -1) return;

    tasks.erase(tasks.begin() + taskIndex);
    refreshList();
    saveTasks();
}

void TasksWindow::onItemChanged(QListWidgetItem* item) {
    int row = taskList->row(item);
    if (row < 0 || row >= static_cast<int>(rowToTaskIndex.size())) return;

    int taskIndex = rowToTaskIndex[row];
    if (taskIndex == -1) return;

    tasks[taskIndex].done = (item->checkState() == Qt::Checked);
    refreshList();
    saveTasks();
}

void TasksWindow::saveTasks() {
    std::string path = getStorageDirPath().toStdString() + "/pomodoro_tasks.txt";
    std::ofstream file(path, std::ios::trunc);
    if (file.is_open()) {
        for (const Task& task : tasks) {
            int done = task.done ? 1 : 0;
            std::string dateStr = task.date.toString("yyyy-MM-dd").toStdString();
            file << done << "|" << dateStr << "|" << task.text.toStdString() << "\n";
        }
        file.close();
    }
}

void TasksWindow::loadTasks() {
    tasks.clear();
    std::string path = getStorageDirPath().toStdString() + "/pomodoro_tasks.txt";
    std::ifstream file(path);

    if (!file.is_open()) {
        refreshList();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        size_t firstSep = line.find('|');
        size_t secondSep = line.find('|', firstSep + 1);
        if (firstSep == std::string::npos || secondSep == std::string::npos) {
            continue;
        }

        std::string donePart = line.substr(0, firstSep);
        std::string datePart = line.substr(firstSep + 1, secondSep - firstSep - 1);
        std::string textPart = line.substr(secondSep + 1);

        Task task;
        task.done = (donePart == "1");
        task.date = QDate::fromString(QString::fromStdString(datePart), "yyyy-MM-dd");
        task.text = QString::fromStdString(textPart);

        tasks.push_back(task);
    }

    file.close();
    sortTasksByDate();
    refreshList();
}
QString TasksWindow::getStorageDirPath() {
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp(); // из .../PomodoroQt.app/Contents/MacOS в .../PomodoroQt.app/Contents
    dir.cdUp(); // в .../PomodoroQt.app
    dir.cdUp(); // в папку, СОДЕРЖАЩУЮ .app — то, что нам нужно
    return dir.absolutePath();
}