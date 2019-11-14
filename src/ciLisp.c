#include "ciLisp.h"


void yyerror(char *s) {
    fprintf(stderr, "\nERROR: %s\n", s);
    // note stderr that normally defaults to stdout, but can be redirected: ./src 2> src.log
    // CLion will display stderr in a different color from stdin and stdout
}

// Array of string values for operations.
// Must be in sync with funcs in the OPER_TYPE enum in order for resolveFunc to work.
char *funcNames[] = {
        "neg",
        "abs",
        "exp",
        "sqrt",
        "add",
        "sub",
        "mult",
        "div",
        "remainder",
        "log",
        "pow",
        "max",
        "min",
        "exp2",
        "cbrt",
        "hypot",
        "read",
        "rand",
        "print",
        "equal",
        "less",
        "greater",
        ""
};

OPER_TYPE resolveFunc(char *funcName)
{
    int i = 0;
    while (funcNames[i][0] != '\0')
    {
        if (strcmp(funcNames[i], funcName) == 0)
            return i;
        i++;
    }
    return CUSTOM_OPER;
}

// Called when an INT or DOUBLE token is encountered (see ciLisp.l and ciLisp.y).
// Creates an AST_NODE for the number.
// Sets the AST_NODE's type to number.
// Populates the value of the contained NUMBER_AST_NODE with the argument value.
// SEE: AST_NODE, NUM_AST_NODE, AST_NODE_TYPE.
AST_NODE *createNumberNode(double value, NUM_TYPE type)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed size and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, assign values to contained NUM_AST_NODE
    node->type = NUM_NODE_TYPE;
    if(type == DOUBLE_TYPE)
    {
        node->data.number.type = type;
        node->data.number.dval = value;
    }else
    {
        node->data.number.type = type;
        node->data.number.dval = (long)value;
    }
    return node;
}

// Called when an f_expr is created (see ciLisp.y).
// Creates an AST_NODE for a function call.
// Sets the created AST_NODE's type to function.
// Populates the contained FUNC_AST_NODE with:
//      - An OPER_TYPE (the enum identifying the specific function being called)
//      - 2 AST_NODEs, the operands
// SEE: AST_NODE, FUNC_AST_NODE, AST_NODE_TYPE.
AST_NODE *createFunctionNode(char *funcName, AST_NODE *op1, AST_NODE *op2)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space (or error)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, populate contained FUNC_AST_NODE
    // NOTE: you do not need to populate the "ident" field unless the function is type CUSTOM_OPER.
    // When you do have a CUSTOM_OPER, you do NOT need to allocate and strcpy here.
    // The funcName will be a string identifier for which space should be allocated in the tokenizer.
    // For CUSTOM_OPER functions, you should simply assign the "ident" pointer to the passed in funcName.
    // For functions other than CUSTOM_OPER, you should free the funcName after you're assigned the OPER_TYPE.
    node->type = FUNC_NODE_TYPE;
    node->data.function.op1 = op1;
    node->data.function.op2 = op2;
    node->data.function.oper = resolveFunc(funcName);

    return node;
}

// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees the whole abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
void freeNode(AST_NODE *node)
{
    if (!node)
        return;

    if (node->type == FUNC_NODE_TYPE)
    {
        // Recursive calls to free child nodes
        freeNode(node->data.function.op1);
        freeNode(node->data.function.op2);

        // Free up identifier string if necessary
        if (node->data.function.oper == CUSTOM_OPER)
        {
            free(node->data.function.ident);
        }
    }

    free(node);
}

// Evaluates an AST_NODE.
// returns a RET_VAL storing the the resulting value and type.
// You'll need to update and expand eval (and the more specific eval functions below)
// as the project develops.
RET_VAL eval(AST_NODE *node)
{
    if (!node)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN}; // see NUM_AST_NODE, because RET_VAL is just an alternative name for it.

    // TODO complete the switch.
    // Make calls to other eval functions based on node type.
    // Use the results of those calls to populate result.
    switch (node->type)
    {
        case FUNC_NODE_TYPE:
            result = evalFuncNode(&node->data.function);
            break;
        case NUM_NODE_TYPE:
            result = evalNumNode(&node->data.number);
            break;
        case SYM_NODE_TYPE:
            result = evalSymNode(&node->data.symbol, node->data.symbol.ident);
            break;
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}  

// returns a pointer to the NUM_AST_NODE (aka RET_VAL) referenced by node.
// DOES NOT allocate space for a new RET_VAL.
RET_VAL evalNumNode(NUM_AST_NODE *numNode)
{
    if (!numNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    // TODO populate result with the values stored in the node.
    // SEE: AST_NODE, AST_NODE_TYPE, NUM_AST_NODE

    result.type = numNode->type;
    result.dval = numNode->dval;

    return result;
}


RET_VAL evalFuncNode(FUNC_AST_NODE *funcNode)
{
    if (!funcNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    // TODO populate result with the result of running the function on its operands.
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE

    RET_VAL op1 = eval(funcNode->op1);
    RET_VAL op2 = eval(funcNode->op2);

    double op1val = op1.dval;
    double op2val = op2.dval;

    if(op1.type == DOUBLE_TYPE || op2.type == DOUBLE_TYPE)
        result.type = DOUBLE_TYPE;

    switch (funcNode->oper)
    {

        case NEG_OPER:
            result.dval = -op1val;
            break;
        case ABS_OPER:
            result.dval = fabs(op1val);
            break;
        case EXP_OPER:
            result.dval = exp(op1val);
            break;
        case SQRT_OPER:
            result.dval = sqrt(op1val);
            break;
        case ADD_OPER:
            result.dval = op1val+op2val;
            break;
        case SUB_OPER:
            result.dval = op1val-op2val;
            break;
        case MULT_OPER:
            result.dval = op1val*op2val;
            break;
        case DIV_OPER:
            if(op2val == 0.0)
                printf("dividing by zero ");
            result.dval = op1val/op2val;
            break;
        case REMAINDER_OPER:
            result.dval = remainder(op1val, op2val);
            break;
        case LOG_OPER:
            result.dval = log(op1val);
            break;
        case POW_OPER:
            result.dval = pow(op1val, op2val);
            break;
        case MAX_OPER:
            result.dval = fmax(op1val, op2val);
            break;
        case MIN_OPER:
            result.dval = fmin(op1val, op2val);
            break;
        case EXP2_OPER:
            result.dval = exp2(op1val);
            break;
        case CBRT_OPER:
            result.dval = cbrt(op1val);
            break;
        case HYPOT_OPER:
            result.dval = hypot(op1val, op2val);
            break;
        case READ_OPER:break;
        case RAND_OPER:break;
        case PRINT_OPER:break;
        case EQUAL_OPER:break;
        case LESS_OPER:break;
        case GREATER_OPER:break;
        case CUSTOM_OPER:break;
    }

    return result;
}

RET_VAL evalSymNode(SYMBOL_TABLE_NODE* symNode, char* id)
{
    if (!symNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    SYMBOL_TABLE_NODE* curNode = symNode;
    while(curNode != NULL)
    {
        if(strcmp(curNode->ident, id) == 0)
            result.dval = curNode->val->data.number.dval;


        curNode = curNode->next;
    }



    return result;
}

//set the symbol table to the expr, the parent of all
//the values in the table is set to the expr

AST_NODE* setSymbolTable(SYMBOL_TABLE_NODE* symtable, AST_NODE* s_expr)
{
    s_expr->symbolTable = symtable;
    SYMBOL_TABLE_NODE* curNode = symtable;
    while(curNode != NULL)
    {
        curNode->val->parent = s_expr;
        curNode = curNode->next;
    }
    return s_expr;
}

//create an ast node of type symbol table

AST_NODE* symbolASTNode(char* id)
{
    AST_NODE* symbolAST = malloc(sizeof(AST_NODE));

    symbolAST->type = SYM_NODE_TYPE;
    symbolAST->data.symbol.ident = malloc(sizeof(char*) * strlen(id)+1);
    strcpy(symbolAST->data.symbol.ident, id);
    return symbolAST;
}

//create a symbol table node with the given id and value
SYMBOL_TABLE_NODE* createSymbolTableNode(char* id, AST_NODE* s_expr)
{
    SYMBOL_TABLE_NODE* symbolTableNode = malloc(sizeof(SYMBOL_TABLE_NODE));

    symbolTableNode->ident = id;
    symbolTableNode->val = s_expr;
    return symbolTableNode;
}

SYMBOL_TABLE_NODE* addSymbol(SYMBOL_TABLE_NODE* symTableList, SYMBOL_TABLE_NODE* symTableToAdd)
{
    while(symTableList->next != NULL)
        symTableList = symTableList->next;


    symTableList->next = symTableToAdd;
    symTableToAdd->next = NULL;
    return symTableList;
}

// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val)
{
    // TODO print the type and value of the value passed in.


    if(val.type == DOUBLE_TYPE)
        printf("%lf Double", val.dval);
    if(val.type == INT_TYPE)
        printf("%ld Int", (long)val.dval);
}