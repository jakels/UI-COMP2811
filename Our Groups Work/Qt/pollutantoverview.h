#ifndef POLLUTANTOVERVIEW_H
#define POLLUTANTOVERVIEW_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QTableWidget>
#include <QLineEdit>

class Pollutantoverview : public QWidget {
    Q_OBJECT

public:
    explicit Pollutantoverview(QWidget *parent = nullptr);

private:
    QLineEdit *searchBar;
    QTableWidget *table;
    QtCharts::QChartView *createChart();  // Corrected return type
    QTableWidget *createComplianceTable();
    void populateTable();
    void setupSearchFunction();

    private slots:
        void handleSearch();
    void showPollutantDetails(int row, int column);
    void showChartDataTooltip(const QPointF &point, bool state);
};

#endif // POLLUTANTOVERVIEW_H
