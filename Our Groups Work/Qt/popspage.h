#ifndef POPSPAGE_H
#define POPSPAGE_H

#include "DatasetInterface.h"
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <vector>
#include <string>
#include <QToolTip>
#include <QHeaderView>
#include <QTableWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

class PopsPage : public QWidget
{
Q_OBJECT

public:
    explicit PopsPage(QWidget *parent = nullptr); // Constructor

private slots:
    void showRowDetails(int row, int column);  // Slot function to display detailed information for a single row in the table

private:
    void loadPopData(const std::string &filter); // Method to load POP data
    QChart *createChart();                      // Method to create the initial chart
    void updateChart(const std::vector<WaterQualitySample> &filteredSamples); // Method to update the chart data
    void showChartDataTooltip(const QPointF &point, bool state);             // Display tooltip for chart data

    QLabel *infoLabel;                          // Label to display loading information
    QComboBox *popSelector;                     // Combo box for selecting POPs
    QPushButton *loadDataButton;                // Button to load data
    QChartView *chartView;                      // View to display the chart
    QTableWidget *tableWidget;                  // Table widget to display pollutant data
};

#endif // POPSPAGE_H
