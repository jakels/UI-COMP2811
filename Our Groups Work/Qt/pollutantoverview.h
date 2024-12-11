#ifndef POLLUTANTOVERVIEW_H
#define POLLUTANTOVERVIEW_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>

class Pollutantoverview : public QWidget {
    Q_OBJECT

public:
    explicit Pollutantoverview(QWidget *parent = nullptr);

private:
    QLineEdit *searchBar;
    QPushButton *searchButton;
    QTableWidget *table;
    QChartView *createChart();
    QTableWidget *createComplianceTable();
    void populateTable();
    void setupSearchFunction();

    private slots:
        void handleSearch();
    void showPollutantDetails(int row, int column);
    void showChartDataTooltip(const QPointF &point, bool state);
    void updateChartData(std::string chemName);
};

#endif // POLLUTANTOVERVIEW_H

