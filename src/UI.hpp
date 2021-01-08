#ifndef PYTHON_INTERPRETER_UI_H
#define PYTHON_INTERPRETER_UI_H


#include <string>
#include <algorithm>
#include <vector>

class I;

class UI {
    friend class I;
private:
    std::vector<int> v;

public:
    UI();

    UI(const std::string &num);

    bool operator==(const UI &x) ;
    bool operator!=(const UI &x) ;
    bool operator<(const UI &x) ;
    bool operator>(const UI &x) ;
    bool operator<=(const UI &x) ;
    bool operator>=(const UI &x) ;

    //<-------------------------->

    UI operator<<(const int &x) ;
    UI operator+(const UI &x) ;
    UI operator-(const UI &x) ;
    UI operator*(const int &x) ;
    UI operator*(const UI &x) ;
    UI operator/( UI x) ;
    UI operator%(const UI &x) ;

    //<-------------------------->

    void clear_0();

    std::string str()const;
    int trs()const;
};//[0,size) [low,high)

#endif