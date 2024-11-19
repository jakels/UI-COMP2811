# 1. Project Overview and Objectives

### Objective
This project challenges you to develop a Qt-based desktop application that visualises UK/EU water quality data, with an emphasis on accessibility, internationalisation, and clear data presentation. The application should enable people to explore and understand water pollutant levels and safety compliance across various geographic locations.

### Learning Goals
This project enables you to:
- **Apply UI/UX principles**: Design a responsive, accessible interface that accommodates diverse needs and supports internationalisation.
- **Manage large datasets**: Efficiently handle and display extensive water quality data, either through direct CSV reading or by using SQLite for database management.
- **Follow an iterative design process**: Complete three development iterations, incorporating feedback and refining the application based on usability testing.
- **Document your development journey**: Create a structured final report covering design decisions, feedback integration, and evaluations.

### Project Deliverables
Each team is required to submit the following:

1. **Application**: A working Qt desktop application that meets the core project requirements and includes:
    - A dashboard that provides a summary of water quality data.
    - Detailed pages for various pollutant categories, with visual indicators of compliance.
    - An accessible, responsive UI design with clear, data-driven visualisations.

2. **Final Report**:
    - A six-page report summarising the design process, evaluation findings, feedback integration, and final implementation.
    - The report should document the iterative design process, detailing how feedback was incorporated to improve usability and accessibility.

3. **Videos**:
    - **Three Iteration Videos**: Document each development cycle, highlighting prototyping, feedback, and changes made.
        - **Length**: Each iteration video should be between 45 seconds to 1 minute and 20 seconds.
    - **One Showcase Video**: Present a final overview of the application, emphasising key features, accessibility, and usability.
        - **Length**: The showcase video should be between 1 minute and 20 seconds to 1 minute and 45 seconds.
----
# 2. Project Scenario

### Scenario Context
You have been hired to develop a **pollutant monitoring application** that displays water quality data across the UK and EU. This application aims to provide a comprehensive overview of water pollutant levels, safety compliance, and geographic patterns to various people, including environmental researchers, policymakers, and the general public.

### Application Purpose
The application should make it easy for people to:
- **Explore pollutant levels**: View data on common pollutants, persistent organic pollutants (POPs), fluorinated compounds, and litter indicators.
- **Evaluate compliance**: The application should visually indicate whether pollutant levels comply with UK/EU safety standards using colour-coded indicators.
- **Identify geographic patterns**: People should have the option to view geographic hotspots where pollutant levels frequently exceed safety limits.

### Expected Requirements
To make this data actionable and easy to interpret, the application should meet the following requirements:

1. **Data Visualisation**:
    - Use intuitive visualisations, such as time-series and bar charts, to present pollutant levels over time and compare pollutant levels across different locations.
    - Implement a traffic-light colour scheme (green, amber, red) to indicate whether pollutant levels fall within safe limits, are nearing thresholds, or exceed recommended limits.

2. **Compliance Indicators**:
    - Clearly display whether pollutant levels are within UK/EU safety standards.
    - Allow people to filter data by pollutant type, geographic location, or compliance status to see where and which pollutants require attention.

3. **Interactive Map** (optional for advanced implementation):
    - If feasible, integrate a map view to display geographic hotspots, highlighting locations with elevated pollutant levels.
    - Provide colour-coded markers on the map to represent pollutant concentrations and compliance status at various sampling locations.

4. **Accessibility**:
    - Ensure an accessible experience that meets basic accessibility standards. This includes sufficient colour contrast, screen reader labels, and keyboard navigation.
    - Include internationalisation (i18n) options to make the app accessible to non-English-speaking audiences, allowing for a basic selection of supported languages.

### Target Audience
The app should be user-friendly for a range of target audiences:
- **Environmental Researchers**: Access comprehensive pollutant data and track environmental trends.
- **Policymakers**: View summary dashboards and compliance indicators to make informed decisions.
- **General Public**: Quickly interpret pollutant levels, compliance information, and geographical patterns to stay informed about water quality.
---
# 3. Core Application Features and Page Requirements

### Exploring the Starter Code

To support your development, a starter codebase has been provided. This code is primarily focused on loading data from a CSV file and displaying a subset of it in a table using the model/view architecture. The main purpose of this code is to demonstrate how to decouple data presentation from data storage, a key concept for handling large datasets efficiently in Qt applications.

#### Starter Code Guidance and Expectations

- **Data Page Requirement**: 
  - To fully utilise the starter code, your application should include at least one page dedicated to displaying pollutant data in a table format. 
  - This "Data Page" allows us to browse raw or pre-filtered data and helps your group apply the model/view approach demonstrated in the starter code.
- **Achievable starting point for Subsequent Pages**: 
  - Implementing the data page first, by modifying the starter code to handle the water quality dataset, can provide an achievable starting point for week one, supporting implementing the remaining analysis-focused pages.
- **Analysis Pages**: 
  - For the further required pages, you may use the starter code’s data handling approach as a reference, even if you don’t fully integrate the model class. 
  - The analysis pages will primarily focus on presenting statistical summaries, trends, and visualisations based on the data, which you can implement in custom view components.

#### Suggested Workflow

1. **Start with the Data Page**:
    - Modify the starter code’s data loading and table display functions to handle your dataset.
    - Display selected columns relevant to pollutants (e.g., time, pollutant name, concentration, location) to make the data page intuitive and usable.

2. **Develop Analysis Pages Next**:
    - Build additional pages focusing on specific pollutant analyses, such as trends, compliance thresholds, geographical distribution, or summary statistics.
    - Reference the model/view architecture as needed to structure data access, or implement custom methods to filter and present data in alternative visual formats.

    - The application should include a **main dashboard** and at least **4-5 dedicated pages** to meet the core functionality requirements. 
    - Each page should display pollutant data with compliance indicators and include basic navigation and accessibility features.

## Dashboard Overview

The **main dashboard** provides an at-a-glance summary of key pollutant data and links to more detailed pages. It should give people a quick overview of pollutant categories, average pollutant levels, and compliance status across locations.

### Dashboard Layout Requirements

1. **Header**:
    - **Application Title**: Display the application name (e.g., “Water Quality Monitor”).
    - **Navigation**: Include links or buttons to each of the main data visualisation pages.
    - **Language Selector**: Offer a language selection option if implementing internationalisation.

2. **Main Content**:
    - **Cards for Each Pollutant Category**: Present pollutant overview cards for each category (e.g., Pollutant Overview, POPs, Litter Indicators, Fluorinated Compounds, Compliance).
        - **Summary Information**: Each card should include a brief summary of pollutant levels and a compliance indicator using a traffic-light colour scheme.
        - **Interactive Links**: Allow each card to link to its detailed page.
    - **Quick Filters**: Provide options to filter by time range (e.g., last month, last year) or geographic region.

3. **Footer**:
    - **Help Links**: Include links to user guides, help resources, and credits for data sources.

### Responsive Design Requirements for the Dashboard

- **Responsive Layout**: Ensure the dashboard layout adjusts smoothly across desktop and tablet screens, including adaptive resizing for cards and charts.
- **Touch-Friendly**: Ensure all interactive elements are accessible for touch users with sufficient size and spacing.

---

## Required Pages (Minimum of 4-5)

Each of the following pages should include relevant data visualisations, rollover or pop-up content for additional details, and compliance indicators to help people understand pollutant safety status.

1. **Pollutant Overview Page**
    - **Purpose**: Display detailed information on common pollutants like **1,1,2-Trichloroethane** and **Chloroform**.
    - **Key Elements**:
        - **Search Function**: Include a search bar to help people locate pollutants by name.
        - **Visualisation**: Use time-series or bar charts to show pollutant trends over time.
        - **Compliance Indicators**: Colour-code pollutant levels (e.g., green for safe, amber for caution, red for exceeding safe levels).
        - **Pop-Ups**: Provide additional information on pollutant risks, compliance status, and safety thresholds when people hover or click on data points.

2. **Persistent Organic Pollutants (POPs) Page**
    - **Purpose**: Present data on **PCBs** and other persistent organic pollutants due to their long-lasting impact on the environment and health.
    - **Key Elements**:
        - **Data Trends**: Use line charts or similar visualisations to show POP levels at various sampling points over time.
        - **Rollover Pop-Ups**: Include additional information on health risks, monitoring importance, and safety levels.
        - **Compliance Colour-Coding**: Show compliance with UK/EU safety levels using colour-coded indicators.

3. **Environmental Litter Indicators Page**
    - **Purpose**: Summarise physical pollutants, such as **plastic litter** and other visible debris in water.
    - **Key Elements**:
        - **Litter Comparisons**: Provide comparisons of litter levels across different locations and water body types, using bar or pie charts.
        - **Compliance Indicators**: Show adherence to EU Bathing Water standards regarding litter presence.
        - **Interactive Elements**: Allow people to filter by location or type of litter to explore trends.

4. **Fluorinated Compounds Page**
    - **Purpose**: Display levels of **PFAS** and other fluorinated compounds, which are monitored for their environmental persistence.
    - **Key Elements**:
        - **Map or Time-Series Visualisation**: Show distribution of fluorinated compounds across sites, using either a map view with colour-coded markers or time-series charts.
        - **Compliance Indicators**: Use traffic-light colours to indicate levels relative to safety thresholds (e.g., 0.1 µg/L for PFAS).
        - **Rollover Pop-Ups**: Provide contextual information on PFAS persistence and health/environmental implications.

5. **Compliance Dashboard**
    - **Purpose**: Offer an overview of regulatory compliance across all pollutants, showing which substances meet or exceed safety standards.
    - **Key Elements**:
        - **Summary Cards**: Display compliance status for each pollutant with a quick colour-coded indicator.
        - **Filters**: Allow people to filter by location, pollutant, or compliance status to see which areas require attention.
        - **Pop-Up Details**: Include summaries for non-compliant sites, explaining possible causes or historical data trends.

6. **Geographical Hotspots Page** (Optional for Advanced Implementation)
    - **Purpose**: Display a map-based view to highlight sampling locations with elevated pollutant levels, helping people understand geographic distribution and pollution sources.
    - **Key Elements**:
        - **Interactive Map**: Use **QtLocation** to display sampling points, with colour-coded markers based on pollutant levels.
        - **Rollover Information**: Show details for each hotspot, such as pollutant type, recent levels, and compliance status.
        - **Filters**: Allow filtering by specific pollutant or geographic region to focus on areas of interest.

---

## Project Expectations and MVP Requirement

- **Minimum Viable Product (MVP)**: At minimum, the app should demonstrate a working dashboard, basic data visualisations for each page, and preliminary compliance indicators. Fully implemented backends are encouraged but not strictly required for all features.

- **Partial Implementations**: Partial or prototyped features are acceptable, especially if the focus is on UI and interaction. However, features with limited functionality will be graded accordingly. Document incomplete or prototyped elements in the final report.

- **Documentation of Challenges**: Be transparent in your report about any challenges encountered with implementation. Describe any incomplete features, the decisions behind them, and outline what would be needed to fully implement them. Honesty and professional documentation of limitations will be credited.

---

## Design Requirements

- **Primary Libraries**:
    - **QtCharts**: Use **QtCharts** as the main library for data visualisations, including line charts, bar charts, pie charts, and other visual elements. This library is well-supported and cross-platform, offering a range of chart types for presenting pollutant trends, compliance status, and comparisons.
    - **CSV Parser**: lib provided in starter code.

- **Approved Additional Libraries**:
  - You can make use of the following additional libraries, but only within the scope of the project. Other libraries are **not approved** (unless agreed in advance with module leaders) to ensure ease of marking and compatibility across all platforms:
    - **QtLocation** and **QtPositioning**: These libraries can be used for map-based features, such as displaying geographic hotspots and pollutant distribution.
    - **QtSQL**: For teams using SQLite or similar for data storage and management, **QtSQL** may be used to integrate and manage large datasets (CSV is recommended for ease of use).
    - **QtMultimedia**: For adding video or media content to the application, QtMultimedia may be used but should remain focused on project-related content (e.g., showcasing pollutant impact).
    - **QtWebView** allows for embedding web content within the application, which could be useful for accessing or displaying online resources related to pollutants, environmental data, or regulatory information.

- **Responsiveness**:
    - Use Qt layout managers (e.g., `QVBoxLayout`, `QHBoxLayout`, `QGridLayout`) to create flexible, responsive layouts that adapt to desktop and tablet screens.

- **Accessibility**:
    - Implement clear colour contrast for compliance indicators, and label interactive elements for screen readers.
    - Ensure keyboard navigation is functional, allowing people to tab through all interactive components.

#### Language Support and Internationalisation
- **Requirement**: 
- The application should respond to changes in locale, supporting internationalisation in a basic form. 
- You are not required to generate full translations but should design the app to allow for them.
- **Implementation Guidance**:
    - Ensure that all strings in the app are translatable by using Qt’s translation mechanisms.
    - While a GUI language selector is not required, the app should respond appropriately if the locale changes.
    - *Optional*: Groups may choose to include translations in a language of their choice to demonstrate the internationalisation feature. Online translation tools or other approximate translations are acceptable.
- **Note**: The focus is on supporting translatability rather than complete language coverage. For demo purposes, changing the locale and rerunning the app should show the app’s capability to adapt to different languages.

#### Card-Based Design Concept
- **Definition of "Card"**: 
- For this project, a "card" refers to a logical grouping of UI components or widgets that serve a specific purpose, such as displaying related data or controls together. Think of it as a visual module within the app, organised for clarity and cohesion.
- **Implementation Suggestions**:
    - Each "card" could be a **QWidget** or **QFrame** within a **QLayout**, allowing flexibility in design.
    - Alternatively, a **QGroupBox** could be used to create a bordered section with a title, providing a more visually distinct grouping.
    - Students are encouraged to create specialised composite widgets by subclassing `QWidget`, which can enhance code structure and readability.
- **Guidance on Structure**: 
- You can structure the "cards" in whichever way best fits your design; well-organised and cohesive solutions will be awarded higher marks. 
- This approach helps modularise the UI and enables a cohesive aesthetic design for the app.

#### Tooltips and Rollovers
- **Definition**: A "rollover popup" is a tooltip that appears when the customer hovers over certain elements, providing additional context or information.
- **Implementation Guidance**:
    - You can implement this using `QToolTip` to provide brief descriptions or additional data for UI elements.
    - Tooltips can improve the usability and accessibility of the app by offering immediate help or explanations without cluttering the interface.

---
# 4. Development Iterations and Customer Feedback Testing

This project follows an **iterative design process** with four development cycles, or “sprints.” Each cycle includes prototyping, implementation, and evaluation, allowing you to refine the application based on testing and feedback.

## Iteration Process and Requirements

### Week 0: Initial Requirements Review and Early Planning

- **Focus**:
    - Review the **project requirements** and **starter code** to understand the structure and expectations for the application.
    - Familiarise yourself with the **water quality dataset**, examining key fields and potential visualisation opportunities. Explore where data transformations (e.g., CSV to SQLite) might simplify handling large datasets.
    - Sketch initial ideas for the **user interface and layout**, including preliminary thoughts on the **dashboard, page navigation**, and "card" groupings.
    - Consider early design elements, such as "cards" for data grouping, tooltips for added information, accessibility requirements, e.g. keyboard navigation, and translatability to support internationalisation.

- **Suggested Activities**:
    - Explore the starter code by running and reviewing its structure. Experiment with minor modifications to understand how the model/view architecture is used.
    - Design simple **wireframes or sketches** of the main dashboard and key pages. This helps clarify the arrangement of cards, data visualisation areas, and key controls.
    - Brainstorm how to group functionality and content into pages, considering modularity and reusability in the UI.
    - Identify any initial questions or challenges around data handling, UI layout, or project requirements that might need clarification.

- **Deliverables**:
    - **Initial Requirements Review Document**: Summarise your understanding of the project requirements, any key insights from the dataset, and the anticipated structure of your application based on the starter code.
    - **Early Sketches and Wireframes**: Provide initial sketches or wireframes for the main dashboard and at least two key pages, highlighting ideas for "cards" and UI layout.
    - **Reflection on Starter Code**: Brief notes on how the starter code will assist in setting up the data page and other components, as well as any potential challenges or limitations observed.
    - **Preliminary Design Document** (optional): Include ideas for initial data handling approaches (e.g., whether to keep data in CSV or convert to SQLite) and any early-stage design thoughts around colour schemes, fonts, and customer flow.


### Iteration 1: Initial Application Setup and Basic Functionality
- **Focus**:
    - Set up the basic structure and layout of the application, including the main dashboard and initial navigation.
    - Implement initial data loading functionality, either from the CSV file or by converting the data to SQLite.
    - Develop initial data visualisations and compliance indicators on each required page.

- **Deliverables**:
    - A basic, functional version of the dashboard and at least two pages with preliminary pollutant data visualisations.
    - Documentation of design choices, initial challenges, and any early-stage evaluations.

---

### Iteration 2: Expanded Features and Internal Evaluation
- **Focus**:
    - Add additional views, visualisations, and compliance indicators across all required pages.
    - Implement accessibility features, such as colour contrast for compliance indicators and basic keyboard navigation.
    - Begin internationalisation work with a language selector and initial translations if applicable.
    - Conduct an internal evaluation, identifying usability challenges and areas for improvement.

- **Evaluation**:
    - Use an internal heuristic evaluation or cognitive walkthrough to assess usability, focusing on compliance indicators, navigation, and layout clarity.

- **Deliverables**:
    - A more complete application with all required pages, responsive layout adjustments, and initial accessibility and i18n features.
    - Evaluation documentation with a summary of identified issues and planned improvements.

---

### Iteration 3: Customer Feedback Testing and Final Adjustments
- **Focus**:
    - Conduct formal **customer feedback testing** with external participants, gathering feedback on usability, layout clarity, and data accessibility.
    - Make improvements based on feedback, prioritising issues that impact accessibility, navigation, or overall usability.
    - Document all implemented changes, along with any additional challenges or unresolved issues.

- **Required Customer Feedback Testing**:
    - Use recognised usability testing methods, such as:
        - **Heuristic Evaluation** based on Nielsen’s Heuristics.
        - **Shneiderman’s Golden Rules** to assess accessibility and user-friendly design.
        - **Walkthroughs** with typical range of scenarios to identify usability barriers.
    - **Feedback Documentation**: Clearly document all feedback received, specifying what changes were made based on feedback and why certain suggestions were prioritised or deprioritised.

- **Ethics and Consent**:
    - Ensure ethics compliance by using provided participant information sheets and consent forms. Collect feedback anonymously and summarise insights in an ethical and privacy-compliant way.

- **Deliverables**:
    - A refined version of the application with documented improvements.
    - Ethics documentation, including participant consent forms and anonymised feedback records.

---

## Project Expectations for Iterations and Feedback

- **Clear Documentation**: Document each iteration’s process, including what was achieved, what feedback was received, and what changes were made based on that feedback.
- **Evaluation Techniques**: Explain your evaluation techniques for each iteration, detailing the rationale behind each method.
- **Transparency on Challenges**: If certain features or feedback-based adjustments are incomplete, document these challenges and any necessary steps for full implementation.
- **Customer Feedback Transparency**: In the final report, clearly explain how customer feedback influenced your design choices. Mention specific changes, as well as feedback that was not implemented and why.

## Final Report Requirements

- **Page Limit**: The final report has a strict **six-page limit** for the main content. This includes sections on project goals, design choices, iteration summaries, evaluation methods, and results.
    - **Appendices**: You may include additional images, tables, feedback documentation, evaluations, and other supporting material in an appendix. The appendix should be concise and purposeful—avoid using it as a “dumping ground” for excess content. Be judicious in your choices and include only relevant materials.
    - **In-Text References to Appendices**: All appendix items should be referenced explicitly within the main text. This will help maintain focus in the core report while supporting detailed references to additional materials.
    - **References Section**: A separate references section is encouraged and **does not count toward the six-page limit**. References can follow either **University of Leeds** or **ACM citation style**.



### Assessment Focus for Iterations and Feedback Testing
Iterative refinement and feedback as central to achieving a customer-friendly, accessible application. Clear documentation of each iteration’s process, evaluation methods, and feedback integration is key to demonstrate a thoughtful, human-centred development approach.

---

# 5. Evaluation Criteria

Your project will be evaluated based on the following criteria, totaling 100%. 
Each rubric is designed to assess both the technical implementation and the quality of documentation, iterative improvement, and usability considerations.

## Rubric Overview

### Prototype Quality – 20%
- **Stability and Functionality** (3 marks): 
  - Assess the application’s overall stability, responsiveness, and freedom from crashes or bugs.
- **Completeness** (3 marks): 
  - Evaluate whether the application meets all required features, including dashboard functionality, pollutant pages, and basic accessibility.
- **UI and Responsiveness** (5 marks): 
  - Review the overall design, layout consistency, and responsiveness of the UI, ensuring it adapts smoothly across desktop and tablet screens.
- **Improvements and Creativity** (6 marks): 
  - Award marks for creativity and thoughtful improvements made throughout iterations, based on feedback and testing.

### Documentation and Report – 30%
- **Structure and Clarity** (4 marks): 
  - Assess the report’s overall organisation, coherence, and flow. The report should clearly explain each design choice, feedback, and iteration outcome.
- **Arguments and Supporting Information** (6 marks): 
  - Evaluate the strength of arguments made in the report, with evidence supporting design and implementation decisions.
- **Scoping Analysis and Scenario Clarity** (8 marks): 
  - Review the quality of the project’s scoping and scenario analysis, ensuring the project’s focus aligns with monitoring context.
- **Application of UI/UX Theory** (6 marks): 
  - Look for references to relevant UI/UX theories and design principles, applied to design decisions and iteration improvements.
- **Use of Prototyping Techniques** (5 marks): 
  - Assess the variety and appropriateness of prototyping methods used throughout development, and how these methods informed design choices.

### Iterations and Evaluations – 20%
- **Ethics Compliance** (3 marks): 
  - Ensure ethics documentation is complete, including participant consent forms and anonymised feedback data, following ethical standards.
- **Documented Iterative Cycles** (1 mark): 
  - Check that the project includes at least three documented iterations, with iterative improvements across each cycle.
- **Evaluation Techniques** (4 marks): 
  - Review the evaluation methods used (e.g., heuristic evaluations, walkthroughs) and their relevance to the project’s design.
- **Evaluation Reasoning** (2 marks): 
  - Ensure clear reasoning behind each evaluation method and design choice, detailing why specific techniques were chosen.
- **Feedback Integration** (6 marks): 
  - Award marks based on the extent to which customer feedback was incorporated, with clear documentation of implemented and deprioritised feedback.
- **Evidence of Evaluation** (3 marks): 
  - Assess the quality and clarity of evaluation evidence, including tables, anonymised images, and summarised feedback data.

### Video Submissions – 20%
- **Iteration Videos** (3 marks): 
  - Evaluate the submission of three iteration videos, showcasing development progression, feedback, and adjustments.
- **Showcase Video** (1 mark): 
  - Ensure submission of a final showcase video demonstrating the app’s main features and accessibility.
- **Video Length Compliance** (4 marks): 
  - Check that all videos meet the required lengths (iterations: 45s-1m20s; showcase: 1m20s-1m45s).
- **Clarity and Accessibility** (2 marks): 
  - Review the quality, clarity, and accessibility of each video. Videos should be understandable on first viewing, with accessible features like subtitles where possible.
- **Process and Evolution** (6 marks): 
  - Award marks for evidence of development process and iterative evolution across videos, highlighting major improvements and design choices.
- **Audience-Appropriate Showcase** (2 marks): 
  - Assess the showcase video for its suitability for a general (non-technical) audience, effectively highlighting key features and usability aspects.

### Module Participation – 10%
- **Individual Feedback and Participation**: 
  - This will be assessed based on engagement and participation throughout the module, including completion of peer feedback assignments on Feedback Fruits.

---

## Marking Guidance

- **Minimum Viable Product (MVP)**: 
  - An MVP with basic dashboard, data visualisations, and compliance indicators will meet minimum requirements but may receive lower marks. Fully implemented features and thoughtful enhancements will be rewarded.
- **Documentation Requirements**: 
  - The six-page report is a strict limit; however, appendices may include additional supporting materials (e.g., images, tables, feedback). Each appendix item should be explicitly referenced in the main text, and references are allowed beyond the six-page limit in UoLeeds or ACM format.
- **Transparency in Iterations**: 
  - Be upfront about limitations, incomplete features, and design challenges. Document what was accomplished in each iteration, along with feedback integration and prioritisation decisions.

---

# Submission Contents

### Deadline
- **Date**: 2pm, Thursday, 12th December 2024
- **Submission**: Only one team member should submit the files on behalf of the entire team.

### Submission Format
- **File Format**: Submit a single compressed ZIP file containing all required components.
- **File Naming**: Name the ZIP file as follows: `COMP2811_TeamName.zip`, replacing `TeamName` with your assigned team name.

### Required Contents
The ZIP file should include the following:

1. **Application Folder**:
    - Include all source code, configuration files, and assets required to compile and run the application.
    - **Compatibility**: Ensure the application is compatible with the Faculty of Engineering and Physical Sciences (FEPS) Red Hat system, which runs on all lab computers and is available on the Feng Linux VM.
    - **Testing Requirement**: Test your application thoroughly on the FEPS Red Hat system to ensure it runs as expected. Non-compatible applications may affect grading.

2. **README File** (`README.md`):
    - Include a **detailed README** in Markdown format with the following information:
        - **Team Members**: Full names and student IDs of each team member.
        - **Project Overview**: Brief description of the application, its purpose, and main features.
        - **Setup Instructions**: Step-by-step guide on how to compile, configure, and run the application on the FEPS Red Hat system.
        - **Dependencies**: List all libraries and dependencies required, including any setup steps if additional libraries beyond the standard Qt installation are needed.
        - **File Structure**: Outline the main structure of the application folder, explaining key folders and files.
        - **Known Issues**: Any known limitations or unresolved bugs should be documented in this section.

3. **Final Report** (`report.pdf`):
    - A six-page PDF document following the format and requirements outlined in Section 4, including appendices as needed (with explicit in-text references).

4. **Video Files**:
    - **Iteration Videos**: Three videos (between 45 seconds and 1 minute 20 seconds) demonstrating development progress and changes made across each iteration.
    - **Showcase Video**: One final showcase video (between 1 minute 20 seconds and 1 minute 45 seconds) that provides a comprehensive overview of the application and highlights its main features.
    - **Format**: All videos should be provided in standard video formats compatible with most media players. **Do not submit links to online platforms** (e.g., YouTube, Vimeo); all videos must be accessible offline in the ZIP file.
    - **File Size**: Keep video file sizes reasonable to avoid excessively large ZIP files. Aim for a **maximum file size of 100 MB per video**, using video compression if necessary without compromising quality.

5. **Ethics Documentation**:
    - Include participant consent forms and any relevant information sheets from customer feedback testing in a folder labeled `Ethics Documentation`.

---

### Communication Guidelines
- **General Questions**: For any general questions about the project, please use the **COMP2811 Teams channel**. This allows everyone in the module to benefit from shared information and responses.
- **Private or Personal Inquiries**: For matters of a personal or private nature, please contact the module leader directly via email or Teams.

---

### Additional Notes
- **Documentation Completeness**: Double-check that the final report and appendices contain all necessary information to support the project’s requirements, evaluations, and design decisions.
- **File Organisation**: Organise the ZIP file for easy navigation, with clearly labeled folders for each component.

