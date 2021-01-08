#include "Evalvisitor.h"
#include <map>
#include <iostream>
#include <vector>
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
		auto l = ctx->testlist(0)->test();
		auto r = ctx->testlist(1)->test();
		if( ctx -> augassign() -> getText() == "+=" ) {
			for(auto li = l.begin(), ri = r.begin();li != l.end();li ++, ri ++)
				(*(Func::nw->getptr((*li)->getText()))) += visit(*ri).as<Value>();
		}
		if( ctx -> augassign() -> getText() == "-=" ) {
			for(auto li = l.begin(), ri = r.begin();li != l.end();li ++, ri ++)
				(*(Func::nw->getptr((*li)->getText()))) -= visit(*ri).as<Value>();
		}
		if( ctx -> augassign() -> getText() == "*=" ) {
			for(auto li = l.begin(), ri = r.begin();li != l.end();li ++, ri ++)
				(*(Func::nw->getptr((*li)->getText()))) *= visit(*ri).as<Value>();
		}
		if( ctx -> augassign() -> getText() == "/=" ) {
			for(auto li = l.begin(), ri = r.begin();li != l.end();li ++, ri ++)
				(*(Func::nw->getptr((*li)->getText()))) /= visit(*ri).as<Value>();
		}
		if( ctx -> augassign() -> getText() == "//=" ) {
			for(auto li = l.begin(), ri = r.begin();li != l.end();li ++, ri ++)
				(*(Func::nw->getptr((*li)->getText()))).divide_e(visit(*ri).as<Value>());
		}
		if( ctx -> augassign() -> getText() == "%=" ) {
			for(auto li = l.begin(), ri = r.begin();li != l.end();li ++, ri ++)
				(*(Func::nw->getptr((*li)->getText()))) %= visit(*ri).as<Value>();
		}
	}
	else{
		auto t = visit(T.back()).as<std::vector<Value>>();
		for(auto it = ++T.rbegin(); it != T.rend(); ++it) {
			for(int i = 0; i < t.size(); i++ )
				*(Func::nw->getptr((*it)->test(i)->getText())) = t[i];
		}
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
	throw antlrcpp::Any(std::vector<Value>(1));
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
		if( visit( T[i] ).as<Value>().trans_b().b ){
			visit( S[i] ); return Value();
		}

	if (T.size() != S.size()) visit( S.back() );
	return Value();
}

antlrcpp::Any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
	while( visit( ctx -> test() ).as<Value>().trans_b().b ){
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
	return visit(ctx->or_test());
}

antlrcpp::Any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
	if( ctx -> OR() .size() ){
		for( auto x: ctx->and_test() )
			if( visit(x).as<Value>().trans_b().b ) return Value( true );
		return Value( false );
	}
	return visit(ctx->and_test(0));
}

antlrcpp::Any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
	if( ctx -> AND() . size() ){
		for( auto x: ctx->not_test() )
			if( not visit(x).as<Value>().trans_b().b ) return Value( false );
		return Value( true );
	}
	return visit(ctx->not_test(0));
}

antlrcpp::Any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
	if( ctx->NOT() ) return !( visit( ctx->not_test() ).as<Value>() );
	return visit(ctx->comparison());
}

antlrcpp::Any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
	auto F = ctx->arith_expr();
	auto O = ctx->comp_op();

	if( O . size() ){
		std::vector< Value > vct;

		vct.push_back( visit( F[0] ).as<Value>() );

		for(int i = 0;i < O.size() ;i ++ ){
			vct.push_back( visit( F[i+1] ).as<Value>() );

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
		return Value( true );
	}
	return  visit( F[0] );
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
	if( ctx -> atom_expr() ) return visit(ctx->atom_expr());
	if( ctx -> MINUS() ) return visit( ctx -> factor() ).as<Value>() * Value( I("-1") );
	return visit( ctx -> factor() );
}

antlrcpp::Any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
	if( ctx -> trailer() ){
		if( ctx -> atom() -> getText() == "print" ){
			auto arglist = ctx -> trailer() -> arglist();
			if(arglist == nullptr) return Value();

			auto vct = arglist -> argument();
			visit(vct[0]).as<Value>().print();

			for(int i = 1;i < vct.size();i ++){
				std::cout<<' ';
				visit( vct[i] ).as<Value>().print();
			}

			std::cout<<std::endl;
			return Value();
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
		auto newcall = new Func(*(m_func[ ctx->atom()->getText() ]));
		auto lst = newcall -> run( ctx -> trailer() ->arglist() ).as<std::vector<Value>>();
		if (lst.size() == 1) return lst[0];
		return lst;
	}
	return visit(ctx->atom());
}

antlrcpp::Any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
	// printf("visitatom: %s\n", ctx->getText().c_str());
	if( ctx -> NAME() ) return Value( *(Func::nw->getptr(ctx -> getText())) );
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
	return visit(ctx->test());

}

antlrcpp::Any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
	std::vector<Value> lst;
	auto v = ctx -> test();
	for(auto x:v) {
		auto t = visit(x);
		if (t.is<std::vector<Value>>()) return t.as<std::vector<Value>>();
		lst.push_back(t.as<Value>());
	}
	return lst;
}

antlrcpp::Any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
	return visitChildren(ctx);
}
