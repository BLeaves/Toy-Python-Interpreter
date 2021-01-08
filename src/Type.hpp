#ifndef PYTHON_INTERPRETER_TYPE_H
#define PYTHON_INTERPRETER_TYPE_H


#include <iostream>
#include <cmath>
#include "I.hpp"

class Value{
public:
enum Typename{None,Bool,Int,Float,Str};

static constexpr double exp=1e-16;

public:
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
    double small_value()const;

public:
    Value():tpnm(None){    clear_ptr();}
    Value(bool bl):tpnm(Bool),b(bl), f(){    clear_ptr();}
    Value(const I &it):tpnm(Int), b(), f(){   clear_ptr();    pi=new I(it);}
    Value(double fl):tpnm(Float),b(), f(fl){    clear_ptr();}
    Value(const std::string &str):tpnm(Str), b(), f(){   clear_ptr();    ps=new std::string(str);}
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

    bool operator!()const;

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

    ~Value(){   delete_ptr();}
};

//and or not

int Value::comp(const Value &rhs)const{
    if( tpnm == Str ){
        if( (*ps) == (* (rhs.ps) ) ) return 0;
        if( (*ps) < (* (rhs.ps) ) ) return -1;
        return 1;
    }
    if(tpnm == Float || rhs.tpnm == Float ){
        double x1=small_value(),x2=rhs.small_value();
        if( fabs(x1-x2) < exp ) return 0;
        return x1 < x2 ? (-1) : 1 ;
    }

    int tmp= trans_i().pi->comp( * (rhs.trans_i().pi) ) ;
    return tmp;
}

double Value::small_value()const{
    if(tpnm == Bool) return int(b);
    if(tpnm == Int) return pi->trs();
    return f;
}

//<-------------------------------->

Value& Value::operator=(const Value &vl){
    tpnm=vl.tpnm,b=vl.b,f=vl.f;
    delete_ptr();
    clear_ptr();
    if(tpnm == Int) pi=new I(* (vl.pi) );
    if(tpnm == Str) ps=new std::string(* (vl.ps) );

    return (*this);
}

//<-------------------------------->

Value Value::operator+(const Value &rhs)const{
    if( tpnm == Str ) return Value( (*ps) + (* (rhs.ps) ) );
    if( tpnm == Float || rhs.tpnm == Float ) return Value( small_value() + rhs.small_value() );

    Value ans=trans_i();
    (* ans.pi) = (* ans.pi) + (* rhs.trans_i().pi) ;
    return ans;
}

Value Value::operator-(const Value &rhs)const{
    if( tpnm == Float || rhs.tpnm == Float ) return Value( small_value() - rhs.small_value() );

    Value ans=trans_i();
    (* ans.pi) = (* ans.pi) - (* rhs.trans_i().pi) ;
    return ans;
}

Value Value::operator*(const Value &rhs)const{
    if( tpnm == Str ) return Value( *this ).mtpl_s( rhs.small_value() );
    if( rhs.tpnm == Str ) return Value( rhs ).mtpl_s( small_value() );

    if( tpnm == Float || rhs.tpnm == Float )
        return Value( small_value() * rhs.small_value() );

    Value ans=trans_i();
    (* ans.pi) = (* ans.pi) * (* rhs.trans_i().pi) ;
    return ans;
}

Value Value::operator/(const Value &rhs)const{
    return Value( small_value()/rhs.small_value() );
}

Value Value::divide(const Value &rhs)const{
    Value ans= this->trans_i();
    (*ans.pi) = (*ans.pi)/(*(rhs.trans_i().pi));
    return ans;
}

Value Value::operator%(const Value &rhs)const{
    return (*this) - ( this -> divide( rhs ) ) *  rhs ;
}

//<-------------------------------->

Value&  Value::operator+=(const Value &rhs){
    return (*this) = (*this) + rhs;
}

Value&  Value::operator-=(const Value &rhs){
    return (*this) = (*this) - rhs;
}

Value&  Value::operator*=(const Value &rhs){
    return (*this) = (*this) * rhs;
}

Value&  Value::operator/=(const Value &rhs){
    return (*this) = (*this) / rhs;
}

Value&  Value::divide_e(const Value &rhs){
    return (*this) = ( this->divide( rhs ) );
}

Value&  Value::operator%=(const Value &rhs){
    return (*this) = (*this) % rhs;
}

//<-------------------------------->

bool Value:: operator!()const{
    return ( ! trans_b().b) ;
}

bool Value:: operator<(const Value &rhs)const{
    return comp(rhs) == -1;
}

bool Value:: operator>(const Value &rhs)const{
    return comp(rhs) == 1;
}

bool Value:: operator<=(const Value &rhs)const{
    return comp(rhs) <= 0;
}

bool Value:: operator>=(const Value &rhs)const{
    return comp(rhs) >= 0 ;
}

bool Value:: operator==(const Value &rhs)const{
    return comp( rhs ) == 0 ;
}

bool Value:: operator!=(const Value &rhs)const{
    return comp(rhs) != 0 ;
}

Value Value::trans_b()const{
    if(tpnm == Bool) return (*this);
    if(tpnm == Int) return Value( pi->sign != 0 );
    if(tpnm == Float) return Value( fabs(f) > exp );
    return Value( (*ps) != "" );
}

Value Value::trans_i()const{
    if(tpnm == Bool) return Value( I( b?"1":"0" ) );
    if(tpnm == Int) return (*this);
    if(tpnm == Float) return Value( std::to_string( int(f) ) );
    return Value( I( *ps ) );
}

Value Value::trans_f()const{
    if(tpnm == Bool) return Value( double(b) );
    if(tpnm == Int) return Value( double( pi->trs() ) );
    if(tpnm == Float) return (*this);
    return Value( std::stod( *ps ) );
}

Value Value::trans_s()const{
    if(tpnm == Bool) return Value( b ? std::string("True") : std::string("False") );
    if(tpnm == Int) return Value( std::to_string( pi->trs() ) );
    if(tpnm == Float) return Value( std::to_string( f ) );
    return (*this);
}

#endif