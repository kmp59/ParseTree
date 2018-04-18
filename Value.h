/*
 * Author: Kevin Patel
 */
#ifndef VALUE_H_
#define VALUE_H_

#include "parsetree.h"
#include "projlex.h"

class Value {
    int	ival;
    string sval;
    NodeType type;

public:
    Value() : ival(0),type(ERRTYPE) {}
    Value(int i) : ival(i),type(INTTYPE) {}
    Value(string s) : ival(0),sval(s),type(STRTYPE) {}

    NodeType GetType() const { return type; }

    int GetIntValue() const {
        if( type != INTTYPE )
            throw std::runtime_error("using GetIntValue on a Value that is not an INT");
        return ival;
    }

    string GetStrValue() const {
        if( type != STRTYPE )
            throw std::runtime_error("using GetStrValue on a Value that is not a STRING");
        return sval;
    }
    friend ostream &operator <<(ostream& output, Value& val){
        if(val.GetType() == INTTYPE){
            output << val.GetIntValue();
        }
        else if(val.GetType() == STRTYPE){
            output << val.GetStrValue();
        }
        return output;
    }
    Value operator + (Value& value){
        if(value.GetType() == INTTYPE){
            if(type == INTTYPE){
                return Value(ival + value.GetIntValue());
            }else {
                throw std::runtime_error("using GetIntValue on a Value that is not an INT");
            }
        }
        else if(value.GetType() == STRTYPE){
            if(type == STRTYPE){
                return Value(sval + value.GetStrValue());
            }
        }
        return Value();
    }
    Value operator - (Value& value){
        if(value.GetType() == INTTYPE){
            if(type == INTTYPE){
                return Value(ival - value.GetIntValue());
            }else{
                throw std::runtime_error("using GetIntValue on a Value that is not an INT");
            }
        }else {
            throw std::runtime_error("using GetIntValue on a Value that is not an INT");
        }
    }
    Value operator * (Value& value){
        if(value.GetType() == INTTYPE){
            if(type == INTTYPE){
                return Value(ival * value.GetIntValue());
            }else {
                throw std::runtime_error("using GetIntValue on a Value that is not an INT");
            }
        }
        return Value();
    }
};

#endif /* VALUE_H_ */