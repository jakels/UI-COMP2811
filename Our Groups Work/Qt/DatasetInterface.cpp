/* DatasetInterface.cpp
    This file is the interface for the water dataset, it has functions for initialising the DB and querying it.
    You cannot use the database without first initialising it with DB_Initialise
*/

// Imports
#include "DatasetInterface.h"
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include "sqlite3.h"
#include "Utils.h"
#include <cmath>

// Global variables
std::string CSVfilepath = "Water.csv";
std::vector<WaterQualitySample> cachedDataset;
int cachedDatasetSize = 0;
bool initialised = false;

// Print utility function
void WaterQualitySample::print() const
{
    std::cout << "Sample ID: " << id
              << ", Sampling Point: " << samplingPoint
              << ", Result: " << result
              << ", Date: " << sampleDateTime
              << std::endl;
}

std::vector<WaterQualitySample> DB_GetCachedEntries()
{
    Log("Getting cached entries");
    return cachedDataset;
}

std::vector<WaterQualitySample> DB_GetCachedEntriesSubset(int count)
{
    Log("Getting cached entries subset of size " + std::to_string(count));
    std::vector<WaterQualitySample> samples;
    for (int i = 0; i < count; i++)
    {
        samples.push_back(cachedDataset[i]);
    }
    return samples;
}


// Custom comparator to sort by the 'result' field
bool compareByResult(const WaterQualitySample& a, const WaterQualitySample& b) {
    return a.sampleDateTime < b.sampleDateTime;
}

std::vector<WaterQualitySample> OrderSamplesByDate(std::vector<WaterQualitySample> input)
{
    Log("Ordering vector of size " + std::to_string(input.size()));

    std::sort(input.begin(), input.end(), compareByResult);

    return input;
}

// Function to load the CSV file into a vector of WaterQualitySample objects
std::vector<WaterQualitySample> DB_GetAllEntries(const std::string& filePath)
{
    std::vector<WaterQualitySample> samples;
    csv::CSVReader reader(filePath);
    std::vector<std::string> chemicals;

    int i = 0;
    int counter = 0;

    // Iterate through the rows of the CSV
    for (auto& row : reader)
    {
        if(counter == 25000)
        {
            Log("Loaded " + std::to_string(i));
            counter = 0;
        }
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
        sample.headers = reader.get_col_names();
        sample.rawRow = row;
        sample.safeMax = 0.001;
        sample.cautionMax = 0.002;

        if(std::find(chemicals.begin(), chemicals.end(), sample.determinandLabel) != chemicals.end()) {

        } else {
            chemicals.push_back(sample.determinandLabel);
        }

        samples.push_back(sample);
        i++;
        counter++;
    }
    Log("Finished loading samples at " + std::to_string(i) + ", " + std::to_string(chemicals.size()) + " unique chemicals.");

    return samples;
}

// Initialise the database by loading all the CSV data into a SQL db
int DB_Initialise()
{
    Log("Initialising database...");
    cachedDataset = DB_GetAllEntries(GetAbsoloutePathFromRelative(CSVfilepath));
    cachedDatasetSize = cachedDataset.size();
    initialised = true;

    if(cachedDataset.size() == 0)
    {
        Log("All entries size was 0, returning error code.");
        return 1;
    }

    Log("Initialised database.");
    return 0;
}

std::vector<WaterQualitySample> DB_GetEntriesByLocation(const std::string& specificLocation)
{
    Log("Getting all entries with location of " + specificLocation);
    std::vector<WaterQualitySample> samples;

    // Iterate through the rows of the CSV
    for (int i = 0; i < cachedDatasetSize; i++)
    {
        WaterQualitySample sample = cachedDataset[i];

        if(sample.samplingPointLabel != specificLocation)
        {
            continue;
        }

        samples.push_back(sample);
    }

    Log("Returned a vector with " + std::to_string( samples.size() ) + " elements.");
    return samples;
}


std::vector<WaterQualitySample> DB_GetEntriesByChemical(const std::string& chemical)
{
    Log("Getting all entries with chemical of " + chemical);
    std::vector<WaterQualitySample> samples;

    // Iterate through the rows of the CSV
    for (int i = 0; i < cachedDatasetSize; i++)
    {
        WaterQualitySample sample = cachedDataset[i];

        if(sample.determinandLabel != chemical)
        {
            continue;
        }

        samples.push_back(sample);
    }

    Log("Returned a vector with " + std::to_string( samples.size() ) + " elements.");
    return samples;
}

std::vector<WaterQualitySample> DB_GetEntriesByChemicalAndLocation(const std::string& chemical, const std::string& location)
{
    Log("Getting all entries with location of " + location + " and chemical of " + chemical);
    std::vector<WaterQualitySample> samples;

    // Iterate through the rows of the CSV
    for (int i = 0; i < cachedDatasetSize; i++)
    {
        WaterQualitySample sample = cachedDataset[i];

        if(sample.determinandLabel != chemical || sample.samplingPointLabel != location)
        {
            continue;
        }

        samples.push_back(sample);
    }

    Log("Returned a vector with " + std::to_string( samples.size() ) + " elements.");
    return samples;
}

int DB_GetEntriesByChemicalAndLeastResult(std::vector<WaterQualitySample> dataSetVector, std::string determinand)
{
    double minimumResult = 999.999;
    int minimumRow = 0;
    for (int i = 0; i < std::min(dataSetVector.size(), static_cast<size_t>(140733)); ++i) {
        WaterQualitySample current = dataSetVector[i];
        if (current.determinandLabel != determinand) {
            continue;
        }
        if (stod(current.result) < minimumResult) {
            minimumRow = i;
            minimumResult = stod(current.result);
        }
    }

    dataSetVector[minimumRow].print();
    return minimumRow;
}

bool SAMPLE_CheckSampleThreshold(WaterQualitySample sample, std::string thresholdType)
{
    if(thresholdType == "Safe")
    {
        return atof(sample.result.c_str()) < sample.safeMax;
    }
    else if(thresholdType == "Caution")
    {
        return atof(sample.result.c_str()) < sample.cautionMax;
    }
    else if (thresholdType == "Danger")
    {
        return atof(sample.result.c_str()) > sample.cautionMax;
    }
}

int SAMPLES_NumberOfEntriesWithLevelType(std::vector<WaterQualitySample> samples, std::string level)
{
    int total = 0;
    for(int i = 0; i < samples.size(); i++)
    {
        if(SAMPLE_CheckSampleThreshold(samples[i], level))
        {
            total++;
        }
    }

    return total;
}

int SAMPLES_NumberOfSafeEntries(std::vector<WaterQualitySample> samples)
{
    return SAMPLES_NumberOfEntriesWithLevelType(samples, "Safe");
}

int SAMPLES_NumberOfCautionEntries(std::vector<WaterQualitySample> samples)
{
    return SAMPLES_NumberOfEntriesWithLevelType(samples, "Caution");
}

int SAMPLES_NumberOfDangerEntries(std::vector<WaterQualitySample> samples)
{
    return SAMPLES_NumberOfEntriesWithLevelType(samples, "Danger");
}

std::string SAMPLE_GetSafetyLevel(WaterQualitySample sample)
{
    if(SAMPLE_CheckSampleThreshold(sample, "Safe"))
    {
        return "Safe";
    }

    if(SAMPLE_CheckSampleThreshold(sample, "Caution"))
    {
        return "Caution";
    }

    if(SAMPLE_CheckSampleThreshold(sample, "Danger"))
    {
        return "Danger";
    }

    return "NULL";
}