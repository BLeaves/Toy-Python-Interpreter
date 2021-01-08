#include "Evalvisitor.h"
#include <map>
#include <iostream>
#include "control.hpp"
#include "Func.hpp"

std::map<std::string, Func*> EvalVisitor::m_func;

antlrcpp::Any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
	Func::mn = new Func();
	Func::nw = Func::mn;

	visitChildren(ctx);
	
	delete Func::mn;
	//clear m_func
	return Value();
}

antlrcpp::Any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
	m_func[ ctx->NAME()->getText() ] = new Func( ctx );
	return Value();
}

antlrcpp::Any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
	return visit(ctx -> small_stmt());
}

antlrcpp::Any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
	auto T = ctx -> testlist() ;

	if( ctx -> augassign() ){
		if( ctx -> augassign() -> getText() == "+=" ) 
			return visit( T[0] ).as<List>() += visit( T[1] ).as<List>();
		if( ctx -> augassign() -> getText() == "-=" ) 
			return visit( T[0] ).as<List>() -= visit( T[1] ).as<List>();
		if( ctx -> augassign() -> getText() == "*=" ) 
			return visit( T[0] ).as<List>() *= visit( T[1] ).as<List>();
		if( ctx -> augassign() -> getText() == "/=" ) 
			return visit( T[0] ).as<List>() /= visit( T[1] ).as<List>();
		if( ctx -> augassign() -> getText() == "//=" ) 
			return visit( T[0] ).as<List>() .divide_e( visit( T[1] ).as<List>() );
		if( ctx -> augassign() -> getText() == "%=" ) 
			return visit( T[0] ).as<List>() %= visit( T[1] ).as<List>();
	}
	else{
		if( ctx -> ASSIGN() . size() ){
			for(auto &x:T){
				visit( x ).as<List>() = visit( T.back() ).as<List>() ;
			}
		}
		else return visitChildren( ctx );
	}

	return Value();
}

antlrcpp::Any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
	throw Break();
	return Value();
}

antlrcpp::Any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
	throw Continue();
	return Value();
}

antlrcpp::Any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
	if( ctx -> testlist() ) throw visit( ctx -> testlist() );
	throw antlrcpp::Any();
	//throw empty()?
	return Value();
}

antlrcpp::Any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
	auto T = ctx->test();
	auto S = ctx->suite();


	for(int i = 0;i < T.size() ;i ++ )
		if( visit( T[i] ).as<Value>().small_value() > 0.5 ){
			visit( S[i] ); return Value();
		}
	
	visit( S.back() );
	return Value();
}

antlrcpp::Any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
	while( visit( ctx -> test() ).as<Value>().trans_b() == Value(true) ){
		try{
			visit( ctx ->suite() );
		}
		catch(Break){	break;}
		catch(Continue){	continue;}

	}
	return Value();
}

antlrcpp::Any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
	if( ctx->simple_stmt() ) visitChildren(ctx);
	for(auto ele:ctx->stmt()) visit(ele);
	return Value();
}

antlrcpp::Any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
	return visitChildren(ctx);
	return Value();
}

antlrcpp::Any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
	if( ctx -> OR() .size() ){
		for( auto x: ctx->and_test() )
			if( visit(x).as<Value>().small_value() > 0.3 ) return Value( true );
		return Value( false );
	}
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
	if( ctx -> AND() . size() ){
		for( auto x: ctx->not_test() )
			if( visit(x).as<Value>().small_value() < 0.8 ) return Value( false );
		return Value( true );
	}
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
	if( ctx->NOT() ) return !( visit( ctx->not_test() ).as<Value>() );
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
	auto F = ctx->arith_expr();
	auto O = ctx->comp_op();

	Value ans(true);

	if( O . size() ){
		std::vector< Value > vct;

		for(auto &ele:F) vct.push_back( visit( ele ).as<Value>() );

		for(int i = 0;i < O.size() ;i ++ ){
			if( (O[i] -> getText())  == "<" ) 
				if(!( vct[i] <  vct[i+1] ) ) return Value( false );
			if( (O[i] -> getText())  == ">" ) 
				if(!( vct[i] >  vct[i+1] ) ) return Value( false ); 
			if( (O[i] -> getText())  == "<=" ) 
				if(!( vct[i] <=  vct[i+1] ) ) return Value( false ); 
			if( (O[i] -> getText())  == ">=" ) 
				if(!( vct[i] >=  vct[i+1] ) ) return Value( false ); 
			if( (O[i] -> getText())  == "==" ) 
				if(!( vct[i] ==  vct[i+1] ) ) return Value( false ); 
			if( (O[i] -> getText())  == "!=" ) 
				if(!( vct[i] !=  vct[i+1] ) ) return Value( false ); 
		}
	}
	else  return  visit( F[0] );

	return ans;
}

antlrcpp::Any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
	auto F = ctx->term();
	auto O = ctx->addorsub_op();

	Value ans;

	ans = visit( F[0] ) . as<Value>() ;

	for(int i = 0;i < O.size() ;i ++ ){
		if( (O[i] -> getText())  == "+" ) ans += visit( F[i+1] ).as<Value>();
		if( (O[i] -> getText())  == "-" )  ans -= visit( F[i+1] ).as<Value>();
	}

	return ans;
}

antlrcpp::Any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
	auto F = ctx->factor();
	auto O = ctx->muldivmod_op();

	Value ans;

	
	ans = visit( F[0] ) . as<Value>() ;

	for(int i = 0;i < O.size() ;i ++ ){
		if( ( O[i] -> getText())  == "*" ) ans *= visit( F[i+1] ).as<Value>();
		if( ( O[i] -> getText())  == "/" )  ans /= visit( F[i+1] ).as<Value>();
		if( ( O[i] -> getText())  == "//" ) ans .divide_e( visit( F[i+1] ).as<Value>() );
		if( ( O[i] -> getText())  == "%" ) ans %= visit( F[i+1] ).as<Value>();
	}
	

	return ans;
}

antlrcpp::Any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
	if( ctx -> atom_expr() ) return visitChildren(ctx);
	if( ctx -> MINUS() ) return visit( ctx -> factor() ).as<Value>() * Value( I("-1") );
	return visit( ctx -> factor() );
}

antlrcpp::Any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
	if( ctx -> trailer() ){
		if( ctx -> atom() -> getText() == "print" ){
			auto vct = ctx -> trailer() -> arglist() -> argument();
			
			if(vct.size()) return Value();
			visit(vct[0]).as<Value>().print();

			for(int i = 1;i < vct.size();i ++){
				std::cout<<' ';
				visit( vct[i] ).as<Value>().print();
			}

			std::cout<<std::endl;
		}
		else if( ctx -> atom() -> getText() == "float" ){
			return visit( ctx -> trailer() -> arglist() -> argument()[0] -> test()[0] ).as<Value>().trans_f();
		}
		else if( ctx -> atom() -> getText() == "int" ){
			return visit( ctx -> trailer() -> arglist() -> argument()[0] -> test()[0] ).as<Value>().trans_i();
		}
		else if( ctx -> atom() -> getText() == "str" ){
			return visit( ctx -> trailer() -> arglist() -> argument()[0] -> test()[0] ).as<Value>().trans_s();
		}
		else if( ctx -> atom() -> getText() == "bool" ){
			return visit( ctx -> trailer() -> arglist() -> argument()[0] -> test()[0] ).as<Value>().trans_b();
		}
		else return m_func[ visit( ctx -> atom() ).as<std::string>() ] -> run( ctx -> trailer() ->arglist() );
	}
	else{
		if( visit( ctx -> atom() ).is<std::string>() ){
			std::string s=visit( ctx -> atom() ).as<std::string>();
			if( Func::nw -> getptr( s ) == nullptr) 
				Func::nw -> add_ele( s , Value() , Func::nw->n_value);
			
			return *Func::nw -> n_value[ s ];
		}
	}
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
	if( ctx -> NAME() ) return ctx -> getText();
	if( ctx -> NUMBER() ) return Value( I( ctx -> getText() ) );
	if( ctx -> STRING().size() ) {
		std::string s = "";
		auto vct = ctx -> STRING() ;
		for(auto ele:vct) s += ele->getText().substr(1, ele->getText().size() - 2);

		return Value( s );
	}
	if( ctx -> NONE() ) return Value();
	if( ctx -> TRUE() ) return Value(true);
	if( ctx -> FALSE() ) return Value(false);
	return visitChildren(ctx);
	
}

antlrcpp::Any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
	List* lst = new List();
	auto v = ctx -> test();

	for(auto x:v)
		if( visit( x ).is<std::string>() ){
			std::string s=visit(x).as<std::string>();
			if( Func::nw ->n_value[ s ] ) lst -> v.push_back( Func::nw ->n_value[ s ] );
			else{
				Func::nw -> add_ele( s , Value() , Func::nw->n_value );
				lst -> v.push_back( Func::nw ->n_value[ s ] );
			}
		}
		else lst -> add( visit(x).as<Value>() );

	return (*lst);
}

antlrcpp::Any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
	return visitChildren(ctx);
}