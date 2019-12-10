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
        node->data.number.dval = value;
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
AST_NODE *createFunctionNode(char *funcName, AST_NODE *op_list)
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
    node->data.function.opList = op_list;
    node->data.function.oper = resolveFunc(funcName);

    if(node->data.function.oper == CUSTOM_OPER)
        node->data.function.ident = funcName;

    if(node->data.function.oper == READ_OPER)
    {
        printf("read:= ");
        scanf("%lf", &node->data.number.dval);
        while((getchar()) != '\n');
        node->type = NUM_NODE_TYPE;
        return node;
    }
    else if(node->data.function.oper == RAND_OPER)
    {
        node->data.number.dval = (double)rand()/RAND_MAX;
        node->type = NUM_NODE_TYPE;
    }
    if(op_list != NULL)
    {
        AST_NODE* listTemp = op_list;
        while(listTemp != NULL)
        {
            listTemp->parent = node;
            listTemp = listTemp->next;
        }
    }



    return node;
}



AST_NODE* createCondNode(AST_NODE* cond, AST_NODE* condTrue, AST_NODE* condFalse)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space (or error)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    node->type = COND_NODE_TYPE;
    node->data.condition.cond = cond;
    node->data.condition.trueExpr = condTrue;
    node->data.condition.falseExpr = condFalse;
    cond->parent = node;
    condTrue->parent = node;
    condFalse->parent = node;
    return node;
}

void freeSymbolTable(SYMBOL_TABLE_NODE* node)
{
    if(node == NULL)
        return;

    freeSymbolTable(node->next);
    free(node->ident);
    freeNode(node->val);
    free(node);

}

// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees the whole abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
void freeNode(AST_NODE *node)
{
    if (!node)
        return;

    free(node->next);

    if (node->type == FUNC_NODE_TYPE)
    {
        // Recursive calls to free child nodes
        free(node->data.function.ident);
        freeNode(node->data.function.opList);

        // Free up identifier string if necessary
        if (node->data.function.oper == CUSTOM_OPER)
        {
            //free(node->data.function.ident);
            //freeNode(node->data.)

        }
    }

    else if(node->type == COND_NODE_TYPE)
    {
        freeNode(node->data.condition.cond);
        freeNode(node->data.condition.falseExpr);
        freeNode(node->data.condition.trueExpr);
    }
    else if(node->type == SYM_NODE_TYPE)
    {
        free(node->data.symbol.ident);
    }

    //freeSymbolTable(node->symbolTable);

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
            result = evalFuncNode(node);
            break;
        case NUM_NODE_TYPE:
            result = evalNumNode(&node->data.number);
            break;
        case SYM_NODE_TYPE:
            result = evalSymNode(node);
            break;
        case COND_NODE_TYPE:
            result = evalCondNode(node);
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


void pushArgsToStack(AST_NODE* lambda, AST_NODE* list)
{
    AST_NODE* currOp = list;
    SYMBOL_TABLE_NODE* currArg = lambda->symbolTable;
    while (currArg != NULL && currOp != NULL)
    {
        if (currArg->type == ARG_TYPE)
        {
            STACK_NODE* argPushed = calloc(sizeof(STACK_NODE),1);
            argPushed->next = currArg->stack;
            argPushed->val = currOp;
            currArg->stack = argPushed;

            currOp = currOp->next;
        }
        currArg = currArg->next;
    }
}

void popArgsFromStack(AST_NODE* lambda)
{
    SYMBOL_TABLE_NODE* currArg = lambda->symbolTable;
    while(currArg != NULL)
    {
        if(currArg->type != ARG_TYPE)
        {
            STACK_NODE* newTopOfStack = currArg->stack->next;
            freeNode(currArg->stack->val);
            free(currArg->stack);
            currArg->stack = newTopOfStack;

        }
        currArg = currArg->next;
    }
}

RET_VAL evalLambda(AST_NODE* node)
{
    RET_VAL resultEvalLambda = {INT_TYPE, NAN};
    AST_NODE* parent = node;
    SYMBOL_TABLE_NODE* currLambda;
    while (parent != NULL)
    {
        currLambda = parent->symbolTable;
        while(currLambda != NULL)
        {
            if(strcmp(currLambda->ident, node->data.function.ident)== 0 && currLambda->type == LAMBDA_TYPE)
            {
                pushArgsToStack(currLambda->val, node->data.function.opList);
                resultEvalLambda = eval(currLambda->val);
                popArgsFromStack(currLambda->val);
                return resultEvalLambda;
            }
            currLambda = currLambda->next;
        }
        parent = parent->parent;
    }
    return resultEvalLambda;
}

RET_VAL evalFuncNode(AST_NODE *Node)
{
    if (!Node)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    // TODO populate result with the result of running the function on its operands.
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE

    RET_VAL op1;
    RET_VAL op2;

    if(Node->data.function.opList != NULL && Node->data.function.oper != PRINT_OPER)
    {
        op1 = eval(Node->data.function.opList);
        op2 = eval(Node->data.function.opList->next);
        if(op1.type == DOUBLE_TYPE || op2.type == DOUBLE_TYPE)
            result.type = DOUBLE_TYPE;
    }

    double op1val = op1.dval;
    double op2val = op2.dval;

    switch (Node->data.function.oper)
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
        case MULT_OPER:
        case PRINT_OPER:
            result = multi_para_func(Node->data.function.opList, Node->data.function.oper);
            break;
        case SUB_OPER:
            result.dval = op1val-op2val;
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
        case EQUAL_OPER:
            result.type = INT_TYPE;
            if(op1val == op2val)
                result.dval = 1;
            else
                result.dval = 0;
            break;
        case LESS_OPER:
            result.type = INT_TYPE;
            if(op1val < op2val)
                result.dval = 1;
            else
                result.dval = 0;
            break;
        case GREATER_OPER:
            result.type = INT_TYPE;
            if(op1val > op2val)
                result.dval = 1;
            else
                result.dval = 0;
            break;
        case CUSTOM_OPER:
            result = evalLambda(Node);
            break;
    }

    return result;
}

RET_VAL evalArg(SYMBOL_TABLE_NODE *arg) {
    RET_VAL result = (RET_VAL) {INT_TYPE, NAN};
    if(arg->stack != NULL){
        result = eval(arg->stack->val);
    }
    return result;
}

RET_VAL evalSymNode(AST_NODE* node)
{
    if (!node)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL resultEvalSym = {INT_TYPE, NAN};
    AST_NODE* parent = node->parent;
    SYMBOL_TABLE_NODE* curNode;
    while(parent != NULL)
    {
        curNode = parent->symbolTable;
        while (curNode != NULL) {
            if (strcmp(curNode->ident, node->data.symbol.ident) == 0) {
                if(curNode->type == VARIABLE_TYPE) {
                    if(curNode->val_type != NO_TYPE)
                    {
                        resultEvalSym = eval(curNode->val);
                        resultEvalSym.type = curNode->val_type;
                        return resultEvalSym;
                    }
                    if(curNode->val_type == NO_TYPE)
                    {
                        curNode->val->data.number.type = checkType(curNode->val->data.number.dval);
                        curNode->val_type = curNode->val->data.number.type;
                        resultEvalSym = eval(curNode->val);
                        resultEvalSym.type = curNode->val_type;
                        return resultEvalSym;
                    }
                    else{
                        if(curNode->val->data.number.type == DOUBLE_TYPE && curNode->val_type == INT_TYPE)
                            printf("WARNING: precision loss in the assignment for variable %s \n", curNode->ident);

                        curNode->val->data.number.type = curNode->val_type;
                    }
                    curNode->val->data.number.type = curNode->val_type;

                    return eval(curNode->val);
                }
                else if(curNode->type == ARG_TYPE)
                {
                    resultEvalSym = evalArg(curNode);
                    return resultEvalSym;
                }
            }
            curNode = curNode->next;
        }

        parent = parent->parent;
    }
    printf("Variable %s not found\n", node->data.symbol.ident);
    return resultEvalSym;
}

RET_VAL evalCondNode(AST_NODE* cond)
{

    RET_VAL condResult = eval(cond->data.condition.cond);
    if(condResult.dval == 1)
        return eval(cond->data.condition.trueExpr);

    return eval(cond->data.condition.falseExpr);
}

//set the symbol table to the expr, the parent of all
//the values in the table is set to the expr

AST_NODE* setSymbolTable(SYMBOL_TABLE_NODE* symtable, AST_NODE* s_expr)
{
    if(s_expr == NULL)
        return NULL;

    SYMBOL_TABLE_NODE* curNode = symtable;
    while(curNode != NULL)
    {
        curNode->val->parent = s_expr;
        curNode = curNode->next;
    }
    s_expr->symbolTable = symtable;
    return s_expr;
}

//create an ast node of type symbol table

AST_NODE* symbolASTNode(char* id)
{
    AST_NODE* symbolAST = malloc(sizeof(AST_NODE));

    symbolAST->type = SYM_NODE_TYPE;
    symbolAST->data.symbol.ident = id;
    return symbolAST;
}

//create a symbol table node with the given id and value
SYMBOL_TABLE_NODE* createSymbolTableNode(int type, char* id, AST_NODE* s_expr)
{
    SYMBOL_TABLE_NODE* symbolTableNode = malloc(sizeof(SYMBOL_TABLE_NODE));

    symbolTableNode->val_type = type;
    symbolTableNode->ident = id;
    symbolTableNode->val = s_expr;
    symbolTableNode->next = NULL;
    symbolTableNode->type = VARIABLE_TYPE;
    return symbolTableNode;
}

SYMBOL_TABLE_NODE *findSymbol(SYMBOL_TABLE_NODE *symbolTable, SYMBOL_TABLE_NODE *symbol) {
    if (symbol == NULL) {
        return NULL;
    }
    SYMBOL_TABLE_NODE *cNode = symbolTable;
    while (cNode != NULL) {
        if (strcmp(cNode->ident, symbol->ident) == 0) {
            return cNode;
        }
        cNode = cNode->next;
    }
    return NULL;
}

SYMBOL_TABLE_NODE* addSymbol(SYMBOL_TABLE_NODE* let_elem, SYMBOL_TABLE_NODE* let_list)
{
//    SYMBOL_TABLE_NODE* temp = symTableList;
//    while(temp->next != NULL)
//        temp = temp->next;
//
//
//    temp->next = symTableToAdd;
//    symTableToAdd->next = NULL;
//    return symTableList;

    if (let_elem == NULL) {
        return let_list;
    }
    if (let_elem->val == NULL) {
        if(let_elem->type == VARIABLE_TYPE) {
            return let_list;
        } else {
            if(let_elem->next == NULL) {
                let_elem->next = let_list;
            } else {
                addSymbol(let_list, let_elem->next);
            }
            return let_elem;
        }
    }
    SYMBOL_TABLE_NODE *symbol = findSymbol(let_list, let_elem);
    if (symbol == NULL) {
        let_elem->next = let_list;
        return let_elem;
    } else {
        symbol->val = let_elem->val;
        symbol->val->data = let_elem->val->data;
        //freeSymbolTable(let_elem);
        return let_list;
    }
}

AST_NODE* addParaToList(AST_NODE* s_expr, AST_NODE* s_exprList)
{
    if(s_exprList == NULL)
        return s_expr;

    s_expr->next = s_exprList;
    return s_expr;
}

RET_VAL multi_para_func(AST_NODE* opList, OPER_TYPE operType)
{
    RET_VAL result = {INT_TYPE, 0.0};
    double temp1 = 0.0;


    switch(operType)
    {
        case ADD_OPER:
            while(opList != NULL)
            {
                temp1 = eval(opList).dval;
                if(checkType(temp1) == DOUBLE_TYPE)
                    result.type = DOUBLE_TYPE;
                result.dval = result.dval + temp1;
                opList = opList->next;

                if(opList == NULL) break;

                temp1 = eval(opList).dval;
                if(checkType(temp1) == DOUBLE_TYPE)
                    result.type = DOUBLE_TYPE;
                result.dval = result.dval + temp1;
                opList = opList->next;
            }
            break;
        case MULT_OPER:
            result.dval = 1.0;
            while(opList != NULL)
            {
                temp1 = eval(opList).dval;
                if(checkType(temp1) == DOUBLE_TYPE)
                    result.type = DOUBLE_TYPE;
                result.dval = result.dval * temp1;
                opList = opList->next;

                if(opList == NULL) break;

                temp1 = eval(opList).dval;
                if(checkType(temp1) == DOUBLE_TYPE)
                    result.type = DOUBLE_TYPE;
                result.dval = result.dval * temp1;
                opList = opList->next;
            }
            break;
        case PRINT_OPER:
            while (opList!=NULL)
            {
                result = eval(opList);
                if(result.type == DOUBLE_TYPE)
                    printf("%lf ", result.dval);
                else
                    printf("%d ", (int)result.dval);
                opList = opList->next;
            }
            printf("\n");
            break;
    }

    return result;

}

NUM_TYPE checkType(double num)
{
    int i = (int) num;
    if(num - i == 0)    return INT_TYPE;

    return DOUBLE_TYPE;
}

SYMBOL_TABLE_NODE* createLambda(int type, char* id, SYMBOL_TABLE_NODE* argList, AST_NODE* s_expr)
{
    SYMBOL_TABLE_NODE* LambdaNode = calloc(sizeof(SYMBOL_TABLE_NODE), 1);

    LambdaNode->val_type = type;
    LambdaNode->type = LAMBDA_TYPE;
    LambdaNode->ident = id;
    LambdaNode->val = s_expr;
    s_expr->symbolTable = addSymbol(s_expr->symbolTable, argList);

    return LambdaNode;
}

SYMBOL_TABLE_NODE* createArg(char* arg)
{
    SYMBOL_TABLE_NODE* newArg = calloc(sizeof(SYMBOL_TABLE_NODE), 1);

    newArg->ident = arg;
    newArg->type = ARG_TYPE;

    return newArg;
}

SYMBOL_TABLE_NODE* AddArgtoList(char* argToAdd, SYMBOL_TABLE_NODE* argList)
{
    if(argList == NULL)
        return createArg(argToAdd);

    SYMBOL_TABLE_NODE* arg = argList;
    while(arg != NULL)
    {
        if(strcmp(argToAdd, arg->ident) == 0)
            return argList;

        arg = arg->next;
    }

    arg = createArg(argToAdd);
    arg->next = argList;
    return arg;
}


// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val)
{
    // TODO print the type and value of the value passed in.


    if(val.type == DOUBLE_TYPE)
        printf("%.3lf Double", val.dval);
    if(val.type == INT_TYPE)
        printf("%ld Int", (long)val.dval);
}
