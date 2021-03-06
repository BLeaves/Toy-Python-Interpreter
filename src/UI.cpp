#include "UI.hpp"

UI::UI(){ v.clear();}

bool UI::iszero() {
    clear_0();
    return v.empty();
}

UI::UI(const std::string &num){
    int len=num.length();

    for(int i=0;i<len and isdigit(num[i]);i++)
        v.push_back(num[i]-'0');

    std::reverse( v.begin() , v.end() );
    clear_0();
}

bool UI::operator==(const UI &x) const{
    return str() == x.str();
    // clear_0();
    // if( v.size() != x.v.size() ) return 0;

    // for(int i=0;i<v.size();i++)
    //     if(v[i]!=x.v[i]) return 0;

    // return 1;
}

bool UI::operator!=(const UI &x) const{
    return !((*this)==x);
}

bool UI::operator<(const UI &x) {
    clear_0();
    if( v.size() != x.v.size() ) return v.size() < x.v.size() ;

    for(int i=x.v.size()-1;i>=0;i--)
        if(v[i] != x.v[i]) return v[i] < x.v[i];

    return 0;
}

bool UI::operator>(const UI &x) {
    clear_0();
    if( v.size() != x.v.size() ) return v.size() > x.v.size() ;

    for(int i=x.v.size()-1;i>=0;i--)
        if(v[i] != x.v[i]) return v[i] > x.v[i];

    return 0;
}

bool UI::operator<=(const UI &x) {
    clear_0();
    return ( (*this==x) || (*this<x));
}

bool UI::operator>=(const UI &x) {
    clear_0();
    return ( (*this==x) || (*this>x));
}

//<-------------------------------->

UI UI::operator<<(const int &x) const {
    UI ans=(*this);
    ans.clear_0();

    int sz=ans.v.size();
    ans.v.resize( sz+x );

    for(int i=sz-1;i>=0;i--) ans.v[i+x]=ans.v[i];
    for(int i=0;i<x;i++) ans.v[i]=0;

    return ans;
}

UI UI::operator+(const UI &x) {
    clear_0();
    UI ans;
     UI (*a),(*b);// construction?
    a=this,b=const_cast<UI*>(&x);
    if( (*a) < (*b) ) std::swap(a,b);

    //ans.clear done
    int ad=0;
    for(int i=0;i<b->v.size();i++){
        ad+=( (a->v[i])+(b->v[i]) );
        ans.v.push_back(ad%10);
        ad/=10;
    }

    for(int i=b->v.size();i<a->v.size();i++){
        ad+= (a->v[i]) ;
        ans.v.push_back(ad%10);
        ad/=10;
    }

    if(ad>0) ans.v.push_back(ad);
    ans.clear_0();

    return ans;
}

UI UI::operator-(const UI &x) {
    clear_0();
    UI ans;
     UI (*a),(*b);// construction?
    a=this,b=const_cast<UI*>(&x);
    if( (*a) < (*b) ) std::swap(a,b);

    //ans.clear done
    ans=(*a);
    for(int i=0;i< (b->v.size()) ;i++){
        if(ans.v[i] < b->v[i]){
            ans.v[i]+=10;
            ans.v[i+1]-=1;//如果不存在？
        }

        ans.v[i] -= (b->v[i]) ;
    }

    for(int i=b->v.size();i< (a->v.size()) ;i++){
        if(ans.v[i] < 0){
            ans.v[i]+=10;
            ans.v[i+1]-=1;//如果不存在？
        }
        else break;
    }

    ans.clear_0();

    return ans;
}

UI UI::operator*(const int &x) {
    clear_0();
    UI ans=(*this);

    for(int i=0;i<ans.v.size();i++) ans.v[i]*=x;
    for(int i=0;i<ans.v.size();i++){
        ans.v[i+1]+=(ans.v[i]/10);
        ans.v[i]%=10;
    }//a[ans.v.size()]+=?
    if(ans.v.back()==0) ans.v.pop_back();
    return ans;
}

UI UI::operator*(const UI &x) {
    clear_0();
    UI ans("0");

    ans.v.resize( std::max(int(x.v.size() + v.size()) - 1, 0) );
    for(int i=int(v.size())-1;i>=0;i--)
        for(int j=int(x.v.size())-1;j>=0;j--)
            ans.v[i+j]+=v[i]*x.v[j];

    int ad=0;
    for(int i=0;i<ans.v.size();i++){
        ans.v[i]+=ad;
        ad=ans.v[i]/10;
        ans.v[i]%=10;
    }

    if(ad) ans.v.push_back(ad);

    ans.clear_0();

    return ans;
}

UI UI::operator/(const UI &x) {
    clear_0();
    UI ans,A;

    for(int i=(int)v.size() - 1 ; i>=0 ; i-- ){
        A.v.insert(A.v.begin(), v[i]);
        A.clear_0();
        int ad=0;
        while( A >= x ){
            A = A - x;
            ad++;
        }
        ans.v.push_back(ad);
    }

    std::reverse( ans.v.begin() , ans.v.end() );
    ans.clear_0();

    return ans;
}

UI UI::operator%(const UI &x) {
    clear_0();
    UI ans;

    for(int i=(int)v.size() - 1 ; i>=0 ; i-- ){
        ans.v.insert(ans.v.begin(), v[i]);
        ans.clear_0();
        while( ans >= x )
            ans = ans - x;
    }
    ans.clear_0();
    return ans;
}

void UI::clear_0(){
    while(!v.empty() && v.back()==0 ) v.pop_back();
}

std::string UI::str()const{
    std::string ans="";

    auto i=v.rbegin();
    for(; i != v.rend() and *i == 0; ++i);
    for(; i != v.rend(); i++)
        ans.push_back('0'+(*i));

    return ans;
}

int UI::trs()const{
    int ans=0;

    for(auto i=v.rbegin() ; i!=v.rend() ; i++)
        ans = ans * 10 + (*i) ;

    return ans;
}
