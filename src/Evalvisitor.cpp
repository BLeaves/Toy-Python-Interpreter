#include "Evalvisitor.h"


antlrcpp::Any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
	return visitChildren(ctx);
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
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
	auto T = ctx -> testlist() ;

	if( ctx -> augassign() ){
		switch ( ctx -> augassign() -> getText() ){
		case "+=":	return visit( T[0] ).as<List>() += visit( T[1] ).as<List>();
		case "-=":	return visit( T[0] ).as<List>() -= visit( T[1] ).as<List>();
		case "*=":	return visit( T[0] ).as<List>() *= visit( T[1] ).as<List>();
		case "/=":	return visit( T[0] ).as<List>() /= visit( T[1] ).as<List>();
		case "//=":	return visit( T[0] ).as<List>() .divide_e( visit( T[1] ).as<List>() ) ;
		case "%=":	return visit( T[0] ).as<List>() %= visit( T[1] ).as<List>();
		}
	}
	else{
		if( ctx -> ASSIGN() ){
			for(auto x:T){
				x = 
			}
		}
		else return visitChildren( ctx );
	}
}

antlrcpp::Any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
	auto T = ctx->test();
	auto S = ctx->suite();


	for(int i = 0;i < T.size() ;i ++ )
		if( visit( T[i] ).as<Value>().small_value() > 0.5 ){
			visit( S[i] ); return;
		}
	
	visit( S.back() );
}

antlrcpp::Any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
	if( ctx->simple_stmt() ) visitChildren(ctx);
	if( ctx->stmt() ) visit( ctx->stmt() );
	return ;
}

antlrcpp::Any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
	if( ctx->OR() ){
		for( auto x: ctx->and_test() )
			if( visit(x).as<Value>().small_value() > 0.3 ) return Value( true );
		return Value( false );
	}
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
	if( ctx->AND() ){
		for( auto x: ctx->not_test() )
			if( visit(x).as<Value>().small_value() < 0.8 ) return Value( false );
		return Value( true );
	}
	else return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
	if( ctx->NOT() ) return !( visit( ctx->not_test() ).as<Value>() );
	return visitChildren(ctx).as<Value>().trans_b();
}

antlrcpp::Any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
	auto F = ctx->arith_expr();
	auto O = ctx->comp_op();

	Value ans(true);

	if(O){

		for(int i = 0;i < O.size() ;i ++ ){
			if( (O[i] -> getText())  == "<" ) 
				if(!( visit(F[i]).as<Value>() < visit(F[i+1]).as<Value>() ) ) return Value( false );
			if( (O[i] -> getText())  == ">" ) 
				if(!( visit(F[i]).as<Value>() > visit(F[i+1]).as<Value>() ) ) return Value( false ); 
			if( (O[i] -> getText())  == "<=" ) 
				if(!( visit(F[i]).as<Value>() <= visit(F[i+1]).as<Value>() ) ) return Value( false ); 
			if( (O[i] -> getText())  == ">=" ) 
				if(!( visit(F[i]).as<Value>() >= visit(F[i+1]).as<Value>() ) ) return Value( false ); 
			if( (O[i] -> getText())  == "==" ) 
				if(!( visit(F[i]).as<Value>() == visit(F[i+1]).as<Value>() ) ) return Value( false ); 
			if( (O[i] -> getText())  == "!=" ) 
				if(!( visit(F[i]).as<Value>() != visit(F[i+1]).as<Value>() ) ) return Value( false ); 
		}
	}
	else  return  F . as<Value>() . trans_b();

	return ans;
}

antlrcpp::Any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
	auto F = ctx->term();
	auto O = ctx->addorsub_op();

	Value ans;

	if( O ){
		ans = visit( F[0] ) . as<Value>() ;

		for(int i = 0;i < O.size() ;i ++ ){
			if( (O[i] -> getText())  == "+" ) ans += visit( F[i+1] ).as<Value>();
			if( (O[i] -> getText())  == "-" )  ans -= visit( F[i+1] ).as<Value>();
		}
	}
	else ans = visit(F) . as<Value>();

	return ans;
}

antlrcpp::Any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
	auto F = ctx->factor();
	auto O = ctx->muldivmod_op();

	Value ans;

	if(O){
		ans = visit( F[0] ) . as<Value>() ;

		for(int i = 0;i < O.size() ;i ++ ){
			if( (visit(O[i]) -> getText())  == "*" ) ans *= visit( F[i+1] ).as<Value>();
			if( (visit(O[i]) -> getText())  == "/" )  ans /= visit( F[i+1] ).as<Value>();
			if( (visit(O[i]) -> getText())  == "//" ) ans .divide_e( visit( F[i+1] ).as<Value>() );
			if( (visit(O[i]) -> getText())  == "%" ) ans %= visit( F[i+1] ).as<Value>();
		}
	}
	else ans = visitChildren(ctx) . as<Value>();

	return ans;
}

antlrcpp::Any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
	if( ctx->atom_expr() ){

	}
	else{
		Value ans = visit( ctx->factor() ).as<Value>();
		if(ctx->MINUS()) ans *= Value( -1 );
	}
}

antlrcpp::Any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
	List *lst=new List;

	for(auto x:ctx->test())
		lst->add( visit(x) );
	
	return (*lst);
}

antlrcpp::Any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
	return visitChildren(ctx);
}