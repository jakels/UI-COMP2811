#ifndef FLUORINATEDPAGE_H
#define FLUORINATEDPAGE_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QStringList>
#include <QLabel>

class FluorinatedPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit FluorinatedPage(QWidget *parent = nullptr);
    ~FluorinatedPage();

private:
    // Member variables
    QVBoxLayout *summaryCardLayout;
    QComboBox *filterDropdownType;
    QComboBox *filterDropdownLocation;
    QComboBox *filterDropdownCompliance;

    // Navigation bar
    QHBoxLayout *createNavigationBar();
    QPushButton *createNavigationButton(const QString &text, const QString &style);

    // Filter sections
    QWidget *createTypeFilterSection();
    QWidget *createLocationFilterSection();
    QWidget *createComplianceFilterSection();

    // Summary card section
    QWidget *createSummarySection();
    QWidget *createSummaryCard(const QString &title, const QString &color);

    // Visualization section
    QWidget *createVisualizationSection();

    // Compliance logic
    void updateComplianceStatus();
};

#endif // FLUORINATEDPAGE_H