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

std::string targetChemical = "Endrin";

Pollutantoverview::Pollutantoverview(QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create search bar
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search pollutants...");
    searchBar->setStyleSheet("padding: 8px; font-size: 14px; border: 1px solid #ccc; border-radius: 5px;");

    // Create a search button
    searchButton = new QPushButton("Search", this);
    searchButton->setStyleSheet(
        "padding: 8px; "
        "font-size: 14px; "
        "border: 1px solid #007BFF; "
        "border-radius: 5px; "
        "background-color: #007BFF; "
        "color: white; "
        "transition: all 0.2s ease-in-out; "
        "hover: { background-color: #0056b3; transform: scale(1.02); box-shadow: 0 5px 10px rgba(0, 0, 0, 0.2); }"
    );
    connect(searchButton, &QPushButton::clicked, this, &Pollutantoverview::handleSearch);

    // Add search bar and button to a horizontal layout
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchBar);
    searchLayout->addWidget(searchButton);

    // Add components to layout
    layout->addLayout(searchLayout);
    layout->addWidget(createChart());
    layout->addWidget(createComplianceTable());
    setLayout(layout);
}

QLineSeries *series;
QDateTimeAxis *axisX;
QValueAxis *axisY;
QChartView *Pollutantoverview::createChart() {
    QChart *chart = new QChart();

    // Create sample data seriesFluro
    series = new QLineSeries();

    // Load in data from the backend, get all the samples that had the chemical
    std::vector<WaterQualitySample> query = OrderSamplesByDate(DB_GetEntriesByChemical(targetChemical));
    int numberOfSamples = query.size();
    double maximumResult = 0;

    // Loop through the samples
    for (int i = 0; i < numberOfSamples; i++) {
        WaterQualitySample sample = query[i];
        auto sampleDate = sample.sampleDateTime.c_str();
        double sampleResult = atof(sample.result.c_str());

        if (sampleResult > maximumResult) {
            maximumResult = sampleResult;
        }

        // Add the sample data to the chart
        series->append(QDateTime::fromString(sampleDate, Qt::ISODate).toMSecsSinceEpoch(), sampleResult);
    }

    chart->addSeries(series);

    // Configure axes
    axisX = new QDateTimeAxis();
    axisX->setFormat("yyyy-MM-dd HH:mm");
    axisX->setTitleText("Time");

    axisY = new QValueAxis();
    axisY->setTitleText(targetChemical.c_str());
    axisY->setLabelFormat("%0.8f");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Add safe threshold as a reference line
    QLineSeries *threshold = new QLineSeries();
    //threshold->append(0, 20);
    //threshold->append(4, 20);
    //threshold->setName("Safe Threshold");

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
    table->setHorizontalHeaderLabels({"Pollutant", "Level", "Status"});
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
    QStringList pollutants;
    QList<double> levels;
    QStringList statuses;
    QStringList riskDescriptions;

    // Load in data from the backend
    std::vector<WaterQualitySample> query = DB_GetCachedEntriesSubset(2000);
    int numberOfSamples = query.size();

    for (int i = 0; i < numberOfSamples; i++) {
        WaterQualitySample sample = query[i];
        double sampleResult = atof(sample.result.c_str());

        // Debugging
        //Log("Sample " + std::to_string(i) + " has date " + sampleDate + " and result " + std::to_string(sampleResult));

        // If the sample result is bigger than max result then set max result to the sample result
        //if(sampleResult > maximumResult)
        //{
        //    maximumResult = sampleResult;
        //}

        // Important part, add the data to the lists
        auto determinandLabel = sample.determinandLabel.c_str();
        auto level = atoi(sample.result.c_str());
        pollutants.append(determinandLabel);
        levels.append(level);

        statuses.append(SAMPLE_GetSafetyLevel(sample).c_str());

        riskDescriptions.append("?");
    }

    table->setRowCount(pollutants.size());

    for (int i = 0; i < pollutants.size(); ++i) {
        QTableWidgetItem *pollutantItem = new QTableWidgetItem(pollutants[i]);
        QTableWidgetItem *levelItem = new QTableWidgetItem(QString::number(levels[i], 'f', 8));
        QTableWidgetItem *statusItem = new QTableWidgetItem(statuses[i]);

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
    QString query = searchBar->text().toLower().trimmed();
    qDebug() << "Search triggered for:" << query;

    for (int row = 0; row < table->rowCount(); ++row) {
        bool match = false;
        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem *item = table->item(row, col);
            if (item && item->text().toLower().contains(query)) {
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
    updateChartData(pollutant.toStdString());

    QString message = QString("Pollutant: %1\nLevel: %2 ppm\nStatus: %3\n\nDescription: %4")
                          .arg(pollutant)
                          .arg(level)
                          .arg(status)
                          .arg(DB_GetEntriesByChemical(pollutant.toStdString())[0].determinandDefinition.c_str());

    targetChemical = pollutant.toStdString();
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

void Pollutantoverview::updateChartData(std::string chemName) {
    // Clear old data
    series->clear();

    // Load in data from the backend, get all the samples that had the chemical
    std::vector<WaterQualitySample> query = OrderSamplesByDate(DB_GetEntriesByChemical(chemName));
    int numberOfSamples = query.size();
    double maximumResult = 0;

    // Loop through the samples
    for (int i = 0; i < numberOfSamples; i++) {
        WaterQualitySample sample = query[i];
        auto sampleDate = sample.sampleDateTime.c_str();
        double sampleResult = atof(sample.result.c_str());

        if (sampleResult > maximumResult) {
            maximumResult = sampleResult;
        }

        // Add the sample data to the chart
        series->append(QDateTime::fromString(sampleDate, Qt::ISODate).toMSecsSinceEpoch(), sampleResult);
    }

    // Update the Y axis range if we have data
    if (!query.empty()) {
        axisY->setRange(0, maximumResult+0.1);
        axisY->setTitleText(chemName.c_str());
    } else {
        // If no data, reset to default range
        axisY->setRange(0, 1);
        QMessageBox::information(this, "No data", "There was no data for your query.");
    }
}