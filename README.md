PA 1 Submission: Building a Lexical Analyzer 

In this programming assignment, you will be building a lexical analyzer for small programming language and a program to test it. This assignment will be followed by two other assignments to build a parser and interpreter to the same language. Although, we are not concerned about the syntax definitions of the language in this assignment, we intend to introduce it ahead of Programming Assignment 2 in order to show the language reserved words, constants, and operators. The syntax definitions of the small programming language are given below using EBNF notations. However, the details of the meanings (i.e. semantics) of the language constructs will be given later on.

Prog ::= PROGRAM IDENT StmtList END PROGRAM
StmtList ::= Stmt; { Stmt; }
Stmt ::= DeclStmt | ControlStmt
DeclStmt ::= ( INT | FLOAT | BOOL ) VarList
VarList ::= Var { ,Var }
ControlStmt ::= AssigStmt | IfStmt | PrintStmt
PrintStmt ::= PRINT (ExprList)
IfStmt ::= IF (Expr) THEN StmtList { ELSE StmtList } END IF
AssignStmt ::= Var = Expr
Var ::= IDENT
ExprList ::= Expr { , Expr }
Expr ::= LogORExpr ::= LogANDExpr { || LogANDRxpr }
LogANDExpr ::= EqualExpr { && EqualExpr }
EqualExpr ::= RelExpr [ == RelExpr ]
RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
AddExpr :: MultExpr { ( + | - ) MultExpr }
MultExpr ::= UnaryExpr { ( * | / ) UnaryExpr }
UnaryExpr ::= ( - | + | ! ) PrimaryExpr | PrimaryExpr
PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | (Expr)

PA 2 Submission: Building a Parser 

In this programming assignment, you will be building a parser for a simple programming language. The syntax definitions of the small programming language are given below using EBNF notations. Your implementation of a parser to the language is based on the following grammar rules specified in EBNF notations.

Prog ::= PROGRAM IDENT StmtList END PROGRAM
StmtList ::= Stmt; { Stmt; }
Stmt ::= DeclStmt | ControlStmt
DeclStmt ::= ( INT | FLOAT | BOOL ) VarList
VarList ::= Var { ,Var }
ControlStmt ::= AssigStmt | IfStmt | PrintStmt
PrintStmt ::= PRINT (ExprList)
IfStmt ::= IF (Expr) THEN StmtList { ELSE StmtList } END IF
AssignStmt ::= Var = Expr
Var ::= IDENT
ExprList ::= Expr { , Expr }
Expr ::= LogORExpr ::= LogANDExpr { || LogANDRxpr }
LogANDExpr ::= EqualExpr { && EqualExpr }
EqualExpr ::= RelExpr [== RelExpr ]
RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
AddExpr :: MultExpr { ( + | - ) MultExpr }
MultExpr ::= UnaryExpr { ( * | / ) UnaryExpr }
UnaryExpr ::= ( - | + | ! ) PrimaryExpr | PrimaryExpr
PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | ( Expr )
The following points describe the programming language. Note that not all of these points will be addressed in this assignment. However, they are listed in order to give you an understanding of the language semantics and what to be considered for implementing an interpreter for the language in Programming Assignment 3. These points are:

Table of Operators Precedence Levels

Precedence

Operator

Description

Associativity

1

Unary +, -, and !

Unary plus, minus, and logical NOT

Right-to-Left

(ignored)

2

*, /

Multiplication and Division

Left-to-Right

 

3

+, -

Addition and Subtraction

Left-to-Right

4

<, >

Relational operators < and >

(no cascading)

5

==

Equality operator

(no cascading)

6

&&

Logical AND

Left-to-Right

7

||

Logical OR

Left-to-Right

 

The language has three types: INT, FLOAT and BOOL.
The precedence rules of operators in the language are as shown in the table of operators precedence levels.
The PLUS, MINUS, MULT, DIV, AND, OR operators are left associative.
A variable has to be declared in a declaration statement.
An IfStmt evaluates a logical expression (Expr) as a condition. If the logical condition value is true, then the StmtList in the Then-part are executed, otherwise they are not. An else part for an IfSmt is optional. Therefore, If an Else-part is defined, the StmtList in the Else-part are executed when the logical condition value is false.
A PrintStmt evaluates the list of expressions (ExprList), and prints their values in order from left to right followed by a newline.
The ASSOP operator (=) in the AssignStmt assigns a value to a variable. It evaluates the Expr on the right-hand side and saves its value in a memory location associated with the left-hand side variable (Var). A left-hand side variable of a numeric type can be assigned a value of either one of the numeric types (i.e., INT, FLOAT) of the language. For example, an integer variable can be assigned a real value, and a real variable can be assigned an integer value. In either case, conversion of the value to the type of the variable must be applied. A BOOL var in the left-hand side of an assignment statement must be assigned a Boolean value.
The binary operations for addition, subtraction, multiplication, and division are performed upon two numeric operands (i.e., INT, FLOAT) of the same or different types. If the operands are of the same type, the type of the result is the same type as the operator’s operands. Otherwise, the type of the result is REAL. The binary logic operations for the AND and OR are applied on two Boolean operands.
The LTHAN and GTHAN relational operators and the EQUAL operator operate upon two operands of compatible types. The evaluation of a relational expression, based on LTHAN or GTHAN operators, or an Equality expression, based on the Equal operator, produce either a true or false value.
It is an error to use a variable in an expression before it has been assigned.
The unary sign operators (+ or -) are applied upon unary numeric operands (i.e., INTEGER, REAL). While the unary NOT operator is applied upon a boolean operand (i.e., BOOL).
Parser Requirements:

Implement a recursive-descent parser for the given language. You may use the lexical analyzer you wrote for Programming Assignment 1, OR you may use the provided implementation when it is posted. The parser should provide the following:

The results of an unsuccessful parsing are a set of error messages printed by the parser functions, as well as the error messages that might be detected by the lexical analyzer.
If the parser fails, the program should stop after the parser function returns.
The assignment does not specify the exact error messages that should be printed out by the parser; however, the format of the messages should be the line number, followed by a colon and a space, followed by some descriptive text. Suggested messages might include "Missing semicolon at end of Statement.", "Incorrect Declaration Statement.", “Missing Right Parenthesis”, “Undefined Variable”, “Missing END”, etc.

Interpreter Requirements:

Implement an interpreter for the language based on the recursive-descent parser developed in Programming Assignment 2. You need to modify the parser functions to include the required actions of the interpreter for evaluating expressions, determining the type of expression values, executing the statements, and checking run-time errors. 

Provided Files

You are given the following files for the process of building an interpretation. These are “lex.h’, “lex.cpp”, “val.h”, “parseInt.h”, and “parseInt.cpp” with definitions and partial implementations of some functions. You need to complete the implementation of the interpreter in the provided copy of “parseInt.cpp. “parse.cpp” will be posted later on.
