%{
    #include "ciLisp.h"
%}

%union {
    double dval;
    char *sval;
    int datatype;
    struct ast_node *astNode;
    struct symbol_table_node* symbolNode;
};

%token <sval> FUNC SYMBOL
%token <dval> INT DOUBLE
%token <datetype> INT_T DOUBLE_T
%token LET LPAREN RPAREN EOL QUIT COND LAMBDA

%type <astNode> s_expr f_expr number s_expr_list
%type <symbolNode> let_section let_list let_elem arg_list
%type <datatype> type
%%

program:
    s_expr EOL {
        fprintf(stderr, "yacc: program ::= s_expr EOL\n");
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
        }
    };

s_expr:
    number {
        fprintf(stderr, "yacc: s_expr ::= number\n");
        $$ = $1;
    }
    | SYMBOL {
        $$ = symbolASTNode($1);
    }
    | f_expr {
        $$ = $1;
    }
    | LPAREN let_section s_expr RPAREN {
        $$ = setSymbolTable($2, $3);
    }
    | LPAREN COND s_expr s_expr s_expr RPAREN {
        $$ = createCondNode($3, $4, $5);
    }
    | QUIT {
        fprintf(stderr, "yacc: s_expr ::= QUIT\n");
        exit(EXIT_SUCCESS);
    }
    | error {
        fprintf(stderr, "yacc: s_expr ::= error\n");
        yyerror("unexpected token");
        $$ = NULL;
    };

number:
    INT {
        fprintf(stderr, "yacc: number ::= INT\n");
        $$ = createNumberNode($1, INT_TYPE);
    }
    | DOUBLE {
        fprintf(stderr, "yacc: number ::= DOUBLE\n");
        $$ = createNumberNode($1, DOUBLE_TYPE);
    };



let_section:
    LPAREN let_list RPAREN {
        $$ = $2;
    };

let_list:
     LET let_elem {
        $$ = $2;
    }
    |let_list let_elem {
        $$ = addSymbol($1, $2);
    };

let_elem:
    LPAREN type SYMBOL s_expr RPAREN {
        $$ = createSymbolTableNode($2, $3, $4);
    }
    | LPAREN type SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN {
        $$ = createLambda($2 ,$3 ,$6 , $8);
    };

type:
    INT_T {
        $$ = INT_TYPE;
    }
    | DOUBLE_T {
        $$ = DOUBLE_TYPE;
    }
    | {
        $$ = NO_TYPE;
    };

f_expr:
    LPAREN FUNC s_expr_list RPAREN {
        fprintf(stderr, "yacc: s_expr ::= LPAREN FUNC expr RPAREN\n");
        $$ = createFunctionNode($2, $3);
    }
    | LPAREN FUNC RPAREN {
        fprintf(stderr, "yacc: s_expr ::= LPAREN FUNC RPAREN\n");
        $$ = createFunctionNode($2, NULL);
    }
    | LPAREN SYMBOL s_expr_list RPAREN {
        $$ = createFunctionNode($2, $3);
    };


s_expr_list:
    s_expr s_expr_list {
        $$ = addParaToList($1, $2);
    }
    | s_expr {
        $$ = $1;
    };

arg_list:
    SYMBOL arg_list {
        $$ = AddArgtoList($1, $2);
    }
    | SYMBOL {
        $$ = createArg($1);
    };
%%

