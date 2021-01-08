#ifndef PYTHON_INTERPRETER_LIST_H
#define PYTHON_INTERPRETER_LIST_H

#include <iostream>
#include <vector>
#include "Type.hpp"

class List{
    friend class EvalVisitor;

    std::vector< Value* >v;

    void delete_all(){
        for(auto &ele:v){
            delete ele;
            ele = nullptr ;
        }
        v.clear();
    }
public:
    List() = default;
    List(const List &rhs){
        for(auto ele:rhs.v)
            v.push_back( new Value( *ele ) );
    }
    List(List &&rhs){
        for(auto &ele:rhs.v)
            v.push_back( ele ),ele = nullptr;
    }
    
    void add(const Value &x){
        v.push_back( new Value(x) );
    }

    List &operator=( const List &rhs ){
        if( (&rhs) == this ) return (*this);

        for(int i = 0;i < v.size();i ++ ){
            delete v[i];
            v[i] = new Value ( *rhs.v[i] );
        }
        return (*this);
    }

    List &operator+=( const List &rhs ){
        for(int i = 0;i < v.size();i ++ )
            (*v[i]) += (*rhs.v[i]);
        return (*this);
    }
    List &operator-=( const List &rhs ){
        for(int i = 0;i < v.size();i ++ )
            (*v[i]) -= (*rhs.v[i]);
        return (*this);
    }
    List &operator*=( const List &rhs ){
        for(int i = 0;i < v.size();i ++ )
            (*v[i]) *= (*rhs.v[i]);
        return (*this);
    }
    List &operator/=( const List &rhs ){
        for(int i = 0;i < v.size();i ++ )
            (*v[i]) /= (*rhs.v[i]);
        return (*this);
    }
    List &divide_e(const List &rhs ){
        for(int i = 0;i < v.size();i ++ )
            v[i] -> divide_e ( *rhs.v[i] );
        return (*this);
    }
    List &operator%=( const List &rhs ){
        for(int i = 0;i < v.size();i ++ )
            (*v[i]) %= (*rhs.v[i]);
        return (*this);
    }

    ~List(){
        delete_all();
    }
}

#endif