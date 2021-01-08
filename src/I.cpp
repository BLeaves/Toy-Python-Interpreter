#include "I.hpp"

int I::comp(const I &rhs){
    if( (*this) == rhs ) return 0;
    if( (*this) < rhs) return -1;
    return 1;
}

I::I(): ui("0") {
    sign=1;
}

I::I(const std::string &num){
    sign=1;
    if(num[0]=='-') sign=-1;

    if(num[0]=='-' || num[0]=='+') ui = UI( num.substr(1) );
    else ui = UI(num);
}

bool I::operator==(const I &x) {
    return sign==x.sign && ui==x.ui ;
}

bool I::operator!=(const I &x) {
    return sign!=x.sign || ui!=x.ui;
}

bool I::operator<(const I &x) {
    if(sign != x.sign) return sign<x.sign;
    return sign>0 ? (ui<x.ui) : (ui>x.ui) ;
}

bool I::operator>(const I &x) {
    if(sign != x.sign) return sign>x.sign;
    return sign<0 ? (ui<x.ui) : (ui>x.ui) ;
}

bool I::operator<=(const I &x) {
    return (*this)<x || (*this)==x ;
}

bool I::operator>=(const I &x) {
    return  (*this==x) || (*this>x);
}

//<-------------------------------->

I I::operator+(const I &x) {
    I ans;

    if( sign==x.sign ) ans.sign=sign,ans.ui=ui+x.ui;
    else{
        ans.sign=ui>x.ui?sign:-sign;
        ans.ui=ui-x.ui;
    }

    return ans;
}

I I::operator-(const I &x) {
    I ans;

    if( sign!=x.sign ) ans.sign=sign,ans.ui=ui+x.ui;
    else{
        ans.sign=ui>x.ui?sign:-sign;
        ans.ui=ui-x.ui;
    }

    return ans;
}

I I::operator*(const I &x) {
    I ans;

    ans.sign=sign*x.sign;
    ans.ui=ui*x.ui;

    return ans;
}

I I::operator/(const I &x) {
    I ans;

    ans.sign=sign/x.sign;
    ans.ui=ui/x.ui;

    if( ans.sign==-1 && (ui%x.ui).v.size() != 0 ){
        ans.ui= ans.ui + UI("1") ;
    }

    return ans;
}

I I::operator%(const I &x) {
    return (*this) - ( (*this)/x ) * x;
}

std::string I::str(){
    if(ui==UI("0")) return "0";

    std::string ans="";
    if(sign==-1) ans.push_back('-');

    return ans+ui.str();
}

int I::trs()const{
    if(sign == 0) return 0 ;
    return sign * ui.trs() ;
}