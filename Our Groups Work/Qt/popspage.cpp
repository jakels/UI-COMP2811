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

PopsPage::PopsPage(QWidget *parent) : QWidget(parent)
{


    // 创建主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);



    // ===== 信息显示部分 =====
    infoLabel = new QLabel("Please select a pollutant to load data", this);
    mainLayout->addWidget(infoLabel);

    // 创建图表并添加到布局
    chartView = new QChartView(createChart(), this);
    mainLayout->addWidget(chartView);

    // 创建 POP 下拉选择框
    popSelector = new QComboBox(this);
    popSelector->addItem("Pops Overall");
    popSelector->addItem("Aldrin");
    popSelector->addItem("Chlordane");
    popSelector->addItem("DDT");
    popSelector->addItem("Endrin");
    popSelector->addItem("Heptachlor");
    popSelector->addItem("Hexachlorobenzene");
    popSelector->addItem("Mirex");
    popSelector->addItem("PCB");
    popSelector->addItem("PCD");
    popSelector->addItem("Toxaphene");

    mainLayout->addWidget(popSelector);

    // 创建加载数据按钮
    loadDataButton = new QPushButton("Load Data", this);
    mainLayout->addWidget(loadDataButton);

    // 创建表格
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3); // 调整列数
    tableWidget->setHorizontalHeaderLabels({"Pollutant", "Level", "Safe"});
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->verticalHeader()->setVisible(false);
    mainLayout->addWidget(tableWidget);

    // 加载数据按钮点击事件
    connect(loadDataButton, &QPushButton::clicked, this, [this]() {
        QString selectedPop = popSelector->currentText();
        loadPopData(selectedPop.toStdString());
    });

    // 加载默认表格数据
    loadDefaultTableData();

    // 连接表格点击信号到槽函数
    connect(tableWidget, &QTableWidget::cellClicked, this, &PopsPage::showRowDetails);

    setLayout(mainLayout);
}
void PopsPage::loadDefaultTableData()
{
    // 获取所有数据
    std::vector<WaterQualitySample> allSamples = DB_GetCachedEntries();

    // 定义需要筛选的字段
    std::vector<std::string> filters = {"Aldrin", "Chlordane", "DDT", "Endrin", "Heptachlor", "Hexachlorobenzene", "Mirex", "PCB", "PCD", "Toxaphene"};

    // 定义 map 用于统计每个字段的总和
    std::map<std::string, double> pollutantSums;

    // 累加每个污染物的结果
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

    // 更新表格行数
    tableWidget->setRowCount(pollutantSums.size());
    int row = 0;

    for (const auto &[pollutant, sum] : pollutantSums) {
        // 填充 Pollutant 列
        QTableWidgetItem *pollutantItem = new QTableWidgetItem(QString::fromStdString(pollutant));
        tableWidget->setItem(row, 0, pollutantItem);

        // 填充 Level 列
        QTableWidgetItem *levelItem = new QTableWidgetItem(QString::number(sum, 'f', 2));
        tableWidget->setItem(row, 1, levelItem);

        // 填充 Safe 列
        QString safeStatus = (sum < 10.0) ? "Yes" : "No"; // 设定安全阈值为 10
        QTableWidgetItem *safeItem = new QTableWidgetItem(safeStatus);
        tableWidget->setItem(row, 2, safeItem);

        // 设置安全列颜色
        safeItem->setBackground((safeStatus == "Yes") ? Qt::green : Qt::red);

        // 添加 Details 按钮
        QPushButton *detailsButton = new QPushButton("Details", this);
        tableWidget->setCellWidget(row, 3, detailsButton);

        // 连接按钮点击信号到槽函数
        connect(detailsButton, &QPushButton::clicked, this, [this, pollutant, sum, safeStatus]() {
            QMessageBox::information(
                this,
                "Pollutant Details",
                QString("Pollutant: %1\nLevel: %2\nStatus: %3")
                    .arg(QString::fromStdString(pollutant))
                    .arg(QString::number(sum, 'f', 2))
                    .arg(safeStatus)
            );
        });

        ++row;
    }
}

void PopsPage::loadPopData(const std::string &filter)
{
    try {
        // 获取所有数据
        std::vector<WaterQualitySample> allSamples = DB_GetCachedEntries();

        // 定义需要筛选的字段
        std::vector<std::string> filters = {"DDT", "Endrin", "PCB", "Aldrin", "Chlordane", "Mirex", "Hexachlorobenzene"};

        // 定义 map 用于统计每个字段的总和
        std::map<std::string, double> pollutantSums;

        // 筛选并累加字段
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

        // 如果没有数据
        if (pollutantSums.empty()) {
            QMessageBox::information(this, "No Data", "No data found for the selected pollutants.");
            return;
        }

        // 更新表格行数
        tableWidget->setRowCount(pollutantSums.size());
        int row = 0;

        for (const auto &[pollutant, sum] : pollutantSums) {
            // 填充 Pollutant 列
            QTableWidgetItem *pollutantItem = new QTableWidgetItem(QString::fromStdString(pollutant));
            tableWidget->setItem(row, 0, pollutantItem);

            // 填充 Level 列
            QTableWidgetItem *levelItem = new QTableWidgetItem(QString::number(sum, 'f', 2));
            tableWidget->setItem(row, 1, levelItem);

            // 填充 Safe 列
            QString safeStatus = (sum < 10.0) ? "Yes" : "No"; // 设定安全阈值为 10
            QTableWidgetItem *safeItem = new QTableWidgetItem(safeStatus);
            tableWidget->setItem(row, 2, safeItem);

            // 设置安全列颜色
            safeItem->setBackground((safeStatus == "Yes") ? Qt::green : Qt::red);

            // 添加按钮
            QPushButton *detailsButton = new QPushButton("Details", tableWidget);
            tableWidget->setCellWidget(row, 3, detailsButton);

            // 连接按钮点击信号到槽函数
            connect(detailsButton, &QPushButton::clicked, this, [this, pollutant, sum, safeStatus]() {
                QMessageBox::information(
                    this,
                    "Pollutant Details",
                    QString("Pollutant: %1\nLevel: %2\nStatus: %3")
                        .arg(QString::fromStdString(pollutant))
                        .arg(QString::number(sum, 'f', 2))
                        .arg(safeStatus)
                );
            });

            ++row;
        }

        // 更新图表，只绘制所选分类的趋势图
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

    // 设置空的初始数据
    QLineSeries *series = new QLineSeries();
    chart->addSeries(series);

    // 配置 X 和 Y 轴
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

    // 清空现有数据和坐标轴
    chart->removeAllSeries();

    QList<QAbstractAxis *> axes = chart->axes();
    for (QAbstractAxis *axis : axes) {
        chart->removeAxis(axis);
    }

    QLineSeries *series = new QLineSeries();

    // 添加数据到折线图
    for (const auto &sample : filteredSamples) {
        QDateTime time = QDateTime::fromString(QString::fromStdString(sample.sampleDateTime), Qt::ISODate);
        double result = std::stod(sample.result);
        series->append(time.toMSecsSinceEpoch(), result);
    }

    chart->addSeries(series);

    // 更新坐标轴范围
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

    // 连接鼠标悬停事件
    connect(series, &QLineSeries::hovered, this, &PopsPage::showChartDataTooltip);
}

void PopsPage::showChartDataTooltip(const QPointF &point, bool state)
{
    if (state) {
        // 获取鼠标所在位置
        QString dateTime = QDateTime::fromMSecsSinceEpoch(point.x()).toString("yyyy-MM-dd");
        QString value = QString::number(point.y(), 'f', 2);  // 格式化为小数点后两位

        // 显示 ToolTip
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

