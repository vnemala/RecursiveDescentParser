#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <iomanip>

using namespace std;

enum ValType { VINT, VREAL, VSTRING, VBOOL, VERR };

class Value {
    ValType	T;
    bool    Btemp;
	int		Itemp;
	float   Rtemp;
    string	Stemp;
       
public:
    Value() : T(VERR), Btemp(false), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(bool vb) : T(VBOOL), Btemp(vb), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(int vi) : T(VINT), Btemp(false), Itemp(vi), Rtemp(0.0), Stemp("") {}
	Value(float vr) : T(VREAL), Btemp(false), Itemp(0), Rtemp(vr), Stemp("") {}
    Value(string vs) : T(VSTRING), Btemp(false), Itemp(0), Rtemp(0.0), Stemp(vs) {}
    
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsString() const { return T == VSTRING; }
    bool IsReal() const {return T == VREAL;}
    bool IsBool() const {return T == VBOOL;}
    
    int GetInt() const { if( IsInt() ) return Itemp; throw "RUNTIME ERROR: Value not an integer"; }
    
    string GetString() const { if( IsString() ) return Stemp; throw "RUNTIME ERROR: Value not a string"; }
    
    float GetReal() const { if( IsReal() ) return Rtemp; throw "RUNTIME ERROR: Value not an integer"; }
    
    bool GetBool() const {if(IsBool()) return Btemp; throw "RUNTIME ERROR: Value not a boolean";}
    
    void SetType(ValType type)
    {
    	T = type;
	}
	
    void SetInt(int val)
    {
    	Itemp = val;
	}
	
	void SetReal(float val)
    {
    	Rtemp = val;
	}
	
	void SetString(string val)
    {
    	Stemp = val;
	}
	
	void SetBool(bool val)
    {
    	Btemp = val;
	}
	
	
    // add op to this
    Value operator+(const Value& op) const {
        if (IsInt() && op.IsInt()) // integers only
            return GetInt() + op.GetInt();
        if (IsReal() && op.IsReal()) // reals only
            return GetReal() + op.GetReal();
        if (IsInt() && op.IsReal()) // int + real
            return GetInt() + op.GetReal();
        if (IsReal() && op.IsInt()) // real + int
            return GetReal() + op.GetInt();
        return Value();   
    }
    
    // subtract op from this
    Value operator-(const Value& op) const {
        if (IsInt() && op.IsInt()) // integers only
            return GetInt() - op.GetInt();
        if (IsReal() && op.IsReal()) // reals only
            return GetReal() - op.GetReal();
        if (IsInt() && op.IsReal()) // int - real
            return GetInt() - op.GetReal();
        if (IsReal() && op.IsInt()) // real - int
            return GetReal() - op.GetInt();
        return Value();

    }
    
    // multiply this by op
    Value operator*(const Value& op) const {
        if (IsInt() && op.IsInt()) // integers only
            return GetInt() * op.GetInt();
        if (IsReal() && op.IsReal()) // reals only
            return GetReal() * op.GetReal();
        if (IsInt() && op.IsReal()) // int * real
            return GetInt() * op.GetReal();
        if (IsReal() && op.IsInt()) // real * int
            return GetReal() * op.GetInt();
        return Value();

    }
    
    // divide this by op
    Value operator/(const Value& op) const {
        if (IsInt() && op.IsInt()) // integers only
            return GetInt() / op.GetInt();
        if (IsReal() && op.IsReal()) // reals only
            return GetReal() / op.GetReal();
        if (IsInt() && op.IsReal()) // int / real
            return GetInt() / op.GetReal();
        if (IsReal() && op.IsInt()) // real / int
            return GetReal() / op.GetInt();
        return Value();   
    }
    
    Value operator==(const Value& op) const {
        if (IsInt() && op.IsInt()) // integers only
        {
            if (GetInt() == op.GetInt()) // int == int
                return Value(true);
            else
                return Value(false);
        }
        
        if (IsReal() && op.IsReal()) {// reals only 
            if (GetReal() == op.GetReal()) // real == real
                return Value(true);
            else
                return Value(false); }
        if (IsBool() && op.IsBool()) {
            if (GetBool() == op.GetBool()) // bool == bool
                return Value(true);
            else
                return Value(false);
        }
        if (IsInt() && op.IsReal()) // int / real
            return Value(true);
        if (IsReal() && op.IsInt()) // real / int
            return Value(true);
        return Value();    
    }

	Value operator>(const Value& op) const {
        if (IsBool() && op.IsBool()) {
            if (GetBool() > op.GetBool()) // bool > bool
                return Value(true);
            else
                return Value(false);
        }
        if (IsInt() && op.IsReal()) {
           if (GetInt() > op.GetReal()) // int > real
               return Value(true);
           else
               return Value(false);   
        }      
        if (IsReal() && op.IsInt()) {
           if (GetReal() > op.GetInt()) // real > int
               return Value(true);
           else
               return Value(false);  
        }
        if (IsInt() && op.IsInt()) {
           if (GetInt() > op.GetInt()) // int > int
               return Value(true);
           else
               return Value(false);  
        }
        if (IsReal() && op.IsReal()) {
           if (GetReal() > op.GetReal()) // real > real
               return Value(true);
           else
               return Value(false);  
        }
        return Value();
    }
	
	Value operator<(const Value& op) const {
        if (IsBool() && op.IsBool()) {
            if (GetBool() < op.GetBool()) // bool < bool
                return Value(true);
            else
                return Value(false);
        }
        if (IsInt() && op.IsReal()) {
           if (GetInt() < op.GetReal()) // int < real
               return Value(true);
           else
               return Value(false);   
        }      
        if (IsReal() && op.IsInt()) {
           if (GetReal() < op.GetInt()) // real < int
               return Value(true);
           else
               return Value(false);  
        }
        if (IsInt() && op.IsInt()) {
           if (GetInt() < op.GetInt()) // int < int
               return Value(true);
           else
               return Value(false);  
        }
        if (IsReal() && op.IsReal()) {
           if (GetReal() < op.GetReal()) // real < real
               return Value(true);
           else
               return Value(false);  
        }
        return Value(true);
    }
	
	Value operator&&(const Value& op) const {
        if (IsBool() && op.IsBool()) {
            if (GetBool() && op.GetBool()) // bool && bool
                return Value(true);
            else
                return Value(false);
        }
        
        return Value();

    }
	
	Value operator||(const Value& op) const{
        if (IsBool() && op.IsBool()) {
            if (GetBool() || op.GetBool()) // bool || bool
                return Value(true);
            else
                return Value(false);
        }
        
        return Value();
    }
	
	Value operator!() const 
    {
        if (IsBool()) {
            return (!GetBool());   
        }
        else {
            return Value();   
        }
    }
    
    
    //NOT Unary operator
	    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.Itemp;
        else if( op.IsString() ) out << op.Stemp;
        else if( op.IsReal()) out << fixed << showpoint << setprecision(2) << op.Rtemp;
        else if(op.IsBool()) out << (op.GetBool()? "true" : "false");
        else out << "ERROR";
        return out;
    }
};


#endif
