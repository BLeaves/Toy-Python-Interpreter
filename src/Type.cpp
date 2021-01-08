#include "Type.hpp"


int Value::comp(const Value &rhs)const{
    if( tpnm == None || rhs.tpnm == None ){
        if( tpnm == rhs.tpnm ) return 0;
        else return 1;
    }
    if( tpnm == Str ){
        if( (*ps) == (* (rhs.ps) ) ) return 0;
        if( (*ps) < (* (rhs.ps) ) ) return -1;
        return 1;
    }
    if(tpnm == rhs.tpnm && tpnm == Int ) return pi->comp( *(rhs.pi) );
    if(tpnm == Int || rhs.tpnm == Int ){
        int tmp= trans_i().pi->comp( * (rhs.trans_i().pi) ) ;
        if(tmp!=0) return tmp;
    }

    double x1=small_value(),x2=rhs.small_value();
    if( fabs(x1-x2) < exp ) return 0;
    return x1 < x2 ? (-1) : 1 ;
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

Value Value:: operator!()const{
    return Value( ! trans_b().b) ;
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
    if(tpnm == Int) return Value( *pi != I() );
    if(tpnm == Float) return Value( fabs(f) < exp );
    if(tpnm == None) return Value( false );
    return Value( (*ps) != "" );
}

Value Value::trans_i()const{
    if(tpnm == Bool) return Value( I( b?"1":"0" ) );
    if(tpnm == Int) return (*this);
    if(tpnm == Float) return Value( I(std::to_string( (long long)(f) )) );
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
    if(tpnm == Int) return Value( pi->str() );
    if(tpnm == Float) return Value( std::to_string( f ) );
    if(tpnm == None) return Value( std::string("None") );
    return (*this);
}

void Value::print()const{
    std::cout << *(trans_s().ps);
}
