#include "datavisualizationpage.h"

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
    tableWidget->setColumnCount(3); // Example: 3 columns

    QStringList headers = {"Column 1", "Column 2", "Column 3"};
    tableWidget->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 3; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(QString("Item %1,%2").arg(row + 1).arg(col + 1));
            tableWidget->setItem(row, col, item);
        }
    }
}
