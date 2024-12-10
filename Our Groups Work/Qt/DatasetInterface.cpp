//
// Created by Jake on 06/12/2024.
//

#include "DatasetInterface.h"
#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"

void WaterQualitySample::print() const {
    std::cout << "ID: " << id
              << ", Sampling Point: " << samplingPoint
              << ", Result: " << result
              << ", Date: " << sampleDateTime
              << std::endl;
}
// Function to load the CSV file into a vector of WaterQualitySample objects
std::vector<WaterQualitySample> DB_GetAllEntries(const std::string& filePath) {
    std::vector<WaterQualitySample> samples;

    // Open the CSV file using csv-parser
    std::cout << "Loading CSV..." << std::endl;
    csv::CSVReader reader(filePath);
    std::cout << "Loaded CSV." << std::endl;

    // Iterate through the rows of the CSV
    for (auto& row : reader) {
        WaterQualitySample sample;
        sample.id = row["@id"].get<>();
        sample.samplingPoint = row["sample.samplingPoint"].get<>();
        sample.samplingPointNotation = row["sample.samplingPoint.notation"].get<>();
        sample.samplingPointLabel = row["sample.samplingPoint.label"].get<>();
        sample.sampleDateTime = row["sample.sampleDateTime"].get<>();
        sample.determinandLabel = row["determinand.label"].get<>();
        sample.determinandDefinition = row["determinand.definition"].get<>();
        sample.determinandNotation = row["determinand.notation"].get<>();
        sample.resultQualifierNotation = row["resultQualifier.notation"].get<>();
        sample.result = row["result"].get<>();
        sample.codedResultInterpretation = row["codedResultInterpretation.interpretation"].get<>();
        sample.determinandUnitLabel = row["determinand.unit.label"].get<>();
        sample.sampledMaterialTypeLabel = row["sample.sampledMaterialType.label"].get<>();
        sample.isComplianceSample = row["sample.isComplianceSample"].get<>();
        sample.purposeLabel = row["sample.purpose.label"].get<>();
        sample.samplingPointEasting = row["sample.samplingPoint.easting"].get<>();
        sample.samplingPointNorthing = row["sample.samplingPoint.northing"].get<>();

        samples.push_back(sample);
    }

    return samples;
}

std::vector<WaterQualitySample> DB_GetEntriesByLocation(const std::string& filePath, const std::string& specificLocation)
{
    std::vector<WaterQualitySample> samples;

    // Open the CSV file using csv-parser
    std::cout << "Loading CSV..." << std::endl;
    csv::CSVReader reader(filePath);
    std::cout << "Loaded CSV." << std::endl;

    // Iterate through the rows of the CSV
    for (auto& row : reader)
    {
        WaterQualitySample sample;
        sample.id = row["@id"].get<>();
        sample.samplingPoint = row["sample.samplingPoint"].get<>();
        sample.samplingPointNotation = row["sample.samplingPoint.notation"].get<>();
        sample.samplingPointLabel = row["sample.samplingPoint.label"].get<>();
        sample.sampleDateTime = row["sample.sampleDateTime"].get<>();
        sample.determinandLabel = row["determinand.label"].get<>();
        sample.determinandDefinition = row["determinand.definition"].get<>();
        sample.determinandNotation = row["determinand.notation"].get<>();
        sample.resultQualifierNotation = row["resultQualifier.notation"].get<>();
        sample.result = row["result"].get<>();
        sample.codedResultInterpretation = row["codedResultInterpretation.interpretation"].get<>();
        sample.determinandUnitLabel = row["determinand.unit.label"].get<>();
        sample.sampledMaterialTypeLabel = row["sample.sampledMaterialType.label"].get<>();
        sample.isComplianceSample = row["sample.isComplianceSample"].get<>();
        sample.purposeLabel = row["sample.purpose.label"].get<>();
        sample.samplingPointEasting = row["sample.samplingPoint.easting"].get<>();
        sample.samplingPointNorthing = row["sample.samplingPoint.northing"].get<>();

        if(sample.samplingPointLabel != specificLocation)
        {
            continue;
        }

        samples.push_back(sample);
    }

    return samples;
}


std::vector<WaterQualitySample> DB_GetEntriesByChemical(const std::string& filePath, const std::string& chemical)
{
    std::vector<WaterQualitySample> samples;

    // Open the CSV file using csv-parser
    std::cout << "Loading CSV..." << std::endl;
    csv::CSVReader reader(filePath);
    std::cout << "Loaded CSV." << std::endl;

    // Iterate through the rows of the CSV
    for (auto& row : reader)
    {
        WaterQualitySample sample;
        sample.id = row["@id"].get<>();
        sample.samplingPoint = row["sample.samplingPoint"].get<>();
        sample.samplingPointNotation = row["sample.samplingPoint.notation"].get<>();
        sample.samplingPointLabel = row["sample.samplingPoint.label"].get<>();
        sample.sampleDateTime = row["sample.sampleDateTime"].get<>();
        sample.determinandLabel = row["determinand.label"].get<>();
        sample.determinandDefinition = row["determinand.definition"].get<>();
        sample.determinandNotation = row["determinand.notation"].get<>();
        sample.resultQualifierNotation = row["resultQualifier.notation"].get<>();
        sample.result = row["result"].get<>();
        sample.codedResultInterpretation = row["codedResultInterpretation.interpretation"].get<>();
        sample.determinandUnitLabel = row["determinand.unit.label"].get<>();
        sample.sampledMaterialTypeLabel = row["sample.sampledMaterialType.label"].get<>();
        sample.isComplianceSample = row["sample.isComplianceSample"].get<>();
        sample.purposeLabel = row["sample.purpose.label"].get<>();
        sample.samplingPointEasting = row["sample.samplingPoint.easting"].get<>();
        sample.samplingPointNorthing = row["sample.samplingPoint.northing"].get<>();

        if(sample.determinandLabel != chemical)
        {
            continue;
        }

        samples.push_back(sample);
    }

    return samples;
}



std::vector<WaterQualitySample> DB_GetEntriesByChemicalByLocation(const std::string& filePath, const std::string& chemical, const std::string& location)
{
    std::vector<WaterQualitySample> samples;

    // Open the CSV file using csv-parser
    std::cout << "Loading CSV..." << std::endl;
    csv::CSVReader reader(filePath);
    std::cout << "Loaded CSV." << std::endl;

    // Iterate through the rows of the CSV
    for (auto& row : reader)
    {
        // std::cout << row << std::endl;
        WaterQualitySample sample;
        sample.id = row["@id"].get<>();
        sample.samplingPoint = row["sample.samplingPoint"].get<>();
        sample.samplingPointNotation = row["sample.samplingPoint.notation"].get<>();
        sample.samplingPointLabel = row["sample.samplingPoint.label"].get<>();
        sample.sampleDateTime = row["sample.sampleDateTime"].get<>();
        sample.determinandLabel = row["determinand.label"].get<>();
        sample.determinandDefinition = row["determinand.definition"].get<>();
        sample.determinandNotation = row["determinand.notation"].get<>();
        sample.resultQualifierNotation = row["resultQualifier.notation"].get<>();
        sample.result = row["result"].get<>();
        sample.codedResultInterpretation = row["codedResultInterpretation.interpretation"].get<>();
        sample.determinandUnitLabel = row["determinand.unit.label"].get<>();
        sample.sampledMaterialTypeLabel = row["sample.sampledMaterialType.label"].get<>();
        sample.isComplianceSample = row["sample.isComplianceSample"].get<>();
        sample.purposeLabel = row["sample.purpose.label"].get<>();
        sample.samplingPointEasting = row["sample.samplingPoint.easting"].get<>();
        sample.samplingPointNorthing = row["sample.samplingPoint.northing"].get<>();

        if(sample.determinandLabel != chemical && sample.samplingPointLabel != location)
        {
            continue;
        }

        samples.push_back(sample);
    }

    return samples;
}


int DebugDataSet()
{
    const std::string filePath = "yorkshire_water_quality.csv";

    try {
        // Load the data from the CSV file
        std::vector<WaterQualitySample> data = DB_GetAllEntries(filePath);

        // Print the first few rows for verification
        for (size_t i = 0; i < std::min(data.size(), static_cast<size_t>(10)); ++i) {
            data[i].print();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading CSV file: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int MinimumResultRow(std::vector<WaterQualitySample> dataSetVector, std::string determinand)
{
    double minimumResult = 999.999;
    int minimumRow = 0;
    for (int i = 0; i < std::min(dataSetVector.size(), static_cast<size_t>(140733)); ++i)
    {
        WaterQualitySample current = dataSetVector[i];
        if(current.determinandLabel != determinand)
        {
            continue;
        }
        if( stod( current.result ) < minimumResult)
        {
            minimumRow = i;
            minimumResult = stod( current.result );
        }
    }

    dataSetVector[minimumRow].print();
    return minimumRow;
}

class StringList {
private:
    std::vector<std::string> list;

public:
    // Function to check if a string is in the list
    bool contains(const std::string& str) const {
        return std::find(list.begin(), list.end(), str) != list.end();
    }

    // Function to add a string to the list
    void add(const std::string& str) {
        if (!contains(str)) { // Only add if the string is not already in the list
            list.push_back(str);
        } else {
            std::cout << "String \"" << str << "\" is already in the list.\n";
        }
    }

    // Function to print all strings in the list
    void print() const {
        for (const auto& str : list) {
            std::cout << str << "\n";
        }
    }
};

int FindNumberOfUniqueEntries(std::vector<WaterQualitySample> dataSetVector)
{
    StringList myList;

    int uniqueDeterms = 0;
    std::vector<std::string> determinandsSeen;
    for (int i = 0; i < std::min(dataSetVector.size(), static_cast<size_t>(140733)); ++i)
    {
        WaterQualitySample current = dataSetVector[i];
        if(myList.contains(current.determinandLabel) == false)
        {
            myList.add(current.determinandLabel);
            uniqueDeterms++;
        }
    }

    return uniqueDeterms;
}

int SQLCreateDatabase(std::string filename)
{
    sqlite3 *db;
    char *errMsg = nullptr;

    // Open or create a database file
    int exitCode = sqlite3_open(filename.c_str(), &db);

    if (exitCode) {
        // If opening the database failed
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        // If the database was opened successfully
        printf("Database created/opened successfully.\n");
    }

    // SQL statement to create the table
    const char *createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS WaterQuality (
            id TEXT PRIMARY KEY,
            samplingPoint TEXT,
            samplingPointNotation TEXT,
            samplingPointLabel TEXT,
            sampleDateTime TEXT,
            determinandLabel TEXT,
            determinandDefinition TEXT,
            determinandNotation TEXT,
            resultQualifierNotation TEXT,
            result REAL,
            codedResultInterpretation TEXT,
            determinandUnitLabel TEXT,
            sampledMaterialTypeLabel TEXT,
            isComplianceSample TEXT,
            purposeLabel TEXT,
            samplingPointEasting INTEGER,
            samplingPointNorthing INTEGER
        );
    )";

    // Execute the SQL statement
    exitCode = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);

    if (exitCode != SQLITE_OK) {
        // If the table creation failed
        fprintf(stderr, "Error creating table: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        // If the table was created successfully
        printf("Table created successfully.\n");
    }

    // Close the database connection
    sqlite3_close(db);
    return 0;
}


