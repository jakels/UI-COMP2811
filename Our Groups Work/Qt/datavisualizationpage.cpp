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
    std::vector<WaterQualitySample> samples = DB_GetEntriesByChemicalAndLocation("Endrin", "MALHAM TARN");

    tableWidget->setRowCount(5); // Example: 5 rows

    QStringList headers = {"timestamp"};
    tableWidget->setColumnCount(headers.count()); // Example: 3 columns
    tableWidget->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < 5; ++row)
    {
        WaterQualitySample current = samples[row];

        std::cout << row << "" << current.sampleDateTime << std::endl;
        continue;
        for (int col = 0; col < headers.count(); ++col)
        {
            std::string colDisplay = "";
            switch (col)
            {
                default:
                    break;
                case 0:
                    colDisplay = current.sampleDateTime;
                    break;
                case 1:
                    colDisplay = current.samplingPoint;
                    break;
                case 2:
                    colDisplay = current.samplingPointNotation;
                    break;
                case 3:
                    colDisplay = current.samplingPointLabel;
                    break;
            }


            QTableWidgetItem *item = new QTableWidgetItem(QString(colDisplay.c_str()));
            tableWidget->setItem(row, col, item);
        }
    }
}
