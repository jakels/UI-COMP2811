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
#include "translation_manager.h"

PopsPage::PopsPage(QWidget *parent) : QWidget(parent)
{
    // Create the main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mainLayout->addLayout(createFilters());

    // ===== Information Display Section =====
    infoLabel = new QLabel(t("Please select a pollutant to load data").c_str(), this);
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
    loadDataButton = new QPushButton(t("Load Data").c_str(), this);
    mainLayout->addWidget(loadDataButton);

    // Create the table
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3); // Adjust the number of columns
    tableWidget->setHorizontalHeaderLabels({t("Pollutant").c_str(), t("Level").c_str(), t("Safe").c_str()});
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->verticalHeader()->setVisible(false);
    mainLayout->addWidget(tableWidget);

    // Connect the Load Data button click event
    connect(loadDataButton, &QPushButton::clicked, this, [this]() {
        QString selectedPop = popSelector->currentText();
        loadPopData(selectedPop.toStdString(), "All Regions", "All Time");
    });

    // Load default table data
    loadPopData(popSelector->currentText().toStdString(), "All Regions", "All Time");

    // Connect table click signals to the corresponding slot
    connect(tableWidget, &QTableWidget::cellClicked, this, &PopsPage::showRowDetails);

    setLayout(mainLayout);
}

void PopsPage::loadPopData(const std::string &filter, const std::string &region, const std::string &time)
{
    try
    {
        // Retrieve all data
        std::vector<WaterQualitySample> allSamples = DB_GetCachedEntries();
        if(region != t("All Regions"))
        {
            allSamples = DB_GetEntriesByLocation(region);
        }

        if(time == t("Last Month"))
        {
            std::vector<WaterQualitySample> newSamples;
            for(auto sample : allSamples)
            {
                if(SAMPLE_WithinLastMonth(sample))
                {
                    newSamples.push_back(sample);
                }
            }
            newSamples = allSamples;
        }

        if(time == t("Last Year"))
        {
            std::vector<WaterQualitySample> newSamples;
            for(auto sample : allSamples)
            {
                if(SAMPLE_WithinLastYear(sample))
                {
                    newSamples.push_back(sample);
                }
            }
            newSamples = allSamples;
        }

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
            QMessageBox::information(this, t("No Data").c_str(), t("No data found for the request.").c_str());
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
            QString safeStatus = t("Safe").c_str();
            if(sum > 5)
            {
                safeStatus = t("Caution").c_str();
            }
            if(sum > 8)
            {
                safeStatus = t("Danger").c_str();
            }
            QTableWidgetItem *safeItem = new QTableWidgetItem(safeStatus);
            tableWidget->setItem(row, 2, safeItem);

            // Set the color of the Safe column
            safeItem->setBackground((safeStatus == t("Safe").c_str()) ? QBrush(QColor("#008000")) : Qt::red);

            // Add a button
            /*QPushButton *detailsButton = new QPushButton(t("Details").c_str(), tableWidget);
            tableWidget->setCellWidget(row, 3, detailsButton);

            // Connect the button click signal to the corresponding slot
            connect(detailsButton, &QPushButton::clicked, this, [this, pollutant, sum, safeStatus]() {
                QMessageBox::information(
                        this,
                        t("Pollutant Details").c_str(),
                        QString("%1").arg(QString(t("Pollutant: %1\nLevel: %2\nStatus: %3").c_str())
                                .arg(QString::fromStdString(pollutant))
                                .arg(QString::number(sum, 'f', 2))
                                .arg(safeStatus))
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

        updateChart(OrderSamplesByDate(allSamples));

    } catch (const std::exception &e) {
        QMessageBox::critical(this, t("Error").c_str(), QString(t("No data for request.").c_str()).arg(e.what()));
    }
}

QChart* PopsPage::createChart()
{
    QChart *chart = new QChart();
    chart->setTitle(t("Pollutant Trends Over Time").c_str());

    // Set initial empty data
    QLineSeries *series = new QLineSeries();
    chart->addSeries(series);

    // Configure X and Y axes
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat(t("yyyy-MM-dd").c_str());
    axisX->setTitleText(t("Time").c_str());
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(t("Pollutant Level (ppm)").c_str());
    axisY->setLabelFormat(t("%0.2f").c_str());
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
        axisX->setFormat(t("yyyy-MM-dd").c_str());
        axisX->setTitleText(t("Time").c_str());
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText(t("Pollutant Level (ppm)").c_str());
        axisY->setLabelFormat(t("%0.2f").c_str());
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
        QString dateTime = QDateTime::fromMSecsSinceEpoch(point.x()).toString(t("yyyy-MM-dd").c_str());
        QString value = QString::number(point.y(), 'f', 2);  // Format to two decimal places

        // Display the tooltip
        QToolTip::showText(QCursor::pos(),
                           QString(t("Time: %1\nValue: %2 ppm").c_str()).arg(dateTime).arg(value),
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
            t("Pollutant Details").c_str(),
            QString(t("Pollutant: %1\nLevel: %2\nStatus: %3").c_str())
                    .arg(pollutant)
                    .arg(level)
                    .arg(safeStatus)
    );
}

QHBoxLayout *PopsPage::createFilters() {
    QHBoxLayout *filterLayout = new QHBoxLayout();

    QLabel *timeLabel = new QLabel(t("Filter by Time:").c_str());
    timeLabel->setStyleSheet(t("font-size: 16px; font-weight: bold; color: #1D3557;").c_str());
    QComboBox *timeFilter = new QComboBox();
    timeFilter->addItems({t("All Time").c_str(), t("Last Month").c_str(), t("Last Year").c_str()});

    timeFilter->setStyleSheet(t("padding: 8px; font-size: 14px; border: 1px solid #457B9D; border-radius: 5px; color: #333;").c_str());
    timeFilter->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    timeFilter->setMinimumWidth(150);
    timeFilter->setMinimumContentsLength(10);

    QLabel *regionLabel = new QLabel(t("Filter by Region:").c_str());
    regionLabel->setStyleSheet(t("font-size: 16px; font-weight: bold; color: #1D3557;").c_str());
    QComboBox *regionFilter = new QComboBox();
    regionFilter->addItems({t("All Regions").c_str()});
    for(auto region : DB_UniqueLocations())
    {
        regionFilter->addItem(QString::fromStdString(region));
    }
    regionFilter->setStyleSheet(t("padding: 8px; font-size: 14px; border: 1px solid #457B9D; border-radius: 5px; color: #333;").c_str());
    regionFilter->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    regionFilter->setMinimumWidth(150);
    regionFilter->setMinimumContentsLength(10); // adjusts width based on number of characters

    QPushButton *applyFilterButton = new QPushButton(t("Apply Filters").c_str());
    applyFilterButton->setStyleSheet(
            t(
                    "QPushButton {"
                    "  background-color: #457B9D; color: white; border-radius: 5px; "
                    "  padding: 10px 20px; font-size: 14px; font-weight: bold; }"
                    "QPushButton:hover { background-color: #1D3557; }"
                    "QPushButton:pressed { background-color: #14213D; }"
            ).c_str()
    );

    connect(applyFilterButton, &QPushButton::clicked, this, [=]() {
        QString selectedTime = timeFilter->currentText();
        QString selectedRegion = regionFilter->currentText();
        QString selectedPop = popSelector->currentText();
        loadPopData(selectedPop.toStdString(), selectedRegion.toStdString(), selectedTime.toStdString());
        qDebug() << t("Filters applied: Time -").c_str() << selectedTime << t(", Region -").c_str() << selectedRegion;
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
