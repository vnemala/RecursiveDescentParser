/* Implementation of Recursive-Descent Parser
 * parseInt.cpp
 * Programming Assignment 3
 * Fall 2022
*/
//19/20
#include "parseInt.h"

map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(istream& in, int& line);

//Program is: Prog ::= PROGRAM IDENT StmtList END PROGRAM
bool Prog(istream& in, int& line)
{
	bool f1;
	LexItem tok = Parser::GetNextToken(in, line);
		
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			
			f1 = StmtList(in, line); 
			
			if(f1) {
				tok = Parser::GetNextToken(in, line);
				if( tok == END){
					tok = Parser::GetNextToken(in, line);
					if(tok != PROGRAM){
						ParseError(line, "Missing PROGRAM Keyword.");
						return false;
					}
					return true;
				}
				else
				{
					ParseError(line, "Missing END of Program.");
					return false;
				}
				
			}
			else
			{
				ParseError(line, "Incorrect Program Body.");
				return false;
			}
		}
		else
		{
			ParseError(line, "Missing Program Name.");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	ParseError(line, "Missing PROGRAM.");
	return false;
}//End of Prog

//StmtList ::= Stmt; { Stmt; }
bool StmtList(istream& in, int& line){
	bool status;
		
	LexItem tok;
	
	status = Stmt(in, line);
		
	while(status)
	{
		tok = Parser::GetNextToken(in, line);
		if(tok != SEMICOL)
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
			
		status = Stmt(in, line);
	}
			
	tok = Parser::GetNextToken(in, line);
	if(tok == END )
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else if(tok == ELSE)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else 
	{
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}
	
}//End of StmtList function

//Stmt ::= DeclStmt | ControlStmt
bool Stmt(istream& in, int& line){
	bool status=false;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {
	case INT: case FLOAT: case BOOL:
		Parser::PushBackToken(t);
		status = DeclStmt(in, line);
		if(!status)
		{
			ParseError(line, "Incorrect Declaration Statement.");
			return status;
		}
		break;
	case IF: case PRINT: case IDENT: 
		Parser::PushBackToken(t);
		status = ControlStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect control Statement.");
			return status;
		}
		break;
	default:
		Parser::PushBackToken(t);
	}
	return status;
}//End of Stmt function

//DeclStmt ::= ( INT | FLOAT | BOOL ) VarList
bool DeclStmt(istream& in, int& line)
{
	LexItem t;
	bool status;
	
	t = Parser::GetNextToken(in, line);
	if(t == INT || t == FLOAT || t == BOOL)
	{
		status = VarList(in, line, t);
		
		if (!status)
		{
			ParseError(line, "Incorrect variable in Declaration Statement.");
			return status;
		}
	}
	else
	{
		ParseError(line, "Incorrect Declaration Type.");
		return false;
	}
	
	return true;
		
}//End of DeclStmt

//VarList:= Var {,Var}
bool VarList(istream& in, int& line, LexItem& type) {
	bool status = false;
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
        Token typeToken = type.GetToken();
		SymTable[identstr] = typeToken;
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = VarList(in, line, type);
	}
	else {
		Parser::PushBackToken(tok);
		return true;
	}
	return status;	
}//End of VarList
	

//ControlStmt ::= AssigStmt | IfStmt | PrintStmt 
bool ControlStmt(istream& in, int& line) {
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case PRINT:
		status = PrintStmt(in, line);
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		break;
		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}//End of ControlStmt


//PrintStmt:= PRINT (ExpreList) 
bool PrintStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in PrintStmt" << endl;
	ValQue = new queue<Value>;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression list after Print");
		while(!(*ValQue).empty())
		{
			ValQue->pop();		
		}
		delete ValQue;
		return false;
	}
	
	//Evaluate: print out the list of expressions' values
    //cout << "ValQue" << endl;
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	cout << endl;
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	return true;
}//End of PrintStmt

//IfStmt:= IF (Expr) then StmtList [Else StmtList] END IF
bool IfStmt(istream& in, int& line) {
	bool ex = false, status ; 
	LexItem t;
	Value val1;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	ex = Expr(in, line, val1);
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	if (val1.GetBool()) {
        t = Parser::GetNextToken(in, line);
        if(t != THEN)
        {
            ParseError(line, "If Statement Syntax Error");
            return false;
        }
        status = StmtList(in, line);
        if(!status)
        {
            ParseError(line, "Missing Statement for If-Stmt Then-Part");
            return false;
        }
        //t = Parser::GetNextToken(in, line);
        while (t!=END) {
            t = Parser::GetNextToken(in, line);
        }
    }
    else {
        t = Parser::GetNextToken(in, line);
        //cout << "token before while loop: " << t << endl;
        do {
            t = Parser::GetNextToken(in, line);
        }
        while (t != END && t != ELSE);
        //cout << "token is now " << t;
        //while (t != ELSE || t != END) {
        //    t = Parser::GetNextToken(in, line);
        //    cout << t << " ";
        //}
        if( t == ELSE ) {
            status = StmtList(in, line);
            if(!status)
            {
                ParseError(line, "Missing Statement for If-Stmt Else-Part");
                return false;
            }
            else
            {
                t = Parser::GetNextToken(in, line);
            }
	    }
    }
	if( t == END){
		t = Parser::GetNextToken(in, line);
		if(t != IF){
			ParseError(line, "Missing closing keywords of IF statement.");
			return false;
		}
		return true;
	}
	else
	{
		ParseError(line, "Missing END of IF.");
		return false;
	}
	
}//End of IfStmt function

//Var:= ident
bool Var(istream& in, int& line, LexItem & idtok)
{
	string identstr;
		
	if (idtok == IDENT){
		identstr = idtok.GetLexeme();
		//cout<< "in assigstmt var: " << identstr << endl;
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}	
		return true;
	}
	else if(idtok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << idtok.GetLexeme() << ")" << endl;
		return false;
	}
	
	return false;
}//End of Var

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	Value val1;
	bool varstatus = false, status = false;
	LexItem t;
    t = Parser::GetNextToken(in, line);
    string str = t.GetLexeme();
	varstatus = Var( in, line, t);
	
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		
		if (t == ASSOP){
            
			status = Expr(in, line, val1);
            //check val1 type is the same as type of identifier (str) -- otherwise type mismatch 
            //enum ValType { VINT, VREAL, VSTRING, VBOOL, VERR };
            Token tokVar = SymTable.find(str)->second;
            //Token tokExpr;
            if (val1.GetType() == VINT) {
                int val1ValueInt = val1.GetInt();
                //tokExpr = Token(INT); 
                if (tokVar == FLOAT){
                    float val1New = (float)(val1ValueInt);
                    val1 = Value(val1New);
                }
            }
            else if (val1.GetType() == VREAL) {
                float val1ValueFloat = val1.GetReal();
                //tokExpr = Token(FLOAT); 
                if (tokVar == INT) {
                    int val1New = (int)(val1ValueFloat);
                    val1 = Value(val1New);
                }
            }
            if (tokVar == BOOL && val1.GetType() != VBOOL) {
                ParseError(line, "Illegal Assignment Operation");
				return false;
            }
			TempsResults[str] = val1;
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
			
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;	
}//End of AssignStmt

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value val1;
	status = Expr(in, line, val1);
    if (val1.IsErr()) {
        ParseError(line, "Undefined Variable and Value Error");
		return false;
    }
    ValQue -> push(val1);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
		
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList

//Expr ::= LogORExpr ::= LogANDExpr { || LogANDRxpr }
bool Expr(istream& in, int& line, Value & retVal) {
	Value val1, val2;
	LexItem tok;
	bool t1 = LogANDExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
	
	retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == OR ) 
	{
		t1 = LogANDExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator OR");
			return false;
		}
		retVal = retVal || val2;
		//cout << "OR op result " << retVal.IsBool() << " " << retVal.GetBool() << endl;
		if(retVal.IsErr())
		{
			ParseError(line, "Illegal OR operation.");
			//cout << "(" << tok.GetLexeme() << ")" << endl;		
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of Expr/LogORExpr


//LogANDExpr ::= EqualExpr { && EqualExpr }
bool LogANDExpr(istream& in, int& line, Value & retVal) {
	LexItem tok;
	Value val1, val2;
	bool t1 = EqualExpr(in, line, val1);
	//cout << "status of EqualExpr and val1: " << t1<< " " << val1.IsErr() << " " << val1.IsBool() << endl;
	//cout << "status of EqualExpr: " << t1<< endl;
		
	if( !t1 ) {
		return false;
	}
	//cout << "value of var1 in AND op " << val1.GetBool() << endl;
	retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == AND ) 
	{
		t1 = EqualExpr(in, line, val2);
		//cout << "value of var1 in AND op " << val1.GetBool() << endl;
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator AND");
			return false;
		}
		//evaluate expression for Logical AND
		retVal = retVal && val2;
		//cout << "AND op result " << retVal.IsBool() << " " << retVal.GetBool() << endl;
		if(retVal.IsErr())
		{
			ParseError(line, "Illegal AND operation.");
			//cout << "(" << tok.GetLexeme() << ")" << endl;		
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	
	Parser::PushBackToken(tok);
	return true;
}//End of LogANDExpr

//EqualExpr ::= RelExpr [ == RelExpr ]
bool EqualExpr(istream& in, int& line, Value & retVal) {
	LexItem tok;
	Value val1, val2;
	bool t1 = RelExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == EQUAL ) 
	{
		t1 = RelExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator EQUAL");
			return false;
		}
		//evaluate expression for Logical ==
		retVal = (retVal == val2);
		if(retVal.IsErr())
		{
			ParseError(line, "Illegal == operation.");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if(tok == EQUAL)
		{
			ParseError(line, "Illegal Equality Expression.");
			return false;
		}
		else if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of EqualExpr

//RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
bool RelExpr(istream& in, int& line, Value & retVal) {
	LexItem tok;
	Value val1, val2;
	bool t1 = AddExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == LTHAN || tok == GTHAN) 
	{
		t1 = AddExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator LGTHAN");
			return false;
		}
		
		if (tok == LTHAN){
			//evaluate expression for <
			retVal = retVal < val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal < operation.");
				return false;
			}
		}
		else if (tok == GTHAN){
			//evaluate expression for >
            //cout << "retVal is " << retVal << " and val2 is " << val2 << endl;
			retVal = retVal > val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal > operation.");
				return false;
			}
		}
		tok = Parser::GetNextToken(in, line);
		
		if(tok == LTHAN || tok == GTHAN)
		{
			ParseError(line, "Illegal Relational Expression.");
			return false;
		}
		else if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of RelExpr

//AddExpr :: MultExpr { ( + | - ) MultExpr }
bool AddExpr(istream& in, int& line, Value & retVal) {
	Value val1, val2;
	bool t1 = MultExpr(in, line,val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS ) 
	{
		t1 = MultExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator PLUSMINUS");
			return false;
		}
		if (tok == PLUS){
			//evaluate expression for *
			retVal = retVal + val2;
			//cout << "AND op result " << retVal.IsBool() << " " << retVal.GetBool() << endl;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal PLUS operation.");
				//cout << "(" << tok.GetLexeme() << ")" << endl;		
				return false;
			}
		}
		else if (tok == MINUS){
			//evaluate expression for -
			retVal = retVal - val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal MINUS operation.");
				return false;
			}
		}
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr

//MultExpr ::= UnaryExpr { ( * | / ) UnaryExpr }
bool MultExpr(istream& in, int& line, Value & retVal) {
	Value val1, val2;
	bool t1 = UnaryExpr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  )
	{
		t1 = UnaryExpr(in, line, val2);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator MULTDIV");
			return false;
		}
		if (tok == MULT){
			//evaluate expression for *
			retVal = retVal * val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal MULT operation.");
				//cout << "(" << tok.GetLexeme() << ")" << endl;		
				return false;
			}
		}
		else if (tok == DIV){
			//evaluate expression for /
            if (val2.GetInt() == 0) {
                ParseError(line, "Run-Time Error-Illegal Division by Zero.");
				return false; 
            }
			retVal = retVal / val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal DIV operation.");
				return false;
			}
		}
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr

//UnaryExpr ::= ( - | + | ! ) PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream& in, int& line, Value & retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	Value val1;
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else
		Parser::PushBackToken(t);
		
	status = PrimaryExpr(in, line, sign, val1);
	retVal = val1;
    //cout << "status is " << status << endl;
	return status;
}//End of UnaryExpr



//PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | ( Expr )
bool PrimaryExpr(istream& in, int& line, int sign,Value & retVal) {
	Value val1;
    bool notVal = false;
	LexItem tok = Parser::GetNextToken(in, line);
    string lexeme = tok.GetLexeme();
    if (tok == NOT) {
        notVal = true;
        tok = Parser::GetNextToken(in, line);
        lexeme = tok.GetLexeme();
    }
	if( tok == IDENT ) {
        if (sign != 0) {
            cout << sign << endl;
            ParseError(line, "Illegal Operand Type for Sign Operator");
			return false;
        }
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undeclared Variable");
			return false;	
		}
        retVal = TempsResults[lexeme];
        //cout << "initial" << retVal << endl;
        if (notVal) 
        {
            retVal = !retVal;
        //cout << "notVal: " << retVal<< endl;
        }
        //cout << retVal;
		return true;
	}
	else if( tok == ICONST ) {
        //cast tok.GetLexeme into integer
        int valInt = stoi(lexeme);
		retVal = Value(valInt);
        //cout << retVal << endl;
		return true;
	}
	else if( tok == SCONST ) {
		retVal = Value(lexeme);
        //cout << retVal;
		return true;
	}
	else if( tok == RCONST ) {
		float valFloat = stof(lexeme);
		retVal = Value(valFloat);
        //cout << retVal << endl;
		return true;
	}
	else if( tok == BCONST ) {
        bool valBool;
        if (lexeme == "TRUE") {
            valBool = true;
        }
        else {
            valBool = false;   
        }
		retVal = Value(valBool);
        //if (notVal) {retVal = !retVal;}
        //cout << "retVal: " << retVal << endl;
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else 
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}



