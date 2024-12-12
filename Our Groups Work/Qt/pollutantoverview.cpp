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
    "QPushButton {"
 "  background-color: #457B9D; color: white; border-radius: 5px; "
 "  padding: 10px 20px; font-size: 14px; font-weight: bold; }"
 "QPushButton:hover { background-color: #1D3557; }"
 "QPushButton:pressed { background-color: #14213D; }"
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

    chart->setTitle("Determinand Trends Over Time");

    // Connect hovered signal
    connect(series, &QLineSeries::hovered, this, &Pollutantoverview::showChartDataTooltip);

    return new QChartView(chart);
}

QTableWidget *Pollutantoverview::createComplianceTable() {
    table = new QTableWidget(8, 4, this); // Adjust row count dynamically later
    table->setHorizontalHeaderLabels({"Determinand", "Level", "Location", "Status"});
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
    QStringList levels;
    QStringList statuses;
    QStringList locations;
    QStringList riskDescriptions;

    // Load in data from the backend
    std::vector<WaterQualitySample> query = DB_GetCachedEntriesSubset(2000);
    int numberOfSamples = query.size();

    for (int i = 0; i < numberOfSamples; i++) {
        WaterQualitySample sample = query[i];
        QString resultStr = QString::fromStdString(sample.result);
        auto determinandLabel = sample.determinandLabel.c_str();
        auto level = atoi(sample.result.c_str());
        pollutants.append(determinandLabel);
        locations.append(sample.samplingPointLabel.c_str());
        levels.append(resultStr);

        statuses.append(SAMPLE_GetSafetyLevel(sample).c_str());

        riskDescriptions.append("?");
    }

    table->setRowCount(pollutants.size());

    for (int i = 0; i < pollutants.size(); ++i) {
        QTableWidgetItem *pollutantItem = new QTableWidgetItem(pollutants[i]);
        QTableWidgetItem *levelItem = new QTableWidgetItem(levels[i]);
        QTableWidgetItem *statusItem = new QTableWidgetItem(statuses[i]);
        QTableWidgetItem *locationItem = new QTableWidgetItem(locations[i]);

        pollutantItem->setToolTip(riskDescriptions[i]);
        levelItem->setToolTip(riskDescriptions[i]);
        locationItem->setToolTip(locations[i]);
        statusItem->setToolTip("Status: " + statuses[i] + "\n" + riskDescriptions[i]);

        // Color-code statuses
        if (statuses[i] == "Safe") {
            //statusItem->setBackground(Qt::green);
            statusItem->setBackground(QColor ("#008000"));
        } else if (statuses[i] == "Caution") {
            statusItem->setBackground(QColor("#FFBF00")); // Amber color
        } else {
            statusItem->setBackground(Qt::red);
        }

        table->setItem(i, 0, pollutantItem);
        table->setItem(i, 1, levelItem);
        table->setItem(i, 2, locationItem);
        table->setItem(i, 3, statusItem);

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
    QString description;

    // Generate dynamic description based on status
    if (status == "Safe") {
        description = "The pollutant level is within the safe threshold, posing minimal or no risk to health.";
    } else if (status == "Caution") {
        description = "The pollutant level is nearing the upper limit of safety. Monitoring and caution are advised.";
    } else if (status == "Danger") {
        description = "The pollutant level exceeds the safe threshold, posing significant health and environmental risks.";
    } else {
        description = "No specific information available for this status. Please review the pollutant details.";
    }

    // Update chart data dynamically
    updateChartData(pollutant.toStdString());

    // Check for data existence and avoid out-of-bounds issues
    std::string determinandDefinition;
    auto chemicalEntries = DB_GetEntriesByChemical(pollutant.toStdString());
    if (!chemicalEntries.empty()) {
        determinandDefinition = chemicalEntries[0].determinandDefinition;
    } else {
        determinandDefinition = "No additional details available for this pollutant.";
    }

    // Construct the message
    QString message = QString("Pollutant: %1\nLevel: %2 ppm\nStatus: %3\n\nDescription: %4\n\nDetails: %5")
                          .arg(pollutant)
                          .arg(level)
                          .arg(status)
                          .arg(description)
                          .arg(QString::fromStdString(determinandDefinition));

    // Set the target chemical for further use
    targetChemical = pollutant.toStdString();

    // Display the message
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