#include "Func.hpp"
#include "Evalvisitor.h"

Func* Func::nw;
Func* Func::mn;

extern EvalVisitor visitor;

Func::Func(Python3Parser::FuncdefContext*  _ctx):ctx(_ctx){
    auto nm = ctx->parameters()->typedargslist()->tfpdef();
    auto vl = ctx->parameters()->typedargslist()->test();

    for(int i = nm.size() - vl.size() - 1;i >= 0;i --)
        add_ele(  nm[i]->getText()  , Value() , m_para);

    for(int i = nm.size() - vl.size() ;i < nm.size();i ++)
        add_ele(  nm[i]->getText()  , visitor.visit( vl[ i - vl.size() ] ).as<Value>() , m_para);
    
}

antlrcpp::Any Func::run( Python3Parser::ArglistContext *ctx_al ){
    nw = this;

    //init n_value
    auto a=ctx_al -> argument();
    //
    for(auto x:m_para)
        n_value[ x.first ] = new Value( *x.second );
    
    if(a[0] -> ASSIGN() ){
        for(auto x:a){
            add_ele( x->test()[0]->getText() , visitor.visit( x->test()[1] ).as<Value>() , n_value );
        }
    }
    else{
        auto nm = ctx->parameters()->typedargslist()->tfpdef();
        for(int i = 0;i < a.size() ; i ++ ){
           add_ele( nm[i]->getText() , visitor.visit( a[i]->test()[0] ).as<Value>() , n_value );
        } 
    }


    try{
        visitor.visit( ctx -> suite() );
    }
    catch(antlrcpp::Any &x){
        clear_mp( n_value );
        nw = mn;
        return x;
    }
    return Value();
}

void Func::add_ele(const std::string &Name, const Value &x , std::map< std::string , Value* > &mp){
    if( mp[ Name ] ) delete mp[Name] ;
    mp[ Name ] = new  Value( x );
}


Func::~Func(){
    clear_mp( m_para );
    clear_mp( n_value );
}
