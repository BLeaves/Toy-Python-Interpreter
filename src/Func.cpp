#include "Func.hpp"
#include "Evalvisitor.h"

Func* Func::nw;
Func* Func::mn;

extern EvalVisitor visitor;

Func::Func(Python3Parser::FuncdefContext*  _ctx):ctx(_ctx){
    auto typeargs = ctx->parameters()->typedargslist();
    auto nm = typeargs ? typeargs->tfpdef() : std::vector<Python3Parser::TfpdefContext*>();
    auto vl = typeargs ? typeargs->test() : std::vector<Python3Parser::TestContext*>();

    for(int i = int(nm.size()) - int(vl.size()) - 1;i >= 0;i --)
        m_para[ nm[i]->getText() ] = new Value();

    for(int i = int(nm.size() - vl.size()) ;i < nm.size();i ++)
        add_ele(  nm[i]->getText()  , visitor.visit( vl[ i - (nm.size() - vl.size()) ] ).as<Value>() , m_para);
}

void Func::clear_mp(std::map< std::string , Value* > &mp){
    for(auto pr:mp) delete pr.second;
    mp.clear();
}

antlrcpp::Any Func::run( Python3Parser::ArglistContext *ctx_al ){
    //init n_value
    for(auto x:m_para)
        n_value[ x.first ] = new Value( *x.second );

    //
    auto a= ctx_al ? ctx_al -> argument() : std::vector<Python3Parser::ArgumentContext*>();
    auto typeargs = ctx->parameters()->typedargslist();
    auto nm = typeargs ? typeargs->tfpdef() : std::vector<Python3Parser::TfpdefContext*>();
    int i = 0;
    for(;i < a.size() and a[i]->ASSIGN() == nullptr ; i ++ )
        add_ele( nm[i]->getText() , visitor.visit( a[i]->test()[0] ).as<Value>() , n_value );
    for(;i < a.size(); i ++)
        add_ele( a[i]->test()[0]->getText() , visitor.visit( a[i]->test()[1] ).as<Value>() , n_value );

    auto tmp = nw;
    nw = this;
    try{
        visitor.visit( ctx -> suite() );
    }
    catch(const antlrcpp::Any &x){
        clear_mp( n_value );
        nw = tmp;
        return x;
    }
    nw = tmp;

    return std::vector<Value>(1);
}

void Func::add_ele(const std::string &Name, const Value &x , std::map< std::string , Value* > &mp){
    if( mp[ Name ] ) delete mp[Name] ;
    mp[ Name ] = new  Value( x );
}

Value* Func::getptr(const std::string &nm){
    auto p = n_value.find(nm);
    if( p != n_value.end() ) return p->second;
    auto q = mn->n_value.find(nm);
    if( q != mn->n_value.end() ) return q->second;
    return n_value[nm] = new Value();
}


Func::~Func(){
    clear_mp( m_para );
    clear_mp( n_value );
}
