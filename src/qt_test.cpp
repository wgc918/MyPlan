#include "qt_test.h"
#include <QApplication>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QStandardPaths>
#include <QTableWidget>
#include <QUuid>
#include <QVBoxLayout>
#include <algorithm>

namespace {
QString priorityText(int priority) { return QString("P%1").arg(priority); }

QString descriptionPreview(const QString &description) {
  return description.trimmed().isEmpty() ? QStringLiteral("双击添加描述")
                                         : description.simplified();
}

QString dueDateText(const QDate &date) {
  return date.isValid() ? date.toString("yyyy-MM-dd")
                        : QStringLiteral("未设置");
}

QCalendarWidget *makeCalendar(QWidget *parent) {
  QCalendarWidget *cal = new QCalendarWidget(parent);
  cal->setStyleSheet(R"(
    QCalendarWidget {
      background: #ffffff;
    }
    QCalendarWidget QWidget#qt_calendar_navigationbar {
      background: #f0f3f9;
    }
    QCalendarWidget QToolButton {
      color: #202533;
      background: transparent;
      border: none;
      border-radius: 4px;
      padding: 4px 8px;
      font-weight: 600;
    }
    QCalendarWidget QToolButton:hover {
      background: #dfe6f2;
    }
    QCalendarWidget QToolButton::menu-indicator {
      image: none;
    }
    QCalendarWidget QSpinBox {
      background: #ffffff;
      color: #202533;
      border: 1px solid #cfd6e4;
      border-radius: 4px;
      padding: 4px 6px;
    }
    QCalendarWidget QTableView {
      background: #ffffff;
      color: #202533;
      selection-background-color: #e7efff;
      selection-color: #182033;
      border: none;
      outline: none;
    }
    QCalendarWidget QAbstractItemView:enabled {
      background: #ffffff;
      color: #202533;
      selection-background-color: #e7efff;
      selection-color: #182033;
    }
    QCalendarWidget QAbstractItemView:disabled {
      color: #b0b8c8;
    }
  )");
  return cal;
}
} // namespace

qt_test::qt_test(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_qt_test),
      m_currentDailyDate(QDate::currentDate()),
      m_dailyViewMode(DailyViewMode::Today) {
  ui->setupUi(this);
  setWindowTitle("MyPlan - 任务计划管理器");
  resize(980, 720);

  initDataFile();
  m_taskManager.loadFromFile(m_dataFilePath);
  initUi();
  initConnections();
  refreshDailyTaskTable();
  refreshTodoTaskTable();
  ui->statusbar->showMessage("数据已加载", 2000);
}

qt_test::~qt_test() {
  m_taskManager.saveToFile(m_dataFilePath);
  delete ui;
}

void qt_test::initDataFile() {
  const QString dataDir =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir dir(dataDir);
  if (!dir.exists()) {
    dir.mkpath(".");
  }
  m_dataFilePath = dataDir + "/tasks.json";
}

void qt_test::initUi() {
  ui->dailyStatusFilter->setCurrentIndex(1);
  ui->todoStatusFilter->setCurrentIndex(1);

  for (QTableWidget *table : {ui->dailyTaskTable, ui->todoTaskTable}) {
    table->verticalHeader()->setVisible(false);
    table->verticalHeader()->setDefaultSectionSize(44);
    table->horizontalHeader()->setHighlightSections(false);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    table->setColumnWidth(0, 58);
    table->setColumnWidth(1, 104);
    table->setColumnWidth(4, 150);
    table->setShowGrid(false);
  }
  ui->dailyTaskTable->setColumnWidth(1, 76);

  applyStyle();
  refreshHistoryDateOptions();
  updateDailyViewLabel();
  updateHistoryControls();
}

void qt_test::applyStyle() {
  setStyleSheet(R"(
    QMainWindow, QWidget {
      background: #f6f7fb;
      color: #202533;
      font-family: "Microsoft YaHei", "Segoe UI", sans-serif;
      font-size: 14px;
    }
    QLabel#titleLabel {
      font-size: 28px;
      font-weight: 700;
      color: #172033;
    }
    QLabel#subtitleLabel, QLabel#dailyStatsLabel, QLabel#todoStatsLabel {
      color: #657087;
    }
    QLabel#dailyViewLabel {
      color: #172033;
      font-size: 18px;
      font-weight: 700;
      padding-right: 8px;
    }
    QTabWidget::pane {
      border: 1px solid #dfe4ee;
      border-radius: 8px;
      background: #ffffff;
    }
    QTabBar::tab {
      background: #e9edf5;
      color: #566176;
      padding: 9px 18px;
      border-top-left-radius: 6px;
      border-top-right-radius: 6px;
      margin-right: 4px;
    }
    QTabBar::tab:selected {
      background: #ffffff;
      color: #1f5eff;
      font-weight: 600;
    }
    QLineEdit, QDateEdit, QSpinBox, QComboBox, QPlainTextEdit {
      background: #ffffff;
      border: 1px solid #cfd6e4;
      border-radius: 6px;
      padding: 7px 10px;
      min-height: 22px;
    }
    QComboBox#dailyStatusFilter, QComboBox#todoStatusFilter {
      padding-left: 8px;
      padding-right: 6px;
    }
    QLineEdit:focus, QDateEdit:focus, QSpinBox:focus, QComboBox:focus,
    QPlainTextEdit:focus {
      border: 1px solid #3b73ff;
    }
    QPushButton {
      background: #245dff;
      color: #ffffff;
      border: none;
      border-radius: 6px;
      padding: 8px 14px;
      font-weight: 600;
    }
    QPushButton:hover {
      background: #174ce0;
    }
    QPushButton[secondary="true"] {
      background: #edf1f8;
      color: #2a3448;
    }
    QPushButton[secondary="true"]:hover {
      background: #dfe6f2;
    }
    QPushButton[activeMode="true"] {
      background: #172033;
      color: #ffffff;
    }
    QTableWidget {
      background: #ffffff;
      alternate-background-color: #f8faff;
      border: 1px solid #dfe4ee;
      border-radius: 8px;
      selection-background-color: #e7efff;
      selection-color: #182033;
    }
    QCheckBox {
      spacing: 8px;
    }
    QCheckBox::indicator {
      width: 18px;
      height: 18px;
      border: 2px solid #c4cddb;
      border-radius: 4px;
      background: #ffffff;
    }
    QCheckBox::indicator:checked {
      background: #245dff;
      border-color: #245dff;
    }
    QHeaderView::section {
      background: #f0f3f9;
      color: #596579;
      border: none;
      border-bottom: 1px solid #dfe4ee;
      padding: 8px;
      font-weight: 600;
    }
    QStatusBar {
      background: #f6f7fb;
      color: #657087;
    }
  )");

  for (QPushButton *button :
       {ui->todayPlanBtn, ui->tomorrowPlanBtn, ui->historyPlanBtn}) {
    button->setProperty("secondary", true);
  }
}

void qt_test::initConnections() {
  connect(ui->todayPlanBtn, &QPushButton::clicked, this, [this]() {
    selectDailyDate(QDate::currentDate(), DailyViewMode::Today);
  });
  connect(ui->tomorrowPlanBtn, &QPushButton::clicked, this, [this]() {
    selectDailyDate(QDate::currentDate().addDays(1), DailyViewMode::Tomorrow);
  });
  connect(ui->historyPlanBtn, &QPushButton::clicked, this, [this]() {
    m_dailyViewMode = DailyViewMode::History;
    refreshHistoryDateOptions();
    updateDailyViewLabel();
    updateHistoryControls();
    refreshDailyTaskTable();
  });
  connect(ui->historyRangeCombo,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
            updateHistoryControls();
            refreshDailyTaskTable();
          });
  connect(ui->historyDateCombo,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &qt_test::refreshDailyTaskTable);
  connect(ui->historySearchEdit, &QLineEdit::textChanged, this,
          &qt_test::refreshDailyTaskTable);
  connect(ui->dailyStatusFilter,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &qt_test::refreshDailyTaskTable);
  connect(ui->dailyTaskTable, &QTableWidget::cellDoubleClicked, this,
          [this](int row, int) {
            editTask(taskIdFromRow(ui->dailyTaskTable, row));
          });

  connect(ui->todoSearchEdit, &QLineEdit::textChanged, this,
          &qt_test::refreshTodoTaskTable);
  connect(ui->todoStatusFilter,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &qt_test::refreshTodoTaskTable);
  connect(ui->todoTaskTable, &QTableWidget::cellDoubleClicked, this,
          [this](int row, int) {
            editTask(taskIdFromRow(ui->todoTaskTable, row));
          });
}

void qt_test::selectDailyDate(const QDate &date, DailyViewMode mode) {
  m_currentDailyDate = date;
  m_dailyViewMode = mode;
  updateDailyViewLabel();
  updateHistoryControls();
  refreshDailyTaskTable();
}

void qt_test::onAddDailyTask() {
  if (m_dailyViewMode == DailyViewMode::History) {
    return;
  }

  QLineEdit *titleEdit =
      ui->dailyTaskTable->cellWidget(0, 2)
          ? ui->dailyTaskTable->cellWidget(0, 2)->findChild<QLineEdit *>(
                "dailyInlineTitle")
          : nullptr;
  QSpinBox *prioritySpin =
      ui->dailyTaskTable->cellWidget(0, 1)
          ? ui->dailyTaskTable->cellWidget(0, 1)->findChild<QSpinBox *>(
                "dailyInlinePriority")
          : nullptr;
  if (!titleEdit || !prioritySpin) {
    return;
  }

  const QString title = titleEdit->text().trimmed();
  if (title.isEmpty()) {
    QMessageBox::warning(this, "提示", "请输入计划内容。");
    return;
  }

  Task task;
  task.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
  task.title = title;
  task.date = m_currentDailyDate;
  task.createdAt = QDate::currentDate();
  task.isDaily = true;
  task.priority = prioritySpin->value();

  m_taskManager.addTask(task);
  refreshHistoryDateOptions();
  refreshDailyTaskTable();
  saveTasks("计划已添加");
}

void qt_test::onAddTodoTask() {
  QLineEdit *titleEdit =
      ui->todoTaskTable->cellWidget(0, 2)
          ? ui->todoTaskTable->cellWidget(0, 2)->findChild<QLineEdit *>(
                "todoInlineTitle")
          : nullptr;
  QDateEdit *dueDateEdit =
      ui->todoTaskTable->cellWidget(0, 1)
          ? ui->todoTaskTable->cellWidget(0, 1)->findChild<QDateEdit *>(
                "todoInlineDueDate")
          : nullptr;
  if (!titleEdit || !dueDateEdit) {
    return;
  }

  const QString title = titleEdit->text().trimmed();
  if (title.isEmpty()) {
    QMessageBox::warning(this, "提示", "请输入待办事项。");
    return;
  }

  Task task;
  task.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
  task.title = title;
  task.isDaily = false;
  task.dueDate = dueDateEdit->date();
  task.createdAt = QDate::currentDate();

  m_taskManager.addTask(task);
  refreshTodoTaskTable();
  saveTasks("待办事项已添加");
}

void qt_test::refreshDailyTaskTable() {
  if (m_dailyViewMode == DailyViewMode::History) {
    refreshHistoryTaskTable();
    return;
  }

  const QList<Task> tasks = filteredTasks(
      m_taskManager.getDailyTasks(m_currentDailyDate), QString(),
      statusFilterFromIndex(ui->dailyStatusFilter->currentIndex()));
  populateTaskTable(ui->dailyTaskTable, tasks, true);
  updateStats();
}

void qt_test::refreshHistoryTaskTable() {
  const QList<Task> tasks = filteredTasks(
      historyTasks(), ui->historySearchEdit->text(),
      statusFilterFromIndex(ui->dailyStatusFilter->currentIndex()));
  populateHistoryTaskTable(tasks);
  updateStats();
}

void qt_test::refreshTodoTaskTable() {
  const QList<Task> tasks = filteredTasks(
      m_taskManager.getTodoTasks(true), ui->todoSearchEdit->text(),
      statusFilterFromIndex(ui->todoStatusFilter->currentIndex()));
  populateTodoTaskTable(tasks);
  updateStats();
}

QList<Task> qt_test::filteredTasks(const QList<Task> &tasks,
                                   const QString &searchText,
                                   StatusFilter status) const {
  QList<Task> result;
  const QString needle = searchText.trimmed();

  for (const Task &task : tasks) {
    if (status == StatusFilter::Active && task.completed) {
      continue;
    }
    if (status == StatusFilter::Completed && !task.completed) {
      continue;
    }
    if (!needle.isEmpty() &&
        !task.title.contains(needle, Qt::CaseInsensitive) &&
        !task.description.contains(needle, Qt::CaseInsensitive)) {
      continue;
    }
    result.append(task);
  }
  return result;
}

QList<Task> qt_test::historyTasks() const {
  QList<Task> result;
  const QDate today = QDate::currentDate();
  const HistoryRange range =
      historyRangeFromIndex(ui->historyRangeCombo->currentIndex());
  const QDate specificDate =
      ui->historyDateCombo->currentData(Qt::UserRole).toDate();

  for (const Task &task : m_taskManager.getAllTasks()) {
    if (!task.isDaily || !task.date.isValid() || task.date >= today) {
      continue;
    }
    if (range == HistoryRange::LastWeek && task.date < today.addDays(-7)) {
      continue;
    }
    if (range == HistoryRange::SpecificDate && task.date != specificDate) {
      continue;
    }
    result.append(task);
  }

  std::sort(result.begin(), result.end(), [](const Task &a, const Task &b) {
    if (a.date != b.date) {
      return a.date > b.date;
    }
    if (a.completed != b.completed) {
      return !a.completed;
    }
    if (a.priority != b.priority) {
      return a.priority > b.priority;
    }
    return a.createdAt > b.createdAt;
  });
  return result;
}

qt_test::StatusFilter qt_test::statusFilterFromIndex(int index) const {
  if (index == 1) {
    return StatusFilter::Active;
  }
  if (index == 2) {
    return StatusFilter::Completed;
  }
  return StatusFilter::All;
}

qt_test::HistoryRange qt_test::historyRangeFromIndex(int index) const {
  if (index == 1) {
    return HistoryRange::LastWeek;
  }
  if (index == 2) {
    return HistoryRange::SpecificDate;
  }
  return HistoryRange::All;
}

void qt_test::populateTaskTable(QTableWidget *table, const QList<Task> &tasks,
                                bool daily) {
  table->setRowCount(0);

  if (daily) {
    table->insertRow(0);
    table->setRowHeight(0, 50);
    QTableWidgetItem *addLabel = new QTableWidgetItem("+");
    addLabel->setTextAlignment(Qt::AlignCenter);
    addLabel->setFlags(Qt::ItemIsEnabled);
    addLabel->setForeground(QColor("#245dff"));
    table->setItem(0, 0, addLabel);

    QWidget *priorityWrap = new QWidget(table);
    priorityWrap->setStyleSheet("background: transparent;");
    QHBoxLayout *priorityLayout = new QHBoxLayout(priorityWrap);
    priorityLayout->setContentsMargins(4, 5, 4, 5);
    QSpinBox *prioritySpin = new QSpinBox(priorityWrap);
    prioritySpin->setObjectName("dailyInlinePriority");
    prioritySpin->setRange(1, 5);
    prioritySpin->setValue(3);
    prioritySpin->setPrefix("P");
    priorityLayout->addWidget(prioritySpin);
    table->setCellWidget(0, 1, priorityWrap);

    QWidget *titleWrap = new QWidget(table);
    titleWrap->setStyleSheet("background: transparent;");
    QHBoxLayout *titleLayout = new QHBoxLayout(titleWrap);
    titleLayout->setContentsMargins(4, 5, 4, 5);
    QLineEdit *titleEdit = new QLineEdit(titleWrap);
    titleEdit->setObjectName("dailyInlineTitle");
    titleEdit->setPlaceholderText("在这里添加计划...");
    connect(titleEdit, &QLineEdit::returnPressed, this,
            &qt_test::onAddDailyTask);
    titleLayout->addWidget(titleEdit);
    table->setCellWidget(0, 2, titleWrap);

    QTableWidgetItem *descriptionHint =
        new QTableWidgetItem("添加后可双击编辑描述");
    descriptionHint->setForeground(QColor("#98a2b7"));
    descriptionHint->setFlags(Qt::ItemIsEnabled);
    table->setItem(0, 3, descriptionHint);

    QWidget *actions = new QWidget(table);
    actions->setStyleSheet("background: transparent;");
    QHBoxLayout *actionLayout = new QHBoxLayout(actions);
    actionLayout->setContentsMargins(4, 5, 4, 5);
    QPushButton *addBtn = new QPushButton("添加", actions);
    connect(addBtn, &QPushButton::clicked, this, &qt_test::onAddDailyTask);
    actionLayout->addWidget(addBtn);
    table->setCellWidget(0, 4, actions);
  }

  if (tasks.isEmpty()) {
    const int row = daily ? 1 : 0;
    table->insertRow(row);
    table->setSpan(row, 0, 1, table->columnCount());
    QTableWidgetItem *emptyItem = new QTableWidgetItem(
        daily ? "当前视图还没有计划" : "没有匹配的待办事项");
    emptyItem->setTextAlignment(Qt::AlignCenter);
    emptyItem->setForeground(QColor("#8a94a8"));
    emptyItem->setFlags(Qt::ItemIsEnabled);
    table->setItem(row, 0, emptyItem);
    return;
  }

  for (const Task &task : tasks) {
    const int row = table->rowCount();
    table->insertRow(row);

    QCheckBox *checkbox = new QCheckBox();
    checkbox->setChecked(task.completed);
    checkbox->setProperty("taskId", task.id);
    checkbox->setStyleSheet("margin-left: 18px;");
    connect(checkbox, &QCheckBox::toggled, this,
            [this, daily, id = task.id](bool checked) {
              m_taskManager.markCompleted(id, checked);
              saveTasks(checked ? "任务已完成" : "任务已恢复");
              daily ? refreshDailyTaskTable() : refreshTodoTaskTable();
            });
    table->setCellWidget(row, 0, checkbox);

    QTableWidgetItem *priorityItem =
        new QTableWidgetItem(priorityText(task.priority));
    priorityItem->setTextAlignment(Qt::AlignCenter);
    priorityItem->setData(Qt::UserRole, task.id);
    table->setItem(row, 1, priorityItem);

    QTableWidgetItem *titleItem = new QTableWidgetItem(task.title);
    titleItem->setData(Qt::UserRole, task.id);
    table->setItem(row, 2, titleItem);

    QTableWidgetItem *descriptionItem =
        new QTableWidgetItem(descriptionPreview(task.description));
    descriptionItem->setData(Qt::UserRole, task.id);
    descriptionItem->setForeground(task.description.trimmed().isEmpty()
                                       ? QColor("#98a2b7")
                                       : QColor("#3b4354"));
    table->setItem(row, 3, descriptionItem);

    QWidget *actions = new QWidget(table);
    actions->setStyleSheet("background: transparent;");
    QHBoxLayout *layout = new QHBoxLayout(actions);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(6);

    QPushButton *editBtn = new QPushButton("编辑", actions);
    editBtn->setProperty("secondary", true);
    editBtn->setProperty("taskId", task.id);
    editBtn->setMinimumWidth(58);
    connect(editBtn, &QPushButton::clicked, this,
            daily ? &qt_test::onEditDailyTask : &qt_test::onEditTodoTask);

    QPushButton *deleteBtn = new QPushButton("删除", actions);
    deleteBtn->setProperty("taskId", task.id);
    deleteBtn->setMinimumWidth(58);
    connect(deleteBtn, &QPushButton::clicked, this,
            daily ? &qt_test::onDeleteDailyTask : &qt_test::onDeleteTodoTask);

    layout->addWidget(editBtn);
    layout->addWidget(deleteBtn);
    table->setCellWidget(row, 4, actions);

    if (task.completed) {
      for (int col = 1; col <= 3; ++col) {
        QTableWidgetItem *item = table->item(row, col);
        item->setForeground(QColor("#9aa3b5"));
      }
      QFont strikeFont = titleItem->font();
      strikeFont.setStrikeOut(true);
      titleItem->setFont(strikeFont);
    }
  }
}

void qt_test::populateTodoTaskTable(const QList<Task> &tasks) {
  QTableWidget *table = ui->todoTaskTable;
  table->setRowCount(0);
  table->insertRow(0);
  table->setRowHeight(0, 50);

  QTableWidgetItem *addLabel = new QTableWidgetItem("+");
  addLabel->setTextAlignment(Qt::AlignCenter);
  addLabel->setFlags(Qt::ItemIsEnabled);
  addLabel->setForeground(QColor("#245dff"));
  table->setItem(0, 0, addLabel);

  QWidget *dateWrap = new QWidget(table);
  dateWrap->setStyleSheet("background: transparent;");
  QHBoxLayout *dateLayout = new QHBoxLayout(dateWrap);
  dateLayout->setContentsMargins(4, 5, 4, 5);
  QDateEdit *dueDateEdit = new QDateEdit(QDate::currentDate(), dateWrap);
  dueDateEdit->setObjectName("todoInlineDueDate");
  dueDateEdit->setCalendarPopup(true);
  dueDateEdit->setCalendarWidget(makeCalendar(dueDateEdit));
  dueDateEdit->setDisplayFormat("yyyy-MM-dd");
  dateLayout->addWidget(dueDateEdit);
  table->setCellWidget(0, 1, dateWrap);

  QWidget *titleWrap = new QWidget(table);
  titleWrap->setStyleSheet("background: transparent;");
  QHBoxLayout *titleLayout = new QHBoxLayout(titleWrap);
  titleLayout->setContentsMargins(4, 5, 4, 5);
  QLineEdit *titleEdit = new QLineEdit(titleWrap);
  titleEdit->setObjectName("todoInlineTitle");
  titleEdit->setPlaceholderText("在这里添加待办事项...");
  connect(titleEdit, &QLineEdit::returnPressed, this, &qt_test::onAddTodoTask);
  titleLayout->addWidget(titleEdit);
  table->setCellWidget(0, 2, titleWrap);

  QTableWidgetItem *descriptionHint =
      new QTableWidgetItem("添加后可双击编辑描述");
  descriptionHint->setForeground(QColor("#98a2b7"));
  descriptionHint->setFlags(Qt::ItemIsEnabled);
  table->setItem(0, 3, descriptionHint);

  QWidget *addActions = new QWidget(table);
  addActions->setStyleSheet("background: transparent;");
  QHBoxLayout *addActionLayout = new QHBoxLayout(addActions);
  addActionLayout->setContentsMargins(4, 5, 4, 5);
  QPushButton *addBtn = new QPushButton("添加", addActions);
  connect(addBtn, &QPushButton::clicked, this, &qt_test::onAddTodoTask);
  addActionLayout->addWidget(addBtn);
  table->setCellWidget(0, 4, addActions);

  if (tasks.isEmpty()) {
    table->insertRow(1);
    table->setSpan(1, 0, 1, table->columnCount());
    QTableWidgetItem *emptyItem = new QTableWidgetItem("没有匹配的待办事项");
    emptyItem->setTextAlignment(Qt::AlignCenter);
    emptyItem->setForeground(QColor("#8a94a8"));
    emptyItem->setFlags(Qt::ItemIsEnabled);
    table->setItem(1, 0, emptyItem);
    return;
  }

  for (const Task &task : tasks) {
    const int row = table->rowCount();
    table->insertRow(row);

    QCheckBox *checkbox = new QCheckBox();
    checkbox->setChecked(task.completed);
    checkbox->setProperty("taskId", task.id);
    checkbox->setStyleSheet("margin-left: 18px;");
    connect(checkbox, &QCheckBox::toggled, this,
            [this, id = task.id](bool checked) {
              m_taskManager.markCompleted(id, checked);
              saveTasks(checked ? "任务已完成" : "任务已恢复");
              refreshTodoTaskTable();
            });
    table->setCellWidget(row, 0, checkbox);

    QTableWidgetItem *dueItem = new QTableWidgetItem(dueDateText(task.dueDate));
    dueItem->setTextAlignment(Qt::AlignCenter);
    dueItem->setData(Qt::UserRole, task.id);
    table->setItem(row, 1, dueItem);

    QTableWidgetItem *titleItem = new QTableWidgetItem(task.title);
    titleItem->setData(Qt::UserRole, task.id);
    table->setItem(row, 2, titleItem);

    QTableWidgetItem *descriptionItem =
        new QTableWidgetItem(descriptionPreview(task.description));
    descriptionItem->setData(Qt::UserRole, task.id);
    descriptionItem->setForeground(task.description.trimmed().isEmpty()
                                       ? QColor("#98a2b7")
                                       : QColor("#3b4354"));
    table->setItem(row, 3, descriptionItem);

    QWidget *actions = new QWidget(table);
    actions->setStyleSheet("background: transparent;");
    QHBoxLayout *layout = new QHBoxLayout(actions);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(6);

    QPushButton *editBtn = new QPushButton("编辑", actions);
    editBtn->setProperty("secondary", true);
    editBtn->setProperty("taskId", task.id);
    editBtn->setMinimumWidth(58);
    connect(editBtn, &QPushButton::clicked, this, &qt_test::onEditTodoTask);

    QPushButton *deleteBtn = new QPushButton("删除", actions);
    deleteBtn->setProperty("taskId", task.id);
    deleteBtn->setMinimumWidth(58);
    connect(deleteBtn, &QPushButton::clicked, this, &qt_test::onDeleteTodoTask);

    layout->addWidget(editBtn);
    layout->addWidget(deleteBtn);
    table->setCellWidget(row, 4, actions);

    if (task.completed) {
      for (int col = 1; col <= 3; ++col) {
        table->item(row, col)->setForeground(QColor("#9aa3b5"));
      }
      QFont strikeFont = titleItem->font();
      strikeFont.setStrikeOut(true);
      titleItem->setFont(strikeFont);
    }
  }
}

void qt_test::populateHistoryTaskTable(const QList<Task> &tasks) {
  QTableWidget *table = ui->dailyTaskTable;
  table->setRowCount(0);

  if (tasks.isEmpty()) {
    table->insertRow(0);
    table->setSpan(0, 0, 1, table->columnCount());
    QTableWidgetItem *emptyItem = new QTableWidgetItem("没有匹配的历史计划");
    emptyItem->setTextAlignment(Qt::AlignCenter);
    emptyItem->setForeground(QColor("#8a94a8"));
    emptyItem->setFlags(Qt::ItemIsEnabled);
    table->setItem(0, 0, emptyItem);
    return;
  }

  QDate currentGroup;
  for (const Task &task : tasks) {
    if (task.date != currentGroup) {
      currentGroup = task.date;
      const int groupRow = table->rowCount();
      table->insertRow(groupRow);
      table->setSpan(groupRow, 0, 1, table->columnCount());
      QTableWidgetItem *groupItem =
          new QTableWidgetItem(currentGroup.toString("yyyy-MM-dd dddd"));
      groupItem->setFlags(Qt::ItemIsEnabled);
      groupItem->setForeground(QColor("#172033"));
      groupItem->setBackground(QColor("#eef3ff"));
      QFont font = groupItem->font();
      font.setBold(true);
      groupItem->setFont(font);
      table->setItem(groupRow, 0, groupItem);
    }

    const int row = table->rowCount();
    table->insertRow(row);

    QCheckBox *checkbox = new QCheckBox();
    checkbox->setChecked(task.completed);
    checkbox->setProperty("taskId", task.id);
    checkbox->setStyleSheet("margin-left: 18px;");
    connect(checkbox, &QCheckBox::toggled, this,
            [this, id = task.id](bool checked) {
              m_taskManager.markCompleted(id, checked);
              saveTasks(checked ? "任务已完成" : "任务已恢复");
              refreshDailyTaskTable();
            });
    table->setCellWidget(row, 0, checkbox);

    QTableWidgetItem *priorityItem =
        new QTableWidgetItem(priorityText(task.priority));
    priorityItem->setTextAlignment(Qt::AlignCenter);
    priorityItem->setData(Qt::UserRole, task.id);
    table->setItem(row, 1, priorityItem);

    QTableWidgetItem *titleItem = new QTableWidgetItem(task.title);
    titleItem->setData(Qt::UserRole, task.id);
    table->setItem(row, 2, titleItem);

    QTableWidgetItem *descriptionItem =
        new QTableWidgetItem(descriptionPreview(task.description));
    descriptionItem->setData(Qt::UserRole, task.id);
    descriptionItem->setForeground(task.description.trimmed().isEmpty()
                                       ? QColor("#98a2b7")
                                       : QColor("#3b4354"));
    table->setItem(row, 3, descriptionItem);

    QWidget *actions = new QWidget(table);
    actions->setStyleSheet("background: transparent;");
    QHBoxLayout *layout = new QHBoxLayout(actions);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(6);

    QPushButton *editBtn = new QPushButton("编辑", actions);
    editBtn->setProperty("secondary", true);
    editBtn->setProperty("taskId", task.id);
    editBtn->setMinimumWidth(58);
    connect(editBtn, &QPushButton::clicked, this, &qt_test::onEditDailyTask);

    QPushButton *deleteBtn = new QPushButton("删除", actions);
    deleteBtn->setProperty("taskId", task.id);
    deleteBtn->setMinimumWidth(58);
    connect(deleteBtn, &QPushButton::clicked, this,
            &qt_test::onDeleteDailyTask);

    layout->addWidget(editBtn);
    layout->addWidget(deleteBtn);
    table->setCellWidget(row, 4, actions);

    if (task.completed) {
      for (int col = 1; col <= 3; ++col) {
        table->item(row, col)->setForeground(QColor("#9aa3b5"));
      }
      QFont strikeFont = titleItem->font();
      strikeFont.setStrikeOut(true);
      titleItem->setFont(strikeFont);
    }
  }
}

void qt_test::refreshHistoryDateOptions() {
  QSignalBlocker blocker(ui->historyDateCombo);
  const QVariant selected = ui->historyDateCombo->currentData(Qt::UserRole);
  ui->historyDateCombo->clear();

  const QDate today = QDate::currentDate();
  const QList<QDate> dates = m_taskManager.getAllDates();
  for (const QDate &date : dates) {
    if (date < today) {
      ui->historyDateCombo->addItem(date.toString("yyyy-MM-dd dddd"), date);
    }
  }

  const int previousIndex = ui->historyDateCombo->findData(selected);
  if (previousIndex >= 0) {
    ui->historyDateCombo->setCurrentIndex(previousIndex);
  }
}

void qt_test::updateStats() {
  const auto setStats = [](QLabel *label, const QList<Task> &tasks) {
    int completed = 0;
    for (const Task &task : tasks) {
      if (task.completed) {
        ++completed;
      }
    }
    const int total = tasks.size();
    const int active = total - completed;
    const int percent = total == 0 ? 0 : completed * 100 / total;
    label->setText(QString("共 %1 项 · 已完成 %2 · 未完成 %3 · 进度 %4%")
                       .arg(total)
                       .arg(completed)
                       .arg(active)
                       .arg(percent));
  };

  const QList<Task> dailyTasks =
      m_dailyViewMode == DailyViewMode::History
          ? historyTasks()
          : m_taskManager.getDailyTasks(m_currentDailyDate);
  setStats(ui->dailyStatsLabel, dailyTasks);
  setStats(ui->todoStatsLabel, m_taskManager.getTodoTasks(true));
}

void qt_test::updateDailyViewLabel() {
  QString label = "历史计划";
  if (m_dailyViewMode == DailyViewMode::Today) {
    label = "今日计划";
  } else if (m_dailyViewMode == DailyViewMode::Tomorrow) {
    label = "明日计划";
  }
  ui->dailyViewLabel->setText(label);

  ui->todayPlanBtn->setProperty("activeMode",
                                m_dailyViewMode == DailyViewMode::Today);
  ui->tomorrowPlanBtn->setProperty("activeMode",
                                   m_dailyViewMode == DailyViewMode::Tomorrow);
  ui->historyPlanBtn->setProperty("activeMode",
                                  m_dailyViewMode == DailyViewMode::History);
  for (QPushButton *button :
       {ui->todayPlanBtn, ui->tomorrowPlanBtn, ui->historyPlanBtn}) {
    button->style()->unpolish(button);
    button->style()->polish(button);
  }
}

void qt_test::updateHistoryControls() {
  const bool historyMode = m_dailyViewMode == DailyViewMode::History;
  const bool specificDate =
      historyRangeFromIndex(ui->historyRangeCombo->currentIndex()) ==
      HistoryRange::SpecificDate;
  ui->historyRangeCombo->setVisible(historyMode);
  ui->historyDateCombo->setVisible(historyMode && specificDate);
  ui->historySearchEdit->setVisible(historyMode);
  ui->historySearchEdit->setEnabled(historyMode);
}

void qt_test::onDeleteDailyTask() {
  const QString taskId = taskIdFromSender();
  const Task task = m_taskManager.getTask(taskId);
  if (task.id.isEmpty() || !confirmDelete(task.title)) {
    return;
  }
  m_taskManager.removeTask(taskId);
  refreshHistoryDateOptions();
  refreshDailyTaskTable();
  saveTasks("计划已删除");
}

void qt_test::onDeleteTodoTask() {
  const QString taskId = taskIdFromSender();
  const Task task = m_taskManager.getTask(taskId);
  if (task.id.isEmpty() || !confirmDelete(task.title)) {
    return;
  }
  m_taskManager.removeTask(taskId);
  refreshTodoTaskTable();
  saveTasks("待办事项已删除");
}

void qt_test::onEditDailyTask() { editTask(taskIdFromSender()); }

void qt_test::onEditTodoTask() { editTask(taskIdFromSender()); }

void qt_test::editTask(const QString &taskId) {
  Task task = m_taskManager.getTask(taskId);
  if (task.id.isEmpty()) {
    return;
  }

  QDialog dialog(this);
  dialog.setWindowTitle(task.isDaily ? "编辑每日计划" : "编辑待办事项");
  dialog.resize(460, 340);

  QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
  QFormLayout *formLayout = new QFormLayout();
  QLineEdit *titleEdit = new QLineEdit(task.title, &dialog);
  QPlainTextEdit *descriptionEdit =
      new QPlainTextEdit(task.description, &dialog);

  descriptionEdit->setPlaceholderText("添加描述、上下文或下一步行动...");

  formLayout->addRow("标题", titleEdit);
  QDateEdit *dueDateEdit = nullptr;
  QSpinBox *prioritySpin = nullptr;
  if (task.isDaily) {
    prioritySpin = new QSpinBox(&dialog);
    prioritySpin->setRange(1, 5);
    prioritySpin->setValue(task.priority);
    formLayout->addRow("优先级", prioritySpin);
  } else {
    dueDateEdit = new QDateEdit(
        task.dueDate.isValid() ? task.dueDate : QDate::currentDate(), &dialog);
    dueDateEdit->setCalendarPopup(true);
    dueDateEdit->setCalendarWidget(makeCalendar(dueDateEdit));
    dueDateEdit->setDisplayFormat("yyyy-MM-dd");
    formLayout->addRow("截止日期", dueDateEdit);
  }
  formLayout->addRow("描述", descriptionEdit);
  mainLayout->addLayout(formLayout);

  QDialogButtonBox *buttons = new QDialogButtonBox(
      QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
  buttons->button(QDialogButtonBox::Save)->setText("保存");
  buttons->button(QDialogButtonBox::Cancel)->setText("取消");
  mainLayout->addWidget(buttons);

  connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
  connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

  if (dialog.exec() != QDialog::Accepted) {
    return;
  }

  const QString title = titleEdit->text().trimmed();
  if (title.isEmpty()) {
    QMessageBox::warning(this, "提示", "任务标题不能为空。");
    return;
  }

  task.title = title;
  task.description = descriptionEdit->toPlainText().trimmed();
  if (task.isDaily) {
    task.priority = prioritySpin->value();
  } else {
    task.dueDate = dueDateEdit->date();
  }
  m_taskManager.updateTask(task);
  refreshHistoryDateOptions();
  task.isDaily ? refreshDailyTaskTable() : refreshTodoTaskTable();
  saveTasks("任务已更新");
}

bool qt_test::confirmDelete(const QString &taskTitle) {
  return QMessageBox::question(
             this, "确认删除",
             QString("确定要删除“%1”吗？此操作无法撤销。").arg(taskTitle),
             QMessageBox::Yes | QMessageBox::Cancel,
             QMessageBox::Cancel) == QMessageBox::Yes;
}

bool qt_test::saveTasks(const QString &message) {
  if (m_taskManager.saveToFile(m_dataFilePath)) {
    ui->statusbar->showMessage(message, 2000);
    return true;
  }
  QMessageBox::warning(this, "错误", "保存文件失败。");
  return false;
}

QString qt_test::taskIdFromSender() const {
  const QObject *obj = sender();
  return obj ? obj->property("taskId").toString() : QString();
}

QString qt_test::taskIdFromRow(QTableWidget *table, int row) const {
  if (row < 0 || row >= table->rowCount()) {
    return QString();
  }
  QTableWidgetItem *item = table->item(row, 1);
  return item ? item->data(Qt::UserRole).toString() : QString();
}
