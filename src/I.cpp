#include "I.hpp"

void I::check() {
    if (ui.iszero()) sign = 1;
}

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
    if(num.empty()) return;
    if(num[0]=='-') sign=-1;
    if(num[0]=='-' || num[0]=='+') ui = UI( num.substr(1) );
    else ui = UI(num);
    check();
}

bool I::operator==(const I &x) const {
    return str() == x.str();
    // check();
    // return sign==x.sign && ui==x.ui ;
}

bool I::operator!=(const I &x) const {
    return str() != x.str();
    // check();
    // return sign!=x.sign || ui!=x.ui;
}

bool I::operator<(const I &x) {
    check();
    if(sign != x.sign) return sign<x.sign;
    return sign>0 ? (ui<x.ui) : (ui>x.ui) ;
}

bool I::operator>(const I &x) {
    check();
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
    check();
    return ans;
}

I I::operator-(const I &x) {
    I ans;

    if( sign!=x.sign ) ans.sign=sign,ans.ui=ui+x.ui;
    else{
        ans.sign=ui>x.ui?sign:-sign;
        ans.ui=ui-x.ui;
    }
    check();
    return ans;
}

I I::operator*(const I &x) {
    I ans;

    ans.sign=sign*x.sign;
    ans.ui=ui*x.ui;

    check();
    return ans;
}

I I::operator/(const I &x) {
    I ans;

    ans.sign=sign/x.sign;
    ans.ui=ui/x.ui;

    if( ans.sign==-1 && (ui%x.ui).v.size() != 0 ){
        ans.ui= ans.ui + UI("1") ;
    }

    check();
    return ans;
}

I I::operator%(const I &x) {
    auto ans = (*this) - ( (*this)/x ) * x;
    ans.check();
    return ans;
}

std::string I::str() const {
    if(ui==UI("0")) return "0";

    std::string ans="";
    if(sign==-1) ans.push_back('-');

    return ans+ui.str();
}

int I::trs()const{
    if(sign == 0) return 0 ;
    return sign * ui.trs() ;
}
