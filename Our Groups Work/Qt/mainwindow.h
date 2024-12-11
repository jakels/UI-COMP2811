#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QFrame>
#include <QDebug>
#include <QTranslator>

#include "datavisualizationpage.h"
#include "pollutantoverview.h"
#include "fluorinatedpage.h"
#include "compliancedashboard.h"
#include "DatasetInterface.h"
#include "popspage.h"

// Main application window class
class DashboardWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DashboardWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;

    QWidget *createDashboardPage();
    DataVisualizationPage *createDataVisualizationPage();
    QHBoxLayout *createFilters();
    QScrollArea *createContent();
    QFrame *createCard(const QString &title, const QString &style);

    void setupNavigation();


};

#endif // MAINWINDOW_H

