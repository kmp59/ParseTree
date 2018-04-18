/*
 * Author: Kevin Patel
 */
#ifndef CS280_PARSETREE_H
#define CS280_PARSETREE_H

#include <vector>
#include <map>
#include "parse.h"
#include "projlex.h"
using std::vector;
using std::map;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE };

#include "value.h"

class ParseTree {
    int			linenum;


public:
    ParseTree	*left;
    ParseTree	*right;
    ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
            : linenum(linenum), left(l), right(r) {}

    virtual ~ParseTree() {
        delete left;
        delete right;
    }

    int GetLineNumber() const { return linenum; }

    virtual NodeType GetType() const { return ERRTYPE; }

    virtual Value eVal(map <string, Value> &symTab){
        if(left){
            left->eVal(symTab);
        }
        if(right){
            right->eVal(symTab);
        }
        return Value();
    }
};

class StmtList : public ParseTree {

public:
    StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
};

class VarDecl : public ParseTree {
public:
    VarDecl(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}};

class Assignment : public ParseTree {
public:
    Assignment(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}};

class Print : public ParseTree {
public:
    Print(int line, ParseTree *l) : ParseTree(line, l) {}
    Value eVal(map <string, Value> &symTab){
        Value val = left-> eVal(symTab);
        cout << val << endl;
        return val;
    }
};

class Repeat : public ParseTree {
public:
    Repeat(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class PlusExpr : public ParseTree {
public:
    PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    Value eVal(map <string, Value> &symTab){
        Value leftVal = left-> eVal(symTab);
        Value rightVal = right-> eVal(symTab);
        Value finalVal = leftVal + rightVal;
        return finalVal;
    }
};

class MinusExpr : public ParseTree {
public:
    MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    Value eVal(map <string, Value> &symTab){
        Value leftVal = left-> eVal(symTab);
        Value rightVal = right-> eVal(symTab);
        Value finalVal = leftVal - rightVal;
        return finalVal;
    }
};

class TimesExpr : public ParseTree {
public:
    TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    Value eVal(map <string, Value> &symTab){
        Value leftVal = left-> eVal(symTab);
        Value rightVal = right-> eVal(symTab);
        Value finalVal = leftVal * rightVal;
        return finalVal;
    }
};

class SliceExpr : public ParseTree {
public:
    SliceExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}};

class SliceOperand : public ParseTree {
public:
    SliceOperand(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}};

class IConst : public ParseTree {
    int val;

public:
    IConst(Token& t) : ParseTree(t.GetLinenum()) {
        val = stoi(t.GetLexeme());
    }

    NodeType GetType() const { return INTTYPE; }
    Value eVal(map <string, Value> &symTab){
        return Value(val);
    }
};

class SConst : public ParseTree {
    string val;

public:
    SConst(Token& t) : ParseTree(t.GetLinenum()) {
        val = t.GetLexeme();
    }

    NodeType GetType() const { return INTTYPE; }
    Value eVal(map <string, Value> &symTab){
        return Value(val);
    }
};

class Ident : public ParseTree {
    //int val;

public:
    Ident(Token& t) : ParseTree(t.GetLinenum()) {
        //val = stoi(t.GetLexeme());
    }

    NodeType GetType() const { return INTTYPE; }
};

#endif //CS280_PARSETREE_H