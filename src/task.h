#pragma once

#include <QDate>
#include <QJsonObject>
#include <QString>

struct Task {
  QString id;
  QString title;
  QString description;
  QDate date;
  QDate dueDate;
  QDate createdAt;
  int priority;
  bool completed;
  bool isDaily;

  Task()
      : priority(3), completed(false), isDaily(false),
        createdAt(QDate::currentDate()) {}

  QJsonObject toJson() const;
  static Task fromJson(const QJsonObject &json);
};
