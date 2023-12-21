#ifndef GLOBALSTRINGHELPER_H
#define GLOBALSTRINGHELPER_H

#include <QString>

inline QString shortStringToQString(char* aShortString)
{
    QString result = QString::fromLatin1(aShortString);
    return result;
}

inline char* qStringToShortString(QString aString)
{
    char* cstr;
    std::string str = aString.toStdString();
    cstr = new char [str.size()+1];
    strcpy_s(cstr, str.size() + 1, str.c_str());
    return cstr;

  /*  char* cstr;
    std::string str = aString.toStdString();
    cstr = new char [str.size()+1];
    std::strcpy( cstr, str.c_str() ); //This will throw a warning as it is pure C style unprotected pointer copy without memorybounding
    return cstr;*/
}


#endif // GLOBALSTRINGHELPER_H
