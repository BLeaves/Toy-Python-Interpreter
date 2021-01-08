#ifndef PYTHON_INTERPRETER_I_H
#define PYTHON_INTERPRETER_I_H

#include "UI.hpp"

class Value;

class I{

    friend class Value;

private:
    int sign;
    UI ui;

    int comp(const I &rhs);
    void check();
public:
    I();

    I(const std::string &num);

    bool operator==(const I &x) const;
    bool operator!=(const I &x) const;
    bool operator<(const I &x) ;
    bool operator>(const I &x) ;
    bool operator<=(const I &x) ;
    bool operator>=(const I &x) ;

    //<-------------------------->

    I operator+(const I &x) ;
    I operator-(const I &x) ;
    I operator*(const I &x) ;
    I operator/(const I &x) ;
    I operator%(const I &x) ;

    //<-------------------------->

    std::string str() const;
    int trs()const;
};

#endif
