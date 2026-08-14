#include "taskmanager.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <algorithm>

TaskManager::TaskManager() {}

TaskManager::~TaskManager() {}

bool TaskManager::loadFromFile(const QString &filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    m_currentFilePath = filePath;
    return false;
  }

  QJsonParseError parseError;
  QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
  file.close();

  if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
    m_currentFilePath = filePath;
    return false;
  }

  m_tasks.clear();
  const QJsonArray array = doc.array();
  for (const QJsonValue &value : array) {
    if (value.isObject()) {
      m_tasks.append(Task::fromJson(value.toObject()));
    }
  }

  m_currentFilePath = filePath;
  return true;
}

bool TaskManager::saveToFile(const QString &filePath) {
  QJsonArray array;
  for (const Task &task : m_tasks) {
    array.append(task.toJson());
  }

  QFile file(filePath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    return false;
  }

  QJsonDocument doc(array);
  file.write(doc.toJson(QJsonDocument::Indented));
  file.close();
  m_currentFilePath = filePath;
  return true;
}

QList<Task> TaskManager::getDailyTasks(const QDate &date) const {
  QList<Task> result;
  for (const Task &task : m_tasks) {
    if (task.isDaily && task.date == date) {
      result.append(task);
    }
  }

  std::sort(result.begin(), result.end(), [](const Task &a, const Task &b) {
    if (a.completed != b.completed) {
      return !a.completed;
    }
    if (a.dueDate.isValid() != b.dueDate.isValid()) {
      return a.dueDate.isValid();
    }
    if (a.dueDate != b.dueDate) {
      return a.dueDate > b.dueDate;
    }
    return a.createdAt > b.createdAt;
  });
  return result;
}

QList<Task> TaskManager::getTodayTasks() const {
  return getDailyTasks(QDate::currentDate());
}

QList<Task> TaskManager::getTodoTasks(bool includeCompleted) const {
  QList<Task> result;
  for (const Task &task : m_tasks) {
    if (!task.isDaily && (includeCompleted || !task.completed)) {
      result.append(task);
    }
  }

  std::sort(result.begin(), result.end(), [](const Task &a, const Task &b) {
    if (a.completed != b.completed) {
      return !a.completed;
    }
    if (a.dueDate.isValid() != b.dueDate.isValid()) {
      return a.dueDate.isValid();
    }
    if (a.dueDate != b.dueDate) {
      return a.dueDate < b.dueDate;
    }
    return a.createdAt > b.createdAt;
  });
  return result;
}

QList<Task> TaskManager::getRecentTasks() const { return getTodoTasks(false); }

Task TaskManager::getTask(const QString &taskId) const {
  const Task *task = findTask(taskId);
  return task ? *task : Task();
}

void TaskManager::addTask(const Task &task) { m_tasks.append(task); }

void TaskManager::removeTask(const QString &taskId) {
  for (int i = 0; i < m_tasks.size(); ++i) {
    if (m_tasks[i].id == taskId) {
      m_tasks.removeAt(i);
      break;
    }
  }
}

void TaskManager::updateTask(const Task &task) {
  Task *taskPtr = findTask(task.id);
  if (taskPtr) {
    *taskPtr = task;
  }
}

void TaskManager::markCompleted(const QString &taskId, bool completed) {
  Task *taskPtr = findTask(taskId);
  if (taskPtr) {
    taskPtr->completed = completed;
  }
}

QList<QDate> TaskManager::getAllDates() const {
  QList<QDate> dates;
  for (const Task &task : m_tasks) {
    if (task.isDaily && task.date.isValid() && !dates.contains(task.date)) {
      dates.append(task.date);
    }
  }
  std::sort(dates.begin(), dates.end(), std::greater<QDate>());
  return dates;
}

Task *TaskManager::findTask(const QString &taskId) {
  for (Task &task : m_tasks) {
    if (task.id == taskId) {
      return &task;
    }
  }
  return nullptr;
}

const Task *TaskManager::findTask(const QString &taskId) const {
  for (const Task &task : m_tasks) {
    if (task.id == taskId) {
      return &task;
    }
  }
  return nullptr;
}
