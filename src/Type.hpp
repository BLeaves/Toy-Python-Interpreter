#ifndef PYTHON_INTERPRETER_TYPE_H
#define PYTHON_INTERPRETER_TYPE_H

#include <iostream>
#include <cmath>
#include "I.hpp"

class EvalVisitor;

class Value{

    friend class EvalVisitor;

enum Typename{None,Bool,Int,Float,Str};

static constexpr double exp=1e-6;

private:
    Typename tpnm;
    bool b;
    I *pi;
    double f;
    std::string *ps;


    void clear_ptr(){   pi=nullptr; ps=nullptr;}
    void delete_ptr(){  delete pi;  delete ps;}
    std::string mtpl_s(int times)const{
        std::string ans="";

        while(times--){ ans += (*ps);}

        return ans;
    }

    int comp(const Value &rhs)const;


public:
    double small_value()const;

    Value():tpnm(None){    clear_ptr();}
    Value(bool bl):tpnm(Bool),b(bl){    clear_ptr();}
    Value(const I &it):tpnm(Int){   clear_ptr();    pi=new I(it);}
    Value(double fl):tpnm(Float),f(fl){    clear_ptr();}
    Value(const std::string &str):tpnm(Str){   clear_ptr();    ps=new std::string(str);}
    Value(const Value &vl):tpnm(vl.tpnm),b(vl.b),f(vl.f){
        clear_ptr();
        if(tpnm == Int) pi=new I(* (vl.pi) );
        if(tpnm == Str) ps=new std::string(* (vl.ps) );
    }
    Value(Value &&vl):tpnm(vl.tpnm),b(vl.b),pi(vl.pi),f(vl.f),ps(vl.ps){    vl.clear_ptr();}

    Value& operator=(const Value &rhs);

    Value operator+(const Value &rhs)const;
    Value operator-(const Value &rhs)const;
    Value operator*(const Value &rhs)const;
    Value operator/(const Value &rhs)const;
    Value divide(const Value &rhs)const;
    Value operator%(const Value &rhs)const;

    Value &operator+=(const Value &rhs);
    Value &operator-=(const Value &rhs);
    Value &operator*=(const Value &rhs);
    Value &operator/=(const Value &rhs);
    Value &divide_e(const Value &rhs);
    Value &operator%=(const Value &rhs);

    Value operator!()const;

    bool operator<(const Value &rhs)const;
    bool operator>(const Value &rhs)const;
    bool operator<=(const Value &rhs)const;
    bool operator>=(const Value &rhs)const;
    bool operator==(const Value &rhs)const;
    bool operator!=(const Value &rhs)const;

    Value trans_b()const;
    Value trans_i()const;
    Value trans_f()const;
    Value trans_s()const;

    void print()const;

    ~Value(){   delete_ptr();}
};

//and or not

#endif
