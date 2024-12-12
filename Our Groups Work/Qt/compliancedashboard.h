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
    void resizeEvent(QResizeEvent *event) override; // Handle resizing

private slots:
    void applyFilters();
    void showDetails(const Data &sample);

// protected:
//     void resizeEvent(QResizeEvent *event) override;

private:
    void displaySummaryCards(const std::vector<Data> &samples);
    int calculateColumns(); // Determine the number of columns dynamically

    QScrollArea *scrollArea;         // To make cards scrollable
    QWidget *scrollContent;          // Content widget inside the scroll area
    QGridLayout *cardContainer;      // Grid layout to hold the cards
    QComboBox *locationComboBox;
    QComboBox *pollutantComboBox;
    QComboBox *statusComboBox;

    std::vector<Data> mockData;
    int cardWidth;  // Dynamically adjustable card width
    int cardHeight; // Dynamically adjustable card height
};

#endif // COMPLIANCEDASHBOARD_H

