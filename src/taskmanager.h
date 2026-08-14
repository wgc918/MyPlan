#pragma once

#include "task.h"
#include <QList>
#include <QString>

class TaskManager {
public:
  TaskManager();
  ~TaskManager();

  bool loadFromFile(const QString &filePath);
  bool saveToFile(const QString &filePath);

  QList<Task> getDailyTasks(const QDate &date) const;
  QList<Task> getTodayTasks() const;
  QList<Task> getTodoTasks(bool includeCompleted = true) const;
  QList<Task> getRecentTasks() const;
  QList<Task> getAllTasks() const { return m_tasks; }
  QList<QDate> getAllDates() const;
  Task getTask(const QString &taskId) const;

  void addTask(const Task &task);
  void removeTask(const QString &taskId);
  void updateTask(const Task &task);
  void markCompleted(const QString &taskId, bool completed);

private:
  QList<Task> m_tasks;
  QString m_currentFilePath;

  Task *findTask(const QString &taskId);
  const Task *findTask(const QString &taskId) const;
};
