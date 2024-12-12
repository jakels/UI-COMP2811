#include "datavisualizationpage.h"
#include "DatasetInterface.h"


DataVisualizationPage::DataVisualizationPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    tableWidget = new QTableWidget(this);
    layout->addWidget(tableWidget);

    setupTable();
    setLayout(layout);
}

void DataVisualizationPage::setupTable()
{
    tableWidget->setRowCount(5); // Example: 5 rows

    QStringList headers = {"timestamp"};
    tableWidget->setColumnCount(headers.count()); // Example: 3 columns
    tableWidget->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < 5; ++row)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString("A"));
        tableWidget->setItem(row, 1, item);
    }
}
