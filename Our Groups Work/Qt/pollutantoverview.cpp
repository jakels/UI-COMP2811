#include "pollutantoverview.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QToolTip>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QCursor>
#include <QDebug>
#include "DatasetInterface.h"
#include "Utils.h"
#include <QGridLayout>
#include <QFrame>
#include <QDebug>
#include <QtCharts>
#include <QDateTime>
#include <QToolTip>

Pollutantoverview::Pollutantoverview(QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create search bar
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search pollutants...");
    searchBar->setStyleSheet("padding: 8px; font-size: 14px; border: 1px solid #ccc; border-radius: 5px;");
    connect(searchBar, &QLineEdit::textChanged, this, &Pollutantoverview::handleSearch);

    // Add components to layout
    layout->addWidget(searchBar);
    layout->addWidget(createChart());
    layout->addWidget(createComplianceTable());
    setLayout(layout);
}

QChartView *Pollutantoverview::createChart() {
    QChart *chart = new QChart();

    // Create sample data series
    QLineSeries *series = new QLineSeries();

    // Load in data from the back end, get all the samples that had the chemical
    std::vector<WaterQualitySample> query = OrderSamplesByDate(DB_GetEntriesByChemical("Endrin"));
    // Get the number of samples
    int numberOfSamples = query.size();
    // Set a variable to hold our maximum result which we will use for scaling axis
    double maximumResult = 0;

    // Loop through the samples
    for(int i = 0; i < numberOfSamples; i++)
    {
        // Get the current sample
        WaterQualitySample sample = query[i];
        // Get the date of the current sample
        auto sampleDate = sample.sampleDateTime.c_str();
        // Get the result of the current sample
        double sampleResult = atof(sample.result.c_str());

        // Debugging
        Log("Sample " + std::to_string(i) + " has date " + sampleDate + " and result " + std::to_string(sampleResult));

        // If the sample result is bigger than max result then set max result to the sample result
        if(sampleResult > maximumResult)
        {
            maximumResult = sampleResult;
        }

        // Important part, add the sample data to the chart
        series->append(QDateTime::fromString(sampleDate, Qt::ISODate).toMSecsSinceEpoch(), sampleResult);
    }

    chart->addSeries(series);

    // Configure axes
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("yyyy-MM-dd HH:mm");
    axisX->setTitleText("Time");

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Pollutant Level (ppm)");
    axisY->setLabelFormat("%0.8f");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Add safe threshold as a reference line
    QLineSeries *threshold = new QLineSeries();
    threshold->append(0, 20);
    threshold->append(4, 20);
    threshold->setName("Safe Threshold");

    chart->addSeries(threshold);
    threshold->attachAxis(axisX);
    threshold->attachAxis(axisY);

    chart->setTitle("Pollutant Trends Over Time");

    // Connect hovered signal
    connect(series, &QLineSeries::hovered, this, &Pollutantoverview::showChartDataTooltip);

    return new QChartView(chart);
}

QTableWidget *Pollutantoverview::createComplianceTable() {
    table = new QTableWidget(8, 3, this); // Adjust row count dynamically later
    table->setHorizontalHeaderLabels({"Pollutant", "Level (ppm)", "Status"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);

    // Populate the table
    populateTable();

    // Connect table click signal
    connect(table, &QTableWidget::cellClicked, this, &Pollutantoverview::showPollutantDetails);

    return table;
}

void Pollutantoverview::populateTable()
{
    QStringList pollutants = { };
    QList<int> levels = { };
    QStringList statuses = { };
    QStringList riskDescriptions = { };

    // Load in data from the back end, get all the samples that had the chemical
    std::vector<WaterQualitySample> query = OrderSamplesByDate(DB_GetEntriesByChemical("Endrin"));
    // Get the number of samples
    int numberOfSamples = query.size();
    // Set a variable to hold our maximum result which we will use for scaling axis
    double maximumResult = 0;

    // Loop through the samples
    for(int i = 0; i < numberOfSamples; i++)
    {
        // Get the current sample
        WaterQualitySample sample = query[i];
        // Get the date of the current sample
        auto sampleDate = sample.sampleDateTime.c_str();
        // Get the result of the current sample
        double sampleResult = atof(sample.result.c_str());

        // Debugging
        Log("Sample " + std::to_string(i) + " has date " + sampleDate + " and result " + std::to_string(sampleResult));

        // If the sample result is bigger than max result then set max result to the sample result
        if(sampleResult > maximumResult)
        {
            maximumResult = sampleResult;
        }

        // Important part, add the data to the lists
        pollutants.append(sample.determinandLabel.c_str());
        levels.append(atoi(sample.result.c_str()));
        // Processing needs to be done on these to determine if safe or not etc
        statuses.append("Safe");
        riskDescriptions.append("?");
    }

    table->setRowCount(pollutants.size());

    for (int i = 0; i < pollutants.size(); ++i) {
        QTableWidgetItem *pollutantItem = new QTableWidgetItem(pollutants[i]);
        QTableWidgetItem *levelItem = new QTableWidgetItem(QString::number(levels[i]));
        QTableWidgetItem *statusItem = new QTableWidgetItem(statuses[i]);

        // Set tooltips with descriptions
        pollutantItem->setToolTip(riskDescriptions[i]);
        levelItem->setToolTip(riskDescriptions[i]);
        statusItem->setToolTip("Status: " + statuses[i] + "\n" + riskDescriptions[i]);

        // Color-code statuses
        if (statuses[i] == "Safe") {
            statusItem->setBackground(Qt::green);
        } else if (statuses[i] == "Caution") {
            statusItem->setBackground(QColor("#FFBF00")); // Amber color
        } else {
            statusItem->setBackground(Qt::red);
        }

        table->setItem(i, 0, pollutantItem);
        table->setItem(i, 1, levelItem);
        table->setItem(i, 2, statusItem);
    }
}

void Pollutantoverview::handleSearch() {
    QString query = searchBar->text().toLower();
    qDebug() << "Search triggered for:" << query;

    for (int row = 0; row < table->rowCount(); ++row) {
        bool match = false;
        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem *item = table->item(row, col);
            if (item->text().toLower().contains(query)) {
                match = true;
                break;
            }
        }
        table->setRowHidden(row, !match);
    }
}

void Pollutantoverview::showPollutantDetails(int row, int column) {
    QString pollutant = table->item(row, 0)->text();
    QString level = table->item(row, 1)->text();
    QString status = table->item(row, 2)->text();
    QString description = table->item(row, 0)->toolTip();

    QString message = QString("Pollutant: %1\nLevel: %2 ppm\nStatus: %3\n\nDescription: %4")
                          .arg(pollutant)
                          .arg(level)
                          .arg(status)
                          .arg(description);

    QMessageBox::information(this, "Pollutant Details", message);
}

void Pollutantoverview::showChartDataTooltip(const QPointF &point, bool state) {
    if (state) {
        QToolTip::showText(QCursor::pos(), QString("Time: %1 months\nLevel: %2 ppm")
                                               .arg(point.x())
                                               .arg(point.y()));
    } else {
        QToolTip::hideText();
    }
}
