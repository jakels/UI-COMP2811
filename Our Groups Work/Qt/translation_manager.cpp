//
// Created by Jake on 12/12/2024.
//

#include <fstream>
#include <QStringList>
#include "translation_manager.h"
#include "Utils.h"

QStringList translations = {
    "Sampling Point|نقطة الأخذ|Probenahmestelle",
    "Sampling Point Notation|توضيح نقطة الأخذ|Probenahmestelle Notation",
    "Sampling Point Label|تسمية نقطة الأخذ|Probenahmestelle Etikett",
    "Sample Date Time|تاريخ ووقت العينة|Probenahmedatum und -zeit",
    "Determinand Label|تسمية المحدد|Determinand Etikett",
    "Determinand Definition|تعريف المحدد|Determinand Definition",
    "Determinand Notation|توضيح المحدد|Determinand Notation",
    "Result Qualifier Notation|توضيح نتيجة المؤهل|Ergebnisqualifikator Notation",
    "Result|نتيجة|Ergebnis",
    "Coded Result Interpretation|تفسير النتيجة المشفرة|Kodierte Ergebnisinterpretation",
    "Determinand Unit Label|تسمية وحدة المحدد|Determinand Einheit Etikett",
    "Sampled Material Type Label|تسمية نوع المادة المأخوذة عينة منها|Probenmaterialtyp Etikett",
    "Is Compliance Sample|هل العينة مطابقة|Ist die Probe konform",
    "Purpose Label|تسمية الغرض|Zweck Etikett",
    "Sampling Point Easting|نقطة الأخذ الشرقية|Probenahmestelle Ostung",
    "Sampling Point Northing|نقطة الأخذ الشمالية|Probenahmestelle Nördlich",
    "Safe Max|الحد الأقصى الآمن|Sicherer Max",
    "Caution Max|الحد الأقصى للحذر|Vorsicht Max",
    "Search pollutants...|بحث عن الملوثات...|Schadstoffe suchen...",
    "Location:|موقع:|Ort:",
    "Any Location|أي موقع|Beliebiger Standort",
    "Determinand :|المحدد:|Bestimmend:",
    "Any Determinand|أي محدد|Beliebiger Bestimmter",
    "Status:|الحالة:|Status:",
    "Any Status|أي حالة|Beliebiger Status",
    "Safe|آمن|Sicher",
    "Caution|تحذير|Vorsicht",
    "Danger|خطر|Gefahr",
    "Apply Filters|تطبيق الفلاتر|Filter anwenden",
    "Pollutant: |ملوث: |Schadstoff:",
    "Details|تفاصيل|Einzelheiten",
    "Details for |تفاصيل عن |Einzelheiten für",
    "Historical Trends: |الاتجاهات التاريخية: |Historische Trends:",
    "Dataset too large|حجم البيانات كبير جدًا|Datensatz zu groß",
    "Please refine your search.|يرجى تحسين بحثك.|Bitte verfeinern Sie Ihre Suche.",
    "No Results|لا توجد نتائج|Keine Ergebnisse",
    "No data matches the selected filters.|لا توجد بيانات تطابق الفلاتر المحددة.|Keine Daten entsprechen den ausgewählten Filtern.",
    "Fluorinated Compounds|المركبات المفلورة|Fluorierte Verbindungen",
    "Select Type:|اختر النوع:|Typ auswählen:",
    "Select Location:|اختر الموقع:|Ort auswählen:",
    "Compliance Status:|حالة الامتثال:|Einhaltungsstatus:",
    "No data|لا توجد بيانات|Keine Daten",
    "No data for the fluro-compound selected.|لا توجد بيانات للمركب المفلور المحدد.|Keine Daten für die ausgewählte fluorierte Verbindung.",
    "Fluorinated Pollutant Levels|مستويات الملوثات المفلورة|Fluorierte Schadstoffniveaus",
    "Fluorinated Pollutants Levels Over Time|مستويات الملوثات المفلورة مع مرور الوقت|Fluorierte Schadstoffniveaus im Laufe der Zeit",
    "Time|الوقت|Zeit",
    "Pollutant: %1\nLocation: %2\nStatus: %3|ملوث: %1\nموقع: %2\nحالة: %3|Schadstoff: %1\nOrt: %2\nStatus: %3",
    "Any|أي|Beliebig",
    "Compliant|متوافق|Konform",
    "Non-Compliant|غير متوافق|Nicht konform",
    "There was no data for your query.|لا توجد بيانات لاستعلامك.|Es gab keine Daten für Ihre Abfrage.",
    "Time: %1\nLevel: %2|الوقت: %1\nالمستوى: %2|Zeit: %1\nLevel: %2",
    "Caution|تحذير|Vorsicht",
    "Danger|خطر|Gefahr",
    "Pollutant Levels |مستويات الملوثات |Schadstoffniveaus ",
    "Welcome|مرحبا|Willkommen",
    "The database will take a minute to load. Press OK to start.|ستستغرق قاعدة البيانات دقيقة للتحميل. اضغط موافق للبدء.|Die Datenbank benötigt eine Minute zum Laden. Drücken Sie OK, um zu starten.",
    "Dashboard|لوحة القيادة|Armaturenbrett",
    "Pollutant Overview|نظرة عامة على الملوثات|Verschmutzungsübersicht",
    "POPs|المركبات العضوية الثابتة|POPs",
    "Fluorinated Compounds|المركبات المفلورة|Fluorierte Verbindungen",
    "Compliance Overview|نظرة عامة على الامتثال|Compliance-Übersicht",
    "Water Quality Monitor|مراقب جودة المياه|Wasserqualitätsmonitor",
    "Filter by Time:|تصفية حسب الوقت:|Nach Zeit filtern:",
    "All Time|كل الأوقات|Alle Zeiten",
    "Last Month|الشهر الماضي|Letzten Monat",
    "Last Year|العام الماضي|Letztes Jahr",
    "Filter by Region:|تصفية حسب المنطقة:|Nach Region filtern:",
    "All Regions|كل المناطق|Alle Regionen",
    "Region A|المنطقة أ|Region A",
    "Region B|المنطقة ب|Region B",
    "Region C|المنطقة ج|Region C",
    "Apply Filters|تطبيق الفلاتر|Filter anwenden",
    "View Details|عرض التفاصيل|Details anzeigen",
    "User Guide|دليل المستخدم|Benutzerhandbuch",
    "Help Resources|موارد المساعدة|Hilfsressourcen",
    "Credits|الاعتمادات|Credits",
    "Arabic|العربية|Arabisch",
    "German|ألمانية|Deutsch",
    "English|الإنجليزية|Englisch",
    "Mandarin|الماندرين|Mandarin",
    "Number of Safe:|عدد الآمن:|Anzahl der sicheren:",
    "Number of Caution:|عدد الحذر:|Anzahl der vorsichtigen:",
    "Number of Danger:|عدد الخطر:|Anzahl der Gefahren:",
    "Number of Safe Fluro-Samples:|عدد عينات الفلورو الآمنة:|Anzahl sicherer Fluro-Proben:",
    "Safety rate:|معدل الأمان:|Sicherheitsrate:",
    "p.c|نسبة مئوية|Prozentsatz",
    "Status:|الحالة:|Status:",
    "Above compliance threshold|فوق حد الامتثال|Über dem Compliance-Schwellenwert",
    "Below compliance threshold|تحت حد الامتثال|Unter dem Compliance-Schwellenwert",
    "Number of POP samples:|عدد عينات POP:|Anzahl der POP-Proben:",
    "Number of fail POP samples:|عدد عينات POP الفاشلة:|Anzahl fehlgeschlagener POP-Proben:",
    "Number of Compliance samples:|عدد عينات الامتثال:|Anzahl der Compliance-Proben:",
    "Number of fail compliance samples:|عدد عينات الامتثال الفاشلة:|Anzahl fehlgeschlagener Compliance-Proben:",
    "DashboardButton|زر لوحة القيادة|DashboardButton",
    "PollutantOverviewButton|زر نظرة عامة على الملوثات|PollutantOverviewButton",
    "PopsButton|زر POPs|PopsButton",
    "FluorinatedCompoundsButton|زر المركبات المفلورة|FluorinatedCompoundsButton",
    "ComplianceOverviewButton|زر نظرة عامة على الامتثال|ComplianceOverviewButton",
    "LanguageSelector|محدد اللغة|LanguageSelector",
    "One or more widgets not found for setting tab order.|لم يتم العثور على واحد أو أكثر من العناصر لتعيين ترتيب التبويب.|Ein oder mehrere Widgets wurden nicht zum Festlegen der Tab-Reihenfolge gefunden.",
    "Left arrow pressed - navigating backward|تم الضغط على السهم الأيسر - التنقل للخلف|Linker Pfeil gedrückt - Rückwärts navigieren",
    "Right arrow pressed - navigating forward|تم الضغط على السهم الأيمن - التنقل للأمام|Rechter Pfeil gedrückt - Vorwärts navigieren",
    "Enter or Return pressed|تم الضغط على إدخال أو العودة|Enter oder Return gedrückt",
    "Escape pressed - closing application|تم الضغط على الهروب - إغلاق التطبيق|Escape gedrückt - Anwendung schließen",
    "Search|بحث|Suche",
    "Determinand|المحدد|Determinand",
    "Level|المستوى|Niveau",
    "Location|الموقع|Standort",
    "Status|الحالة|Status",
    "Determinand Trends Over Time|اتجاهات المحدد مع مرور الوقت|Entwicklung des Determinands über die Zeit",
    "yyyy-MM-dd HH:mm|yyyy-MM-dd HH:mm|yyyy-MM-dd HH:mm",
    "Time|الوقت|Zeit",
    "?|؟|?",
    "Safe|آمن|Sicher",
    "Caution|تحذير|Vorsicht",
    "Status: |الحالة: |Status: ",
    "\n|\n|\n",
    "Pollutant Details|تفاصيل الملوث|Details zum Schadstoff",
    "Pollutant: %1\nLevel: %2 ppm\nStatus: %3\n\nDescription: %4\n\nDetails: %5|الملوث: %1\nالمستوى: %2 جزء في المليون\nالحالة: %3\n\nالوصف: %4\n\nالتفاصيل: %5|Schadstoff: %1\nNiveau: %2 ppm\nStatus: %3\n\nBeschreibung: %4\n\nDetails: %5",
    "The pollutant level is within the safe threshold, posing minimal or no risk to health.|مستوى الملوث ضمن الحد الآمن، مما يشكل خطرًا ضئيلاً أو لا يشكل خطرًا على الصحة.|Der Schadstoffgehalt liegt innerhalb der sicheren Schwelle und stellt ein minimales oder kein Gesundheitsrisiko dar.",
    "The pollutant level is nearing the upper limit of safety. Monitoring and caution are advised.|مستوى الملوث يقترب من الحد الأقصى للسلامة. يُنصح بالمراقبة والحذر.|Der Schadstoffgehalt nähert sich der oberen Sicherheitsgrenze. Überwachung und Vorsicht sind ratsam.",
    "No specific information available for this status. Please review the pollutant details.|لا توجد معلومات محددة متاحة لهذه الحالة. يرجى مراجعة تفاصيل الملوث.|Keine spezifischen Informationen für diesen Status verfügbar. Bitte überprüfen Sie die Schadstoffdetails.",
    "No additional details available for this pollutant.|لا توجد تفاصيل إضافية متاحة لهذا الملوث.|Keine weiteren Details für diesen Schadstoff verfügbar.",
    "No data|لا توجد بيانات|Keine Daten",
    "There was no data for your query.|لم تتوفر بيانات لطلبك.|Für Ihre Anfrage wurden keine Daten gefunden.",
    "Time: %1 months\nLevel: %2 ppm|الوقت: %1 أشهر\nالمستوى: %2 جزء في المليون|Zeit: %1 Monate\nNiveau: %2 ppm",
    "Please select a pollutant to load data|يرجى تحديد ملوث لتحميل البيانات|Bitte wählen Sie einen Schadstoff aus, um Daten zu laden",
    "Load Data|تحميل البيانات|Daten laden",
    "Pollutant|ملوث|Schadstoff",
    "Level|المستوى|Stufe",
    "Safe|آمن|Sicher",
    "No Data|لا توجد بيانات|Keine Daten",
    "No data found for the selected pollutants.|لم يتم العثور على بيانات للملوثات المحددة.|Keine Daten für die ausgewählten Schadstoffe gefunden.",
    "Error|خطأ|Fehler",
    "Failed to load data: %1|فشل في تحميل البيانات: %1|Daten konnten nicht geladen werden: %1",
    "Pollutant Trends Over Time|اتجاهات الملوثات بمرور الوقت|Schadstofftrends im Zeitverlauf",
    "Time|الوقت|Zeit",
    "Pollutant Level (ppm)|مستوى الملوث (جزء في المليون)|Schadstoffniveau (ppm)",
    "Safe|آمن|Sicher",
    "Caution|تحذير|Vorsicht",
    "Danger|خطر|Gefahr",
    "Time: %1\nValue: %2 ppm|الوقت: %1\nالقيمة: %2 جزء في المليون|Zeit: %1\nWert: %2 ppm",
    "Pollutant Details|تفاصيل الملوث|Schadstoffdetails",
    "Pollutant: %1\nLevel: %2\nStatus: %3|ملوث: %1\nالمستوى: %2\nالحالة: %3|Schadstoff: %1\nStufe: %2\nStatus: %3",
    "Filter by Time:|تصفية حسب الوقت:|Nach Zeit filtern:",
    "All Time|كل الأوقات|Alle Zeiten",
    "Last Month|الشهر الماضي|Letzten Monat",
    "Last Year|العام الماضي|Letztes Jahr",
    "Filter by Region:|تصفية حسب المنطقة:|Nach Region filtern:",
    "All Regions|جميع المناطق|Alle Regionen",
    "Region A|المنطقة أ|Region A",
    "Region B|المنطقة ب|Region B",
    "Region C|المنطقة ج|Region C",
    "Apply Filters|تطبيق الفلاتر|Filter anwenden",
    "Details|تفاصيل|Einzelheiten",
    "Pollutant Details|تفاصيل الملوث|Schadstoffdetails",
    "Pollutant: %1\nLevel: %2\nStatus: %3|ملوث: %1\nالمستوى: %2\nالحالة: %3|Schadstoff: %1\nStufe: %2\nStatus: %3",
    "Filters applied: Time -|تم تطبيق الفلاتر: الوقت -|Filter angewendet: Zeit -",
    ", Region -|، المنطقة -|, Region -",
    "Compliance|امتثال|Einhaltung"
};

std::string language = "English";


std::string t(const std::string &text)
{
    // Each line will contain the following [english word or phrase]|[arabic word or phrase]|[german word or phrase]
    for (auto translation_line : translations)
    {
        std::string line = translation_line.toStdString();
        // Get a variable for the english, arabic and german words from the line
        std::string english = line.substr(0, line.find("|"));
        std::string arabic = line.substr(line.find("|") + 1, line.find("|", line.find("|") + 1) - line.find("|") - 1);
        std::string german = line.substr(line.find("|", line.find("|") + 1) + 1);

        // Depending on the language, return the correct translation
        if (language == "Arabic") {
            if (text == english) {
                return arabic;
            }
        } else if (language == "German") {
            if (text == english) {
                return german;
            }
        }
    }
    return text;
}

char* tc(const std::basic_string<char> &text)
{
    return const_cast<char *>(t(text).c_str());
}