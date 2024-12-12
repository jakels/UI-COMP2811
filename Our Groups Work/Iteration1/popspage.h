// #ifndef POPSPAGE_H
// #define POPSPAGE_H
//
// #include "DatasetInterface.h"
// #include <QWidget>
// #include <QPushButton>
// #include <QComboBox>
// #include <QVBoxLayout>
// #include <QLabel>
// #include <vector>
// #include <string>
// #include <QToolTip>
// #include <QHeaderView>
// #include <QTableWidget>
// #include <QtCharts/QChartView>
// #include <QtCharts/QChart>
// #include <QtCharts/QLineSeries>
// #include <QtCharts/QDateTimeAxis>
// #include <QtCharts/QValueAxis>
//
// class PopsPage : public QWidget
// {
//     Q_OBJECT
//
// public:
//     explicit PopsPage(QWidget *parent = nullptr); // 构造函数
//
// private slots:
//     void showRowDetails(int row, int column);  // 显示表格单行详细信息的槽函数
//
// private:
//     void loadPopData(const std::string &filter); // 加载 POP 数据的方法
//     QChart *createChart();                      // 创建初始图表
//     void updateChart(const std::vector<WaterQualitySample> &filteredSamples); // 更新图表数据的方法
//     void showChartDataTooltip(const QPointF &point, bool state);             // 显示图表数据的 Tooltip
//     void loadDefaultTableData();                // 加载默认表格数据的方法
//
//     QLabel *infoLabel;                          // 显示加载信息的标签
//     QComboBox *popSelector;                     // POPs 下拉选择框
//     QPushButton *loadDataButton;                // 加载数据按钮
//     QChartView *chartView;                      // 用于显示图表的视图
//     QTableWidget *tableWidget;                  // 用于显示污染物数据的表格
// };
//
// #endif // POPSPAGE_H
