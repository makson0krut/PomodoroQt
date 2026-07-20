#ifndef TASKSWINDOW_H
#define TASKSWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <vector>

struct Task {
    bool done;
    QDate date;
    QString text;
};

class TasksWindow : public QWidget {
    Q_OBJECT

public:
    TasksWindow(QWidget* parent = nullptr);
    void loadTasks();

    signals:
        void backToMenuRequested();

private slots:
    void onAddClicked();
    void onDeleteClicked();
    void onItemChanged(QListWidgetItem* item);

private:
    void saveTasks();
    void refreshList();
    void sortTasksByDate();
    int categorizeDate(const QDate& date, const QDate& today);
    QString formatTaskDate(const QDate& date);
    void addCategorySection(const QString& title, const QColor& color, int categoryIndex, const QDate& today);

    std::vector<Task> tasks;
    std::vector<int> rowToTaskIndex; // -1 = это заголовок, иначе индекс в tasks

    QListWidget* taskList;
    QLineEdit* taskInput;
    QDateEdit* dateInput;
    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* backButton;
};

#endif // TASKSWINDOW_H