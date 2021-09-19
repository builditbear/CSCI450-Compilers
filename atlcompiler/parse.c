// Brandon Chavez, CSCI 450 - Fall 2018, Professor Nelson
/* parse.c:  This is where the parser should be built. */
// Rated "E", for "Even You!".
#define TRUE 1
#define FALSE 0
/* Get the standard definitions and includes. */
#include "defs.h"

/* External Variables. */
#include "global.h"

/* static "local" variables. */
token currentToken;
id_rec* symbolTable = NULL;
int stringStorageMade = FALSE;

// Functions for interacting with the symbol table.
void lookupSymbolA(void);
void lookupSymbolB(void);
void addSymbol(void);
/* Functions for the rules. */
void printToken(FILE* stream, token t);
void match(token t);
void goal(void);
void var_decl(void);
void id_list(void);
void statements(void);
void statement(void);
void read_list(void);
void write_list(void);
void write_elem(void);
void expr(void);
void add_op(void);
void primary(void);


void parse (void)
{
    currentToken = scanner();
    if(list_src)
    {
        while(currentToken != SCANEOF)
        {
            printToken(stdout, currentToken);
            currentToken = scanner();
            printf("\n");
        }
        printToken(stdout, currentToken);
    }else
    {
        goal();
        if(currentToken != SCANEOF)
        {
            fprintf(stderr, "Syntax Error: Expected an EOF token!\n");
        }else
        {
            fprintf(stdout, "Parsing successful! Gooooo Joooooe!\n");
        }
    }
}
//Prints the text value of a token based on enumeration value.
void printToken(FILE* stream, token t)
{
    switch(t)
    {
        case ID:
            fprintf(stream, "ID: %s", mostRecentToken);
            break;
        case STRING:
            fprintf(stream, "STRING: %s", mostRecentToken);
            break;
        case CONST:
            fprintf(stream, "CONST: %s", mostRecentToken);
            break;
        case END:
            fprintf(stream, "end");
            break;
        case READ:
            fprintf(stream, "read");
            break;
        case BEGIN:
            fprintf(stream, "begin");
            break;
        case WRITE:
            fprintf(stream, "write");
            break;
        case INTEGER:
            fprintf(stream, "integer");
            break;
        case PROGRAM:
            fprintf(stream, "program");
            break;
        case WRITELN:
            fprintf(stream, "writeln");
            break;
        case VARIABLE:
            fprintf(stream, "variable");
            break;
        case ASSIGN:
            fprintf(stream, "assign");
            break;
        case PLUS:
            fprintf(stream, "PLUS");
            break;
        case MINUS:
            fprintf(stream, "MINUS");
            break;
        case LPAREN:
            fprintf(stream, "LEFT PARENTHESIS");
            break;
        case RPAREN:
            fprintf(stream, "RIGHT PARENTHESIS");
            break;
        case SEMI:
            fprintf(stream, "SEMICOLON");
            break;
        case COMMA:
            fprintf(stream, "COMMA");
            break;
        case COLON:
            fprintf(stream, "COLON");
            break;
        case PERIOD:
            fprintf(stream, "PERIOD");
            break;
        case SCANEOF:
            fprintf(stream, "END OF FILE.");
            break;
    }
}
//Determines if a symbol already exists in the Symbol Table, alerting the user if a double declaration has occurred,
//and simply adding the symbol's ID to the table otherwise.
void lookupSymbolA(void)
{
    id_rec* temp;
    //The mostRecentToken is assumed to be an ID at the time that lookupSymbol is called.
    temp = find_id(symbolTable, mostRecentToken);
    if(temp == NULL)
    {
        addSymbol();
    }else if(!(strcmp(temp->id, symbolTable->id))) //It is assumed here that the root of the tree is always going to be the program ID.
    {
        fprintf(stderr, "Semantic Error: Declaration of variable using program ID, %s.\n", symbolTable->id);
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stderr, "Semantic Error: Double declaration of variable ID, %s.\n", mostRecentToken);
        exit(EXIT_FAILURE);
    }
}
//Alternate lookup function that prevents using an undeclared variable or the program ID inappropriately.
void lookupSymbolB(void)
{
    id_rec* temp;
    temp = find_id(symbolTable, mostRecentToken);
    if(temp == NULL || (!strcmp(temp->id, symbolTable->id)))
    {
        fprintf(stderr, "Semantic Error: The variable you are trying to assign a value to is not initialized, "
                        "or you are attempting to assign to the program ID.\n");
        exit(EXIT_FAILURE);
    }
}
void addSymbol(void)
{
    id_rec* temp = (id_rec*) malloc(sizeof(id_rec));
    temp->id = strdup(mostRecentToken);
    // Set fields here if you need them! Can't think of anything we'd need for now, since ATL0 only has integer type variables.
    insert_id(&symbolTable, temp);
}
//Small helper function which simply verifies that a given token matches another
//desired or expected token.
void match(token t)
{
    if(currentToken == t)
    {
        currentToken = scanner();
    }else
    {
        fprintf(stderr, "Syntax Error: Expected token, ");
        printToken(stderr, t);
        fprintf(stderr, ", but received: ");
        printToken(stderr, currentToken);
        fprintf(stderr, ".\n");
        exit(EXIT_FAILURE);
    }
}
//Non-Terminal Processing Functions.
//goal is the "start" symbol, from which further rule procedures are applied.
void goal(void)
{
    char firstID[50];
    char secondID[50];

    match(PROGRAM);
    //Add the program ID to the Symbol Table.
    addSymbol();
    //Semantic check to ensure the ID's at the beginning and end of the program match.
    strcpy(firstID, mostRecentToken);
    match(ID);
    match(SEMI);
    var_decl();
    match(BEGIN);
    statements();
    match(END);
    strcpy(secondID, mostRecentToken);
    match(ID);
    if(strcmp(firstID, secondID))
    {
        fprintf(stderr, "Semantic Error: program ID's at the beginning and end do not match.\n");
        exit(EXIT_FAILURE);
    }
    match(PERIOD);
    //Stops the hc upon finishing the program.
    generate("halt");
}
void var_decl(void)
{
    match(VARIABLE);
    id_list();
    match(COLON);
    match(INTEGER);
    match(SEMI);
}
void id_list(void)
{
    lookupSymbolA();
    //Generate assembly instructions for variable declaration.
    label("%s", mostRecentToken);
    generate(".bss 1");
    match(ID);
    while(currentToken == COMMA)
    {
        match(COMMA);
        lookupSymbolA();
        label("%s", mostRecentToken);
        generate(".bss 1");
        match(ID);
    }
}
void statements(void)
{
    statement();
    match(SEMI);
    while(currentToken == ID || currentToken == READ ||
       currentToken == WRITE || currentToken == WRITELN)
    {
        statement();
        match(SEMI);
    }
}
void statement(void)
{
    switch(currentToken)
    {
        case ID: {
            lookupSymbolB();
            //Store ID of the variable we're about to assign to.
            char varID[strlen(mostRecentToken) + 1];
            strcpy(varID, mostRecentToken);
            match(ID);
            match(ASSIGN);
            expr();
            //Assign value to the variable stored.
            generate("stor 0,%s", varID);
            break;
        }
        case READ: {
            match(READ);
            match(LPAREN);
            read_list();
            match(RPAREN);
            break;
        }
        case WRITE: {
            match(WRITE);
            match(LPAREN);
            write_list();
            match(RPAREN);
            break;
        }
        case WRITELN: {
            match(WRITELN);
            match(LPAREN);
            match(RPAREN);
            generate("csp wln");
            break;
        }
        default: {
            fprintf(stderr, "Invalid token for beginning of a statement.\n");
            exit(EXIT_FAILURE);
        }
    }
}
void read_list(void)
{
    lookupSymbolB();
    generate("ladr 0,%s", mostRecentToken);
    generate("csp rdi");
    match(ID);
    while(currentToken == SEMI)
    {
        match(SEMI);
        lookupSymbolB();
        generate("ladr 0,%s", mostRecentToken);
        generate("csp rdi");
        match(ID);
    }
}
void write_list(void)
{
    write_elem();
    while(currentToken == SEMI)
    {
        match(SEMI);
        write_elem();
    }
}
void write_elem(void)
{
    if(currentToken == STRING)
    {
        if(!stringStorageMade)
        {
            label("tmp$stor");
            generate(".bss %d", 100);
            stringStorageMade = TRUE;
        }
        generate("ladr 0,tmp$stor");
        generate("movs %s", mostRecentToken);
        generate("pint 0");
        generate("csp wrs");
        match(STRING);
    }else
    {
        expr();
        generate("pint 0");
        generate("csp wri");
    }
}
//This effectively combines the derivations for expr and add_op into one expr rule.
void expr(void)
{
    primary();
    while(currentToken == PLUS || currentToken == MINUS)
    {
        char op[4];
        if(currentToken == PLUS)
        {
            strcpy(op, "add");
        }else
        {
            strcpy(op, "sub");
        }
        match(currentToken);
        primary();
        //Add or subtract depending on what operation we found earlier.
        generate("%s", op);
    }
    //generate("csp wri");
}
void primary(void)
{
    switch(currentToken)
    {
        case LPAREN:
            match(LPAREN);
            expr();
            match(RPAREN);
            break;
        case ID:
            lookupSymbolB();
            //Load variable onto stack.
            generate("load 0,%s", mostRecentToken);
            match(ID);
            break;
        case CONST:
            //Load integer constant onto stack.
            generate("pint %s", mostRecentToken);
            match(CONST);
            break;
        default:
            fprintf(stderr, "Invalid token for beginning of a primary.\n");
            exit(EXIT_FAILURE);

    }
}
