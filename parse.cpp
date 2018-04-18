/*
 * Author: Kevin Patel
 */
#include "parse.h"
#include "projlex.h"
#include "parsetree.h"
#include <vector>

namespace Parser {
    bool pushed_back = false;
    Token	pushed_token;

    static Token GetNextToken(istream *in, int *line) {
        if( pushed_back ) {
            pushed_back = false;
            return pushed_token;
        }
        return getNextToken(in, line);
    }

    static void PushBackToken(Token& t) {
        if( pushed_back ) {
            abort();
        }
        pushed_back = true;
        pushed_token = t;
    }

}

static int error_count = 0;

void
ParseError(int line, string msg)
{
    cout << line<< ": " << msg << endl;
    error_count++;
}

ParseTree *Prog(istream *in, int *line)
{
    ParseTree *sl = Slist(in, line);

    if( sl == 0 )
        ParseError(*line, "No statements in program");

    if( error_count )
        return 0;

    return sl;
}

// Slist is a Statement followed by a Statement List
ParseTree *Slist(istream *in, int *line) {
    ParseTree *s = Stmt(in, line);
    if( s == 0 )
        return 0;
    Token Tok = Parser::GetNextToken(in, line);
    if(Tok.GetTokenType() != SC){
        ParseError(*line, "No Semicolon!");
        Parser::PushBackToken(Tok);
    }

    while(true){
        Token tok = Parser::GetNextToken(in, line);
        if(tok.GetTokenType() == DONE){
            return s;
        }
        Parser::PushBackToken(tok);
        return new StmtList(s, Slist(in,line));
    }
}

ParseTree *Stmt(istream *in, int *line) {
    Token tok = Parser::GetNextToken(in, line);
    if(tok.GetTokenType() == VAR){
        return VarStmt(in, line);
    }
    else if(tok.GetTokenType() == SET){
        return SetStmt(in, line);
    }
    else if(tok.GetTokenType() == PRINT){
        return PrintStmt(in, line);
    }
    else if(tok.GetTokenType() == REPEAT){
        return RepeatStmt(in, line);
    }else{
        ParseError(*line, "Incorrect Statement!");
        return 0;
    }
}

ParseTree *VarStmt(istream *in, int *line) {
    Token tok = Parser::GetNextToken(in, line);
    if(tok.GetTokenType() != IDENT){
        ParseError(*line, "Not an Identifier!");
        return 0;
    }
    ParseTree *expr = Expr(in, line);
    if(expr == 0){
        ParseError(*line, "Missing Expression!");
        //Parser::PushBackToken()
        return 0;
    }

    return new VarDecl(*line, new Ident(tok), expr);
}

ParseTree *SetStmt(istream *in, int *line) {
    Token tok = Parser::GetNextToken(in, line);
    if(tok.GetTokenType() != IDENT){
        ParseError(*line, "Not an Identifier!");
        return 0;
    }
    ParseTree *expr = Expr(in, line);
    if(expr == 0){
        ParseError(*line, "Missing Expression!");
        return 0;
    }
    return new Assignment(*line, new Ident(tok), expr);
}

ParseTree *PrintStmt(istream *in, int *line) {
    int l = *line;

    ParseTree *ex = Expr(in, line);
    if( ex == 0 ) {
        ParseError(*line, "Missing expression after print!");
        return 0;
    }

    return new Print(l, ex);
}

ParseTree *RepeatStmt(istream *in, int *line) {
    int li = *line;

    ParseTree *expr = Expr(in, line);
    if(expr == 0){
        ParseError(*line, "Missing expression after repeat!");
        return 0;
    }
    ParseTree *stmt = Stmt(in, line);
    if(stmt == 0){
        ParseError(*line, "Missing statement after repeat!");
        return 0;
    }

    return new Repeat(li, expr, stmt);
}

ParseTree *Expr(istream *in, int *line) {
    ParseTree *t1 = Term(in, line);
    if( t1 == 0 ) {
        return 0;
    }

    while ( true ) {
        Token t = Parser::GetNextToken(in, line);

        if( t.GetTokenType() != PLUS && t.GetTokenType() != MINUS ) {
            Parser::PushBackToken(t);
            return t1;
        }

        ParseTree *t2 = Term(in, line);
        if( t2 == 0 ) {
            ParseError(*line, "Missing expression after operator");
            return 0;
        }

        if( t.GetTokenType() == PLUS )
            t1 = new PlusExpr(t.GetLinenum(), t1, t2);
        else
            t1 = new MinusExpr(t.GetLinenum(), t1, t2);
    }
}

ParseTree *Term(istream *in, int *line) {
    ParseTree *fact = Factor(in, line);
    if(fact == 0){
        ParseError(*line, "Missing Factor!");
    }
    while(true) {
        Token T = Parser::GetNextToken(in, line);

        if (T.GetTokenType() != STAR) {
            Parser::PushBackToken(T);
            //ParseError(*line, "Not a star!");
            return fact;
        }

        ParseTree *factor = Factor(in, line);
        if (factor == 0) {
            ParseError(*line, "There is no Factor!");
            return 0;
        }
        fact = new TimesExpr(*line, fact, factor);
    }
}


ParseTree *Factor(istream *in, int *line) {
    ParseTree *Prime = Primary(in, line);
    if(Prime == 0){
        ParseError(*line, "Missing Primary!");
        return 0;
    }

    while(true){
        Token tok = Parser::GetNextToken(in, line);

        if(tok.GetTokenType() == LSQ){
            ParseTree *expr = Expr(in, line);
            if(expr == 0){
                ParseError(*line, "Missing Expression!");
                return 0;
            }
            tok = Parser::GetNextToken(in, line);
            if(tok.GetTokenType() != COLON) {
                ParseError(*line, "Missing Colon!");
            }
            ParseTree *expr2 = Expr(in, line);
            if(expr2 == 0){
                ParseError(*line, "Missing Expression!");
                return 0;
            }
            tok = Parser::GetNextToken(in, line);
            if(tok.GetTokenType() != RSQ){
                ParseError(*line, "Missing Right Square Bracket!");
            }
            Prime = new SliceExpr(*line, Prime, new SliceOperand(*line, expr, expr2));
        } else{
            Parser::PushBackToken(tok);
            return Prime;
        }
    }
}

ParseTree *Primary(istream *in, int *line) {
    Token tok = Parser::GetNextToken(in, line);

    if(tok.GetTokenType() == IDENT){

        return new Ident(tok);
    } else if(tok.GetTokenType() == ICONST){

        return new IConst(tok);
    } else if(tok.GetTokenType() == SCONST){

        return new SConst(tok);
    } else if(tok.GetTokenType() == LPAREN){
        ParseTree *expr = Expr(in, line);
        if(expr == 0){
            ParseError(*line, "Missing Expression!");
            return 0;
        }
        tok = Parser::GetNextToken(in, line);
        if(tok.GetTokenType() != RPAREN){
            ParseError(*line, "Missing Right Paranthesis");
            return 0;
        }
        return expr;

    }else {
        ParseError(*line, "Error!");
        return 0;
    }
}