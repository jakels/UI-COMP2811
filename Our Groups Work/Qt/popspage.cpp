#include "popspage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <vector>
#include <string>
#include <QTableWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QDateTime>
#include <QToolTip>
#include <QtCharts/QChartView>
#include <map>
#include "constants.h"

PopsPage::PopsPage(QWidget *parent) : QWidget(parent)
{
    // Create the main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mainLayout->addLayout(createFilters());

    // ===== Information Display Section =====
    infoLabel = new QLabel("Please select a pollutant to load data", this);
    mainLayout->addWidget(infoLabel);

    // Create the chart and add it to the layout
    chartView = new QChartView(createChart(), this);
    mainLayout->addWidget(chartView);

    // Create the POP selection dropdown
    popSelector = new QComboBox(this);
    for(auto pop : popPollutants)
    {
        popSelector->addItem(pop);
    }

    mainLayout->addWidget(popSelector);

    // Create the Load Data button
    loadDataButton = new QPushButton("Load Data", this);
    mainLayout->addWidget(loadDataButton);

    // Create the table
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3); // Adjust the number of columns
    tableWidget->setHorizontalHeaderLabels({"Pollutant", "Level", "Safe"});
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->verticalHeader()->setVisible(false);
    mainLayout->addWidget(tableWidget);

    // Connect the Load Data button click event
    connect(loadDataButton, &QPushButton::clicked, this, [this]() {
        QString selectedPop = popSelector->currentText();
        loadPopData(selectedPop.toStdString());
    });

    // Load default table data
    loadPopData(popSelector->currentText().toStdString());

    // Connect table click signals to the corresponding slot
    connect(tableWidget, &QTableWidget::cellClicked, this, &PopsPage::showRowDetails);

    setLayout(mainLayout);
}

void PopsPage::loadPopData(const std::string &filter)
{
    try {
        // Retrieve all data
        std::vector<WaterQualitySample> allSamples = DB_GetCachedEntriesSubset(50000);

        // Define the fields to be filtered
        std::vector<std::string> filters = {};
        for(auto popType : popPollutants)
        {
            filters.push_back(popType.toStdString());
        }

        // Define a map to accumulate totals for each field
        std::map<std::string, double> pollutantSums;

        // Filter and accumulate the fields
        for (const auto &sample : allSamples) {
            std::string lowerLabel = sample.determinandLabel;
            std::transform(lowerLabel.begin(), lowerLabel.end(), lowerLabel.begin(), ::tolower);

            for (const auto &field : filters) {
                std::string lowerFilter = field;
                std::transform(lowerFilter.begin(), lowerFilter.end(), lowerFilter.begin(), ::tolower);

                if (lowerLabel.find(lowerFilter) != std::string::npos) {
                    pollutantSums[field] += std::stod(sample.result);
                }
            }
        }

        // If no data is found
        if (pollutantSums.empty()) {
            QMessageBox::information(this, "No Data", "No data found for the selected pollutants.");
            return;
        }

        // Update the number of rows in the table
        tableWidget->setRowCount(pollutantSums.size());
        int row = 0;

        for (const auto &[pollutant, sum] : pollutantSums) {
            // Fill the Pollutant column
            QTableWidgetItem *pollutantItem = new QTableWidgetItem(QString::fromStdString(pollutant));
            tableWidget->setItem(row, 0, pollutantItem);

            // Fill the Level column
            QTableWidgetItem *levelItem = new QTableWidgetItem(QString::number(sum, 'f', 2));
            tableWidget->setItem(row, 1, levelItem);

            // Fill the Safe column
            QString safeStatus = "Safe";
            if(sum > 5)
            {
                safeStatus = "Caution";
            }
            if(sum > 8)
            {
                safeStatus = "Danger";
            }
            QTableWidgetItem *safeItem = new QTableWidgetItem(safeStatus);
            tableWidget->setItem(row, 2, safeItem);

            // Set the color of the Safe column
            safeItem->setBackground((safeStatus == "Safe") ? Qt::green : Qt::red);

            // Add a button
            /*QPushButton *detailsButton = new QPushButton("Details", tableWidget);
            tableWidget->setCellWidget(row, 3, detailsButton);

            // Connect the button click signal to the corresponding slot
            connect(detailsButton, &QPushButton::clicked, this, [this, pollutant, sum, safeStatus]() {
                QMessageBox::information(
                        this,
                        "Pollutant Details",
                        QString("Pollutant: %1\nLevel: %2\nStatus: %3")
                                .arg(QString::fromStdString(pollutant))
                                .arg(QString::number(sum, 'f', 2))
                                .arg(safeStatus)
                );
            });*/

            ++row;
        }

        // Update the chart and only display the trend for the selected category
        std::vector<WaterQualitySample> filteredSamples;
        for (const auto &sample : allSamples) {
            std::string lowerLabel = sample.determinandLabel;
            std::string lowerFilter = filter;
            std::transform(lowerLabel.begin(), lowerLabel.end(), lowerLabel.begin(), ::tolower);
            std::transform(lowerFilter.begin(), lowerFilter.end(), lowerFilter.begin(), ::tolower);

            if (lowerLabel.find(lowerFilter) != std::string::npos) {
                filteredSamples.push_back(sample);
            }
        }

        updateChart(OrderSamplesByDate(filteredSamples));

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("Failed to load data: %1").arg(e.what()));
    }
}

QChart* PopsPage::createChart()
{
    QChart *chart = new QChart();
    chart->setTitle("Pollutant Trends Over Time");

    // Set initial empty data
    QLineSeries *series = new QLineSeries();
    chart->addSeries(series);

    // Configure X and Y axes
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("yyyy-MM-dd");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Pollutant Level (ppm)");
    axisY->setLabelFormat("%0.2f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    return chart;
}

void PopsPage::updateChart(const std::vector<WaterQualitySample> &filteredSamples)
{
    QChart *chart = chartView->chart();

    // Clear existing data and axes
    chart->removeAllSeries();

    QList<QAbstractAxis *> axes = chart->axes();
    for (QAbstractAxis *axis : axes) {
        chart->removeAxis(axis);
    }

    QLineSeries *series = new QLineSeries();

    // Add data to the line chart
    for (const auto &sample : filteredSamples) {
        QDateTime time = QDateTime::fromString(QString::fromStdString(sample.sampleDateTime), Qt::ISODate);
        double result = std::stod(sample.result);
        series->append(time.toMSecsSinceEpoch(), result);
    }

    chart->addSeries(series);

    // Update the range of the axes
    if (!filteredSamples.empty()) {
        QDateTimeAxis *axisX = new QDateTimeAxis();
        axisX->setFormat("yyyy-MM-dd");
        axisX->setTitleText("Time");
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("Pollutant Level (ppm)");
        axisY->setLabelFormat("%0.2f");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
    }

    // Connect the mouse hover event
    connect(series, &QLineSeries::hovered, this, &PopsPage::showChartDataTooltip);
}

void PopsPage::showChartDataTooltip(const QPointF &point, bool state)
{
    if (state) {
        // Get the position of the mouse
        QString dateTime = QDateTime::fromMSecsSinceEpoch(point.x()).toString("yyyy-MM-dd");
        QString value = QString::number(point.y(), 'f', 2);  // Format to two decimal places

        // Display the tooltip
        QToolTip::showText(QCursor::pos(),
                           QString("Time: %1\nValue: %2 ppm").arg(dateTime).arg(value),
                           chartView);
    } else {
        QToolTip::hideText();
    }
}

void PopsPage::showRowDetails(int row, int column)
{
    QString pollutant = tableWidget->item(row, 0)->text();
    QString level = tableWidget->item(row, 1)->text();
    QString safeStatus = tableWidget->item(row, 2)->text();

    QMessageBox::information(
            this,
            "Pollutant Details",
            QString("Pollutant: %1\nLevel: %2\nStatus: %3")
                    .arg(pollutant)
                    .arg(level)
                    .arg(safeStatus)
    );
}

QHBoxLayout *PopsPage::createFilters() {
    QHBoxLayout *filterLayout = new QHBoxLayout();

    QLabel *timeLabel = new QLabel("Filter by Time:");
    timeLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1D3557;");
    QComboBox *timeFilter = new QComboBox();
    timeFilter->addItems({"All Time", "Last Month", "Last Year"});
    timeFilter->setStyleSheet("padding: 8px; font-size: 14px; border: 1px solid #457B9D; border-radius: 5px; color: #333;");
    timeFilter->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    timeFilter->setMinimumWidth(150);
    timeFilter->setMinimumContentsLength(10);

    QLabel *regionLabel = new QLabel("Filter by Region:");
    regionLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1D3557;");
    QComboBox *regionFilter = new QComboBox();
    regionFilter->addItems({"All Regions", "Region A", "Region B", "Region C"});
    regionFilter->setStyleSheet("padding: 8px; font-size: 14px; border: 1px solid #457B9D; border-radius: 5px; color: #333;");
    regionFilter->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    regionFilter->setMinimumWidth(150);
    regionFilter->setMinimumContentsLength(10); // adjusts width based on number of characters

    QPushButton *applyFilterButton = new QPushButton("Apply Filters");
    applyFilterButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #457B9D; color: white; border-radius: 5px; "
        "  padding: 10px 20px; font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1D3557; }"
        "QPushButton:pressed { background-color: #14213D; }"
    );

    connect(applyFilterButton, &QPushButton::clicked, this, [=]() {
        QString selectedTime = timeFilter->currentText();
        QString selectedRegion = regionFilter->currentText();
        qDebug() << "Filters applied: Time -" << selectedTime << ", Region -" << selectedRegion;
    });

    filterLayout->addWidget(timeLabel);
    filterLayout->addWidget(timeFilter);
    filterLayout->addSpacing(20);
    filterLayout->addWidget(regionLabel);
    filterLayout->addWidget(regionFilter);
    filterLayout->addSpacing(20);
    filterLayout->addWidget(applyFilterButton);
    filterLayout->addStretch();

    return filterLayout;
}