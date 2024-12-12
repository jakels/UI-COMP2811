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
#include <QShortcut>
#include <QFrame>
#include <QKeyEvent>
#include <QDebug>
#include "datavisualizationpage.h"
#include "pollutantoverview.h"
#include "fluorinatedpage.h"
#include "compliancedashboard.h"
#include "DatasetInterface.h"
#include "popspage.h"

class DashboardWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DashboardWindow(QWidget *parent = nullptr);
    void restartApplication();
    void onLanguageChanged(int index);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QStackedWidget *stackedWidget;

    QWidget *createDashboardPage();
    DataVisualizationPage *createDataVisualizationPage();
    QHBoxLayout *createFilters();
    QScrollArea *createContent();
    QFrame *createCard(const QString &title, const QString &style);
    QWidget *createNavigationBar();
    //void setupNavigation();
    void configureTabOrder();
};

#endif // MAINWINDOW_H
