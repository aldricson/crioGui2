#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm> // for std::transform
#include <functional>
#include <sstream>
#include <vector>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <cctype>


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
    int length = strlen(str);
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



// Function to center align in text menus
static inline std::string centerAlignString(const std::string& str, unsigned int nbChars) {
    unsigned int totalSpaces = nbChars - str.length();
    unsigned int spacesBefore = totalSpaces / 2;
    unsigned int spacesAfter = totalSpaces - spacesBefore;
    std::string spacesBeforeStr(spacesBefore, ' ');
    std::string spacesAfterStr(spacesAfter, ' ');

    // Construct the result string
    std::string result =  "░" + spacesBeforeStr + str + spacesAfterStr + "░" ;

    return result;
}


static inline void displayMenu(const std::string& title,
                               const std::vector<std::string>& options)
{
        // Find the length of the longest string in options
    unsigned int maxLength = 0;
    for (const auto& option : options) {
        if (option.length() > maxLength) {
            maxLength = option.length();
        }
    }

    // Make sure maxLength is even for center alignment
    if (maxLength % 2 != 0) {
        maxLength++;
    }

    //Give some fresh air to the eyes
    maxLength+=4;

    //Create the top and bottom line
    std::string line;
    for (unsigned int i = 0; i < maxLength+2; ++i) line += "░";
    //spacer
    std::string spacer = centerAlignString(" ", maxLength);
    //1) Top line
    std::cout <<  line <<  std::endl;
    //2) spacer
    std::cout << spacer.c_str() << std::endl;
    //3) title 
    std::cout << centerAlignString(title, maxLength)<< std::endl;
    //4) spacer
    std::cout << spacer.c_str() << std::endl;
    //5) line ... Title zone done
    std::cout << line.c_str() << std::endl;
    // Output each centered option
    for (auto option : options) {
        // Append white spaces to make the option string length equal to maxLength
        option.append(maxLength - option.length(), ' ');
        std::cout << centerAlignString(option, maxLength) << std::endl;
    }
    // Bottom of the "table"
    std::cout << spacer.c_str() << std::endl;
    std::cout << line << std::endl;
}

static inline void handleUserInput(
        const std::vector<std::string>& options,
        const std::vector<std::function<void()>>& actions,
        const std::function<void()> &retryFunction)
    {
        std::string choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.clear();
        std::cin.ignore();

        unsigned int selectedChoice;
        //extract the user choice
        std::stringstream ss(choice);
        if (ss >> selectedChoice && ss.eof() && selectedChoice < actions.size()) {
            //here we have the index of the option selected, as the vector of options and actions are synchronized (with the help of god)
            //we just have to call the action with this index, it will automagically execute the function at this index. 
            actions[selectedChoice]();//<- note the () at the end.... it means we execute the function at the pointer found at the index in the vector
        } 
        else 
        {
            //if we are here or the user is stupid or he's a motha fucka, bacause he choose
            //an invalid option
            std::cout << "Invalid selection. Try again." << std::endl;
            retryFunction();
        }
    }

static inline void displayMenu(const std::string& title,
                               const std::vector<std::string>& options,
                               const std::vector<std::function<void()>>& actions,
                               const std::function<void()> &retryFunction) 
{
    displayMenu(title,options);
    handleUserInput(options,actions,retryFunction);
}

static inline void clearConsole()
{
    // ANSI escape sequence to clear screen for Unix-like systems
    std::cout << "\033[2J\033[1;1H";
}

// Function to draw a cell with a given size and caption
static inline std::string drawCell(int size, std::string aCaption) {
    
    std::string result;
    
    std::string line;
    // the top and bottom border of the cell
    for (int i = 0; i < size+2; ++i) {
        line += "░";
    }
    line += "\n";
    std::string spacer  = centerAlignString(" ", size)+"\n";
    std::string _caption = centerAlignString(aCaption,size)+"\n"; 
    result = line+spacer+_caption+spacer+line;
    return result;
}

static inline std::string concatenateRow(const std::vector<std::string>& row) {
    std::vector<std::vector<std::string>> linesInCells;

    // Split each cell into lines
    for (const auto& cell : row) {
        std::vector<std::string> lines;
        std::istringstream stream(cell);
        std::string line;
        while (std::getline(stream, line)) {
            lines.push_back(line);
        }
        linesInCells.push_back(lines);
    }

    std::string result;

    // Find the maximum number of lines in any cell
    size_t maxLines = 0;
    for (const auto& lines : linesInCells) {
        maxLines = std::max(maxLines, lines.size());
    }

    // Concatenate lines from each cell
    for (size_t i = 0; i < maxLines; ++i) {
        std::string concatenatedLine;
        for (const auto& lines : linesInCells) {
            if (i < lines.size()) {
                concatenatedLine += lines[i];
            } else {
                // Fill with spaces if this cell has fewer lines
                concatenatedLine += std::string(lines[0].size(), ' ');
            }
        }
        result += concatenatedLine + "\n";
    }
    return result;
}


static inline unsigned int strToUnsignedInt(const std::string& str, bool& ok)
{
    try {
        unsigned long ul = std::stoul(str);
        if (ul <= std::numeric_limits<unsigned int>::max()) {
            ok = true;  // Conversion successful
            return static_cast<unsigned int>(ul);
        } else {
            ok = false; // Value is too large for unsigned int
            return 0;   // Return a default value or handle the error as needed
        }
    } catch (const std::exception& e) {
        ok = false; // Exception occurred during conversion
        return 0;   // Return a default value or handle the error as needed
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


static inline void showBanner()
{
    std::cout <<"          `^^^^^^^`              ______ _       _"<< std::endl;
  std::cout <<"      `4$$$c.$$$..e$$P\"         |  ____| |     | |            "<< std::endl;
  std::cout <<"    \"$$c.   `$b$F    .d$P\"      | |__  | |_   _| |_ ___  __ _ "<< std::endl;
  std::cout <<"  ^$$.      $$ d$\"      d$P     |  __| | | | | | __/ _ \\/ _` |"<< std::endl;
  std::cout <<" `\"$c.      4$.  $$       .$$   | |____| | |_| | ||  __/ (_| |"<< std::endl;
  std::cout <<"'$P$$$$$$$$$$$$$$$$$$$$$$$$$$   |______|_|\\__, |\\__\\___|\\__, |"<< std::endl;
  std::cout <<"$$e$P\"    $b     d$`    \"$$c$F             __/ |           | |"<< std::endl;
  std::cout <<"$b  .$$\" $$      .$$ \"4$b.  $$            |___/            |_|"<< std::endl;
  std::cout <<"$$     e$$F       4$$b.     $$ "<< std::endl;
  std::cout <<"$P     `$L$$P` `\"$$d$\"      $$ "<< std::endl;
  std::cout <<"d$     $$   z$$$e   $$     '$. "<< std::endl;
  std::cout <<" $F   *$. \"$$e.e$$\" 4$F   ^$b  "<< std::endl;
  std::cout <<"  .$$ 4$L*$$.     .$$Pd$  '$b  "<< std::endl;
  std::cout <<"   $$$$$.           .$$$*$.    "<< std::endl;
  std::cout <<"    .d$P"            "$$c      "<< std::endl;
  std::cout <<"       .d$$$******$$$$c.       "<< std::endl;
  std::cout <<"            ______             "<< std::endl << std::endl;

  std::string  txt=drawCell(45," WELCOME TO ELYTEQ  COMPATIBILITY LAYER ");
  std::cout << txt.c_str();
               txt=drawCell(45,"proudly coded by A.Gilbert and S.Klalesh");
  std::cout << txt.c_str() << std::endl;
}



#endif
