#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include <iostream>
#include <string>
#include <algorithm> // for std::transform
#include <functional>
#include <vector>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <cctype>
#include <QVariant>
#include <limits>


static inline int indexFind(const QString& str) {
    // Find the starting position of the numeric part from the end of the string
    int startPos = -1;
    for (int i = str.size() - 1; i >= 0; --i) {
        if (!isdigit(str[i].unicode())) {
            startPos = i + 1;
            break;
        }
    }

    // If the entire string is numeric or no numeric part is found
    if (startPos == -1) {
        startPos = 0;
    }

    // Extract the numeric part
    QString numPart = str.mid(startPos);

    // Convert to integer
    bool ok;
    int number = numPart.toInt(&ok);

    // If conversion fails, return a default value (e.g., 0) or handle the error
    if (!ok) {
        return 0; // Or handle the error as required
    }

    return number;
}


static inline bool textToBool(const QString &text) {
    QString lowerText = text.trimmed().toLower(); // Convert to lower case and trim whitespace

    // Check for true values
    if (lowerText == "true" || lowerText == "1" || lowerText == "yes") {
        return true;
    }
    // Check for false values
    else if (lowerText == "false" || lowerText == "0" || lowerText == "no") {
        return false;
    }

    // For any other input, you might want to handle it as an error
    // For example, throw an exception, return a default value, or assert.
    // This part depends on how you want your program to behave in case of unexpected input.

    // Throwing an exception (remove this part if you prefer a different error handling strategy)
    throw std::invalid_argument("Invalid input for boolean conversion");
}


static inline QString boolToText(const bool &_boolean)
{
    if (_boolean)
        return "true";
    else
       return "false";
}



static inline std::string removeSpacesFromCharStar(const char* str) {
    size_t length = strlen(str);
    std::string result;

    for (int i = 0; i < length; i++) {
        if (str[i] != ' ') {
            result += str[i];
        }
    }

    return result;
}



// Function to convert a string to lowercase
static inline std::string toLowerCase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}




static inline void clearConsole()
{
    // ANSI escape sequence to clear screen for Unix-like systems
    std::cout << "\033[2J\033[1;1H";
}


static inline unsigned int strToUnsignedInt(const std::string& str, bool& ok)
{
    // Convert std::string to QString
    QString qStr = QString::fromUtf8(str.c_str());

    // Convert QString to unsigned int using toUInt()
    unsigned int result = qStr.toUInt(&ok);

    // Check if the result is within the valid range for unsigned int
    if (ok && qStr == QString::number(result)) {
        return result;
    } else {
        ok = false;
        return 0; // Handle the error as needed
    }
}


static inline void sortQStringListBySuffix(QStringList &list, const QString &separator)
{
    QRegularExpression regex(separator + "(\\d+).ini");
    std::sort(list.begin(), list.end(), [&regex](const QString &a, const QString &b)
    {
       QRegularExpressionMatch matchA = regex.match(a);
       QRegularExpressionMatch matchB = regex.match(b);
       int numberA = matchA.hasMatch() ? matchA.captured(1).toInt() : INT_MAX;
       int numberB = matchB.hasMatch() ? matchB.captured(1).toInt() : INT_MAX;
       return numberA < numberB;
    });
}


static inline int extractNumberFromSessionString(const QString &inputStr) {
    QRegularExpression regex("session:\\s+(\\d+)");
    QRegularExpressionMatch match = regex.match(inputStr);

    if (match.hasMatch()) {
        return match.captured(1).toInt();
    }

    return -1;  // Return -1 or another appropriate value if no number is found
}






#endif
