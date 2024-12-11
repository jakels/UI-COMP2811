#ifndef DATAVISUALIZATIONPAGE_H
#define DATAVISUALIZATIONPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>

class DataVisualizationPage : public QWidget
{
    Q_OBJECT

public:
    explicit DataVisualizationPage(QWidget *parent = nullptr);

private:
    QTableWidget *tableWidget;

    void setupTable();
};

#endif // DATAVISUALIZATIONPAGE_H
