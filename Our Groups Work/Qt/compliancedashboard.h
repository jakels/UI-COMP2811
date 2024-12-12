#ifndef COMPLIANCEDASHBOARD_H
#define COMPLIANCEDASHBOARD_H

#include <QGridLayout>
#include <QScrollArea>
#include <QComboBox>
#include <vector>
#include <string>

struct Data {
    std::string location;
    std::string pollutant;
    std::string status;
};

class ComplianceDashboard : public QWidget
{
    Q_OBJECT

public:
    ComplianceDashboard(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void applyFilters();
    void showDetails(const Data &sample);

private:
    void displaySummaryCards(const std::vector<Data> &samples);
    int calculateColumns();

    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QGridLayout *cardContainer;
    QComboBox *locationComboBox;
    QComboBox *pollutantComboBox;
    QComboBox *statusComboBox;

    std::vector<Data> mockData;
    int cardWidth;
    int cardHeight;
};

#endif // COMPLIANCEDASHBOARD_H

