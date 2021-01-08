#ifndef PYTHON_INTERPRETER_FUNC_H
#define PYTHON_INTERPRETER_FUNC_H


#include <iostream>
#include <map>
#include <vector>
#include "List.hpp"
#include "Python3BaseVisitor.h"

class Func{
    friend class EvalVisitor;

    static Func* nw;
    static Func* mn;
    
    std::map< std::string , Value* > m_para,n_value;
    Python3Parser::FuncdefContext  *ctx;

    void clear_mp(std::map< std::string , Value* > &mp);
public:
    Func() = default;
    Func(Python3Parser::FuncdefContext *_ctx);

    antlrcpp::Any run( Python3Parser::ArglistContext *ctx_al );

    void add_ele(const std::string &Name, const Value &x , std::map< std::string , Value* > &mp);

    ~Func();
};


void Func::clear_mp(std::map< std::string , Value* > &mp){
    for(auto pr:mp) delete pr.second;
    mp.clear();
}


#endif