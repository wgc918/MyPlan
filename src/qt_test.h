#pragma once

#include "taskmanager.h"
#include "ui_qt_test.h"
#include <QDate>
#include <QMainWindow>

class QTableWidget;

class qt_test : public QMainWindow {
  Q_OBJECT

public:
  explicit qt_test(QWidget *parent = nullptr);
  ~qt_test();

private slots:
  void onAddDailyTask();
  void onDeleteDailyTask();
  void onEditDailyTask();
  void onAddTodoTask();
  void onDeleteTodoTask();
  void onEditTodoTask();

private:
  enum class StatusFilter { All, Active, Completed };
  enum class DailyViewMode { Today, Tomorrow, History };
  enum class HistoryRange { All, LastWeek, SpecificDate };

  Ui_qt_test *ui;
  TaskManager m_taskManager;
  QString m_dataFilePath;
  QDate m_currentDailyDate;
  DailyViewMode m_dailyViewMode;

  void initDataFile();
  void initConnections();
  void initUi();
  void applyStyle();
  void selectDailyDate(const QDate &date, DailyViewMode mode);
  void refreshDailyTaskTable();
  void refreshHistoryTaskTable();
  void refreshTodoTaskTable();
  QList<Task> filteredTasks(const QList<Task> &tasks, const QString &searchText,
                            StatusFilter status) const;
  QList<Task> historyTasks() const;
  StatusFilter statusFilterFromIndex(int index) const;
  HistoryRange historyRangeFromIndex(int index) const;
  void populateTaskTable(QTableWidget *table, const QList<Task> &tasks,
                         bool daily);
  void populateTodoTaskTable(const QList<Task> &tasks);
  void populateHistoryTaskTable(const QList<Task> &tasks);
  void refreshHistoryDateOptions();
  void updateStats();
  void updateDailyViewLabel();
  void updateHistoryControls();
  void editTask(const QString &taskId);
  bool confirmDelete(const QString &taskTitle);
  bool saveTasks(const QString &message);
  QString taskIdFromSender() const;
  QString taskIdFromRow(QTableWidget *table, int row) const;
};
