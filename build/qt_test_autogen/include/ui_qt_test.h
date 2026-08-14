/********************************************************************************
** Form generated from reading UI file 'qt_test.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_TEST_H
#define UI_QT_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qt_test
{
public:
    QWidget *centralwidget;
    QVBoxLayout *mainLayout;
    QVBoxLayout *headerLayout;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QTabWidget *tabWidget;
    QWidget *dailyTab;
    QVBoxLayout *dailyLayout;
    QHBoxLayout *dailyModeLayout;
    QPushButton *todayPlanBtn;
    QPushButton *tomorrowPlanBtn;
    QPushButton *historyPlanBtn;
    QSpacerItem *dailyModeSpacer;
    QLabel *dailyStatsLabel;
    QHBoxLayout *dailyFilterLayout;
    QLabel *dailyViewLabel;
    QComboBox *historyRangeCombo;
    QComboBox *historyDateCombo;
    QLineEdit *historySearchEdit;
    QSpacerItem *dailyFilterSpacer;
    QComboBox *dailyStatusFilter;
    QTableWidget *dailyTaskTable;
    QWidget *todoTab;
    QVBoxLayout *todoLayout;
    QHBoxLayout *todoFilterLayout;
    QLabel *todoStatsLabel;
    QLineEdit *todoSearchEdit;
    QComboBox *todoStatusFilter;
    QTableWidget *todoTaskTable;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *qt_test)
    {
        if (qt_test->objectName().isEmpty())
            qt_test->setObjectName("qt_test");
        qt_test->resize(980, 720);
        centralwidget = new QWidget(qt_test);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setSpacing(14);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(18, 18, 18, 14);
        headerLayout = new QVBoxLayout();
        headerLayout->setSpacing(4);
        headerLayout->setObjectName("headerLayout");
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        headerLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(centralwidget);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setAlignment(Qt::AlignCenter);

        headerLayout->addWidget(subtitleLabel);


        mainLayout->addLayout(headerLayout);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        dailyTab = new QWidget();
        dailyTab->setObjectName("dailyTab");
        dailyLayout = new QVBoxLayout(dailyTab);
        dailyLayout->setSpacing(12);
        dailyLayout->setObjectName("dailyLayout");
        dailyLayout->setContentsMargins(14, 14, 14, 14);
        dailyModeLayout = new QHBoxLayout();
        dailyModeLayout->setObjectName("dailyModeLayout");
        todayPlanBtn = new QPushButton(dailyTab);
        todayPlanBtn->setObjectName("todayPlanBtn");

        dailyModeLayout->addWidget(todayPlanBtn);

        tomorrowPlanBtn = new QPushButton(dailyTab);
        tomorrowPlanBtn->setObjectName("tomorrowPlanBtn");

        dailyModeLayout->addWidget(tomorrowPlanBtn);

        historyPlanBtn = new QPushButton(dailyTab);
        historyPlanBtn->setObjectName("historyPlanBtn");

        dailyModeLayout->addWidget(historyPlanBtn);

        dailyModeSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        dailyModeLayout->addItem(dailyModeSpacer);

        dailyStatsLabel = new QLabel(dailyTab);
        dailyStatsLabel->setObjectName("dailyStatsLabel");

        dailyModeLayout->addWidget(dailyStatsLabel);


        dailyLayout->addLayout(dailyModeLayout);

        dailyFilterLayout = new QHBoxLayout();
        dailyFilterLayout->setObjectName("dailyFilterLayout");
        dailyViewLabel = new QLabel(dailyTab);
        dailyViewLabel->setObjectName("dailyViewLabel");

        dailyFilterLayout->addWidget(dailyViewLabel);

        historyRangeCombo = new QComboBox(dailyTab);
        historyRangeCombo->addItem(QString());
        historyRangeCombo->addItem(QString());
        historyRangeCombo->addItem(QString());
        historyRangeCombo->setObjectName("historyRangeCombo");

        dailyFilterLayout->addWidget(historyRangeCombo);

        historyDateCombo = new QComboBox(dailyTab);
        historyDateCombo->setObjectName("historyDateCombo");

        dailyFilterLayout->addWidget(historyDateCombo);

        historySearchEdit = new QLineEdit(dailyTab);
        historySearchEdit->setObjectName("historySearchEdit");

        dailyFilterLayout->addWidget(historySearchEdit);

        dailyFilterSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        dailyFilterLayout->addItem(dailyFilterSpacer);

        dailyStatusFilter = new QComboBox(dailyTab);
        dailyStatusFilter->addItem(QString());
        dailyStatusFilter->addItem(QString());
        dailyStatusFilter->addItem(QString());
        dailyStatusFilter->setObjectName("dailyStatusFilter");
        dailyStatusFilter->setMaximumWidth(96);

        dailyFilterLayout->addWidget(dailyStatusFilter);


        dailyLayout->addLayout(dailyFilterLayout);

        dailyTaskTable = new QTableWidget(dailyTab);
        if (dailyTaskTable->columnCount() < 5)
            dailyTaskTable->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        dailyTaskTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        dailyTaskTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        dailyTaskTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        dailyTaskTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        dailyTaskTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        dailyTaskTable->setObjectName("dailyTaskTable");
        dailyTaskTable->setAlternatingRowColors(true);
        dailyTaskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        dailyTaskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dailyTaskTable->setColumnCount(5);

        dailyLayout->addWidget(dailyTaskTable);

        tabWidget->addTab(dailyTab, QString());
        todoTab = new QWidget();
        todoTab->setObjectName("todoTab");
        todoLayout = new QVBoxLayout(todoTab);
        todoLayout->setSpacing(12);
        todoLayout->setObjectName("todoLayout");
        todoLayout->setContentsMargins(14, 14, 14, 14);
        todoFilterLayout = new QHBoxLayout();
        todoFilterLayout->setObjectName("todoFilterLayout");
        todoStatsLabel = new QLabel(todoTab);
        todoStatsLabel->setObjectName("todoStatsLabel");

        todoFilterLayout->addWidget(todoStatsLabel);

        todoSearchEdit = new QLineEdit(todoTab);
        todoSearchEdit->setObjectName("todoSearchEdit");

        todoFilterLayout->addWidget(todoSearchEdit);

        todoStatusFilter = new QComboBox(todoTab);
        todoStatusFilter->addItem(QString());
        todoStatusFilter->addItem(QString());
        todoStatusFilter->addItem(QString());
        todoStatusFilter->setObjectName("todoStatusFilter");
        todoStatusFilter->setMaximumWidth(96);

        todoFilterLayout->addWidget(todoStatusFilter);


        todoLayout->addLayout(todoFilterLayout);

        todoTaskTable = new QTableWidget(todoTab);
        if (todoTaskTable->columnCount() < 5)
            todoTaskTable->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        todoTaskTable->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        todoTaskTable->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        todoTaskTable->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        todoTaskTable->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        todoTaskTable->setHorizontalHeaderItem(4, __qtablewidgetitem9);
        todoTaskTable->setObjectName("todoTaskTable");
        todoTaskTable->setAlternatingRowColors(true);
        todoTaskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        todoTaskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        todoTaskTable->setColumnCount(5);

        todoLayout->addWidget(todoTaskTable);

        tabWidget->addTab(todoTab, QString());

        mainLayout->addWidget(tabWidget);

        qt_test->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(qt_test);
        statusbar->setObjectName("statusbar");
        qt_test->setStatusBar(statusbar);

        retranslateUi(qt_test);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(qt_test);
    } // setupUi

    void retranslateUi(QMainWindow *qt_test)
    {
        qt_test->setWindowTitle(QCoreApplication::translate("qt_test", "MyPlan - \344\273\273\345\212\241\350\256\241\345\210\222\347\256\241\347\220\206\345\231\250", nullptr));
        titleLabel->setText(QCoreApplication::translate("qt_test", "MyPlan", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("qt_test", "\345\256\211\346\216\222\344\273\212\345\244\251\357\274\214\345\207\206\345\244\207\346\230\216\345\244\251\357\274\214\345\233\236\347\234\213\350\277\207\345\216\273\343\200\202", nullptr));
        todayPlanBtn->setText(QCoreApplication::translate("qt_test", "\344\273\212\346\227\245\350\256\241\345\210\222", nullptr));
        tomorrowPlanBtn->setText(QCoreApplication::translate("qt_test", "\346\230\216\346\227\245\350\256\241\345\210\222", nullptr));
        historyPlanBtn->setText(QCoreApplication::translate("qt_test", "\345\216\206\345\217\262\346\237\245\350\257\242", nullptr));
        dailyStatsLabel->setText(QCoreApplication::translate("qt_test", "\345\205\261 0 \351\241\271 \302\267 \345\267\262\345\256\214\346\210\220 0 \302\267 \346\234\252\345\256\214\346\210\220 0", nullptr));
        dailyViewLabel->setText(QCoreApplication::translate("qt_test", "\344\273\212\346\227\245\350\256\241\345\210\222", nullptr));
        historyRangeCombo->setItemText(0, QCoreApplication::translate("qt_test", "\345\205\250\351\203\250\345\216\206\345\217\262", nullptr));
        historyRangeCombo->setItemText(1, QCoreApplication::translate("qt_test", "\350\277\221\344\270\200\345\221\250", nullptr));
        historyRangeCombo->setItemText(2, QCoreApplication::translate("qt_test", "\345\205\267\344\275\223\346\237\220\344\270\200\345\244\251", nullptr));

        historySearchEdit->setPlaceholderText(QCoreApplication::translate("qt_test", "\346\220\234\347\264\242\345\216\206\345\217\262\350\256\241\345\210\222...", nullptr));
        dailyStatusFilter->setItemText(0, QCoreApplication::translate("qt_test", "\345\205\250\351\203\250", nullptr));
        dailyStatusFilter->setItemText(1, QCoreApplication::translate("qt_test", "\346\234\252\345\256\214\346\210\220", nullptr));
        dailyStatusFilter->setItemText(2, QCoreApplication::translate("qt_test", "\345\267\262\345\256\214\346\210\220", nullptr));

        QTableWidgetItem *___qtablewidgetitem = dailyTaskTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("qt_test", "\345\256\214\346\210\220", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = dailyTaskTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("qt_test", "\344\274\230\345\205\210\347\272\247", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = dailyTaskTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("qt_test", "\344\273\273\345\212\241", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = dailyTaskTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("qt_test", "\346\217\217\350\277\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = dailyTaskTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("qt_test", "\346\223\215\344\275\234", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(dailyTab), QCoreApplication::translate("qt_test", "\346\257\217\346\227\245\350\256\241\345\210\222", nullptr));
        todoStatsLabel->setText(QCoreApplication::translate("qt_test", "\345\205\261 0 \351\241\271 \302\267 \345\267\262\345\256\214\346\210\220 0 \302\267 \346\234\252\345\256\214\346\210\220 0", nullptr));
        todoSearchEdit->setPlaceholderText(QCoreApplication::translate("qt_test", "\346\220\234\347\264\242\346\240\207\351\242\230\346\210\226\346\217\217\350\277\260...", nullptr));
        todoStatusFilter->setItemText(0, QCoreApplication::translate("qt_test", "\345\205\250\351\203\250", nullptr));
        todoStatusFilter->setItemText(1, QCoreApplication::translate("qt_test", "\346\234\252\345\256\214\346\210\220", nullptr));
        todoStatusFilter->setItemText(2, QCoreApplication::translate("qt_test", "\345\267\262\345\256\214\346\210\220", nullptr));

        QTableWidgetItem *___qtablewidgetitem5 = todoTaskTable->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("qt_test", "\345\256\214\346\210\220", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = todoTaskTable->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("qt_test", "\346\210\252\346\255\242\346\227\245\346\234\237", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = todoTaskTable->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("qt_test", "\345\276\205\345\212\236\344\272\213\351\241\271", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = todoTaskTable->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("qt_test", "\346\217\217\350\277\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = todoTaskTable->horizontalHeaderItem(4);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("qt_test", "\346\223\215\344\275\234", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(todoTab), QCoreApplication::translate("qt_test", "\345\276\205\345\212\236\344\272\213\351\241\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class qt_test: public Ui_qt_test {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_TEST_H
