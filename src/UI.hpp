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

UI::UI(){ v.clear();}

UI::UI(const std::string &num){
    int len=num.length();

    for(int i=0;i<len;i++)
        v.push_back(num[i]-'0');

    std::reverse( v.begin() , v.end() );
}

bool UI::operator==(const UI &x) {
    clear_0();
    if( v.size() != x.v.size() ) return 0;

    for(int i=0;i<v.size();i++)
        if(v[i]!=x.v[i]) return 0;

    return 1;
}

bool UI::operator!=(const UI &x) {
    clear_0();
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

UI UI::operator<<(const int &x) {
    clear_0();
    UI ans=(*this);

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

    ans.v.resize( x.v.size() + v.size() - 1 );
    for(int i=v.size()-1;i>=0;i--)
        for(int j=x.v.size()-1;j>=0;j--)
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

UI UI::operator/(UI x) {
    clear_0();
    UI ans,A;
    A=(*this);

    int ad=0;
    for(int i=(int)v.size() ; i>=0 ; i-- ){
        ad=0;
        while( (x<<i) <= A ){
            A=A-(x<<i);
            ad++;
        }
        ans.v.push_back(ad);
    }

    std::reverse( ans.v.begin() , ans.v.end() );
    if(ans.v.empty()) ans.v.push_back(0);
    ans.clear_0();

    return ans;
}

UI UI::operator%(const UI &x) {
    clear_0();
    return (*this) - ( (*this)/x ) * x;
}

void UI::clear_0(){
    while(!v.empty() && v.back()==0 ) v.pop_back();
}

std::string UI::str()const{
    std::string ans="";

    for(auto i=v.rbegin() ; i!=v.rend() ; i++)
        ans.push_back('0'+(*i));

    return ans;
}

int UI::trs()const{
    int ans=0;

    for(auto i=v.rbegin() ; i!=v.rend() ; i++)
        ans = ans * 10 + (*i) ;

    return ans;
}