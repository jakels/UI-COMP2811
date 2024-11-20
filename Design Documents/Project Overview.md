# Project Overview: Water Quality Monitoring Application

# Objective: Develop an application to help users query, analyze, and export water quality data for the UK/EU regions.

# Target Audience: Water companies, government agencies, the World Health Organization (WHO), environmental protection organizations, etc.

## Core Features:
1.**Data Query**: Allow users to search for specific water quality information based on time, location, and pollution levels.
2.**Data Export**: Provide functionality to export data for external use.

## Technical Choices:
1. **Framework**: The application will be built using the Qt framework.
2. **Fonts**:
   Primary data will be displayed using the Source Code Pro Regular font, ensuring clarity and readability.
   Titles and key information will be emphasized with bold and larger font sizes to make them more noticeable.
3. **Colors**:
   1. A common and simple color scheme of black (#000000) font on a white (#FFFFFF) background.
   2. Alternatively, a softer color combination of gray (#444444) font on an off-white (#F5F5F5) background may be used.

## Interface Design:
1. **Entry Point**: Ensure a clear entry point for users starting the application.
2. **Navigation**: Implement an intuitive navigation system within the app.
3. **User Interaction**: Data queries, exports, analysis, and report generation,etc.
4. **Error Handling**: Provide prompts for incorrect user inputs during queries.
5. **Data Presentation**: Enhance readability by paginating data displays.
6. **Exit Mechanism**: Direct close or designated exit button.

## Data Handling:
1. **Database**:Choose SQLite for managing complex data due to its proficiency over handling more extensive datasets than CSV.
2. **Database Connection**: Utilize Qt’s MySQL driver, including the correct headers, and establish connections using the QSqlDatabase class.

## Customer Flow:
1. **Application Launch**: Users double-click the application icon to open it.
2. **Main Interface**: Upon launching, users are greeted with a clear navigation menu outlining the application's features.
3. **Data Query Interface**: Users navigate to the data query interface where they can input parameters such as time to retrieve relevant water quality data presented in tables or other formats.
4. **Error Handling**: If a user inputs incorrect information during a search, the application provides error feedback.
5. **Data Export**: In the data query interface, users can export the desired data by clicking an “Export Data” button; the application confirms the successful completion of the task.
6. **Application Closure**: Users close the application by clicking the “Close” button.

