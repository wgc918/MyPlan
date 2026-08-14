#include "task.h"
#include <QJsonObject>
#include <QUuid>

QJsonObject Task::toJson() const {
  QJsonObject json;
  json["id"] = id;
  json["title"] = title;
  json["description"] = description;
  json["date"] = date.toString(Qt::ISODate);
  json["dueDate"] = dueDate.toString(Qt::ISODate);
  json["createdAt"] = createdAt.toString(Qt::ISODate);
  json["priority"] = priority;
  json["completed"] = completed;
  json["isDaily"] = isDaily;
  return json;
}

Task Task::fromJson(const QJsonObject &json) {
  Task task;
  task.id = json["id"].toString();
  if (task.id.isEmpty()) {
    task.id = QUuid::createUuid().toString();
  }
  task.title = json["title"].toString();
  task.description = json["description"].toString();
  task.date = QDate::fromString(json["date"].toString(), Qt::ISODate);
  task.dueDate = QDate::fromString(json["dueDate"].toString(), Qt::ISODate);
  task.createdAt = QDate::fromString(json["createdAt"].toString(), Qt::ISODate);
  if (!task.createdAt.isValid()) {
    task.createdAt = QDate::currentDate();
  }
  task.priority = json["priority"].toInt(3);
  task.completed = json["completed"].toBool(false);
  task.isDaily = json["isDaily"].toBool(false);
  return task;
}
