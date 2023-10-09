/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    char topChar; // temporary variable for charracter on top of stack
    while (!Stack_IsEmpty(stack)){
        Stack_Top(stack, &topChar); // get top char
        Stack_Pop(stack); // remove top char
        postfixExpression[(*postfixExpressionLength)++] = topChar; // add top char to postfix expression

        if(topChar == '('){ // if top char is left parenthesis, break
            Stack_Pop(stack);
            break;
        }

        Stack_Pop(stack);

    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
    char topChar; // temporary variable for charracter on top of stack
    if(!Stack_IsEmpty(stack)){
        Stack_Top(stack, &topChar);
    } else {
        Stack_Push(stack, c); // if stack is empty, push the operator on the top of stack
        return;
    }

    // variables for priority of operators
    int priorityC = 0;
    int priorityTop = 0;

    // set priority of operator c
    switch(c){
        case '+':
        case '-':
            priorityC = 1;
            break;
        case '*':
        case '/':
            priorityC = 2;
            break;
    }

    // set priority of operator on top of stack
    switch(topChar){
        case '+':
        case '-':
            priorityTop = 1;
            break;
        case '*':
        case '/':
            priorityTop = 2;
            break;
    }

    // if priority of operator c is higher than priority of operator on top of stack, push operator c on the top of stack
    while (!Stack_IsEmpty(stack) && (priorityC <= priorityTop) && topChar != '(') {
        Stack_Top(stack, &topChar);
        Stack_Pop(stack);
        postfixExpression[(*postfixExpressionLength)++] = topChar;
    }

    Stack_Push(stack, c);
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
    // Allocate memory for the postfix expression
    char *postfixExpression = malloc(MAX_LEN * sizeof(char));
    if (postfixExpression == NULL)
        return NULL;

    // Initialize a stack for operator processing
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        free(postfixExpression);
        return NULL;
    }

    Stack_Init(stack);
    unsigned postfixExpressionLength = 0;

    for (int i = 0; infixExpression[i] != '\0'; i++) {
        char c = infixExpression[i];

        switch (c) {
            // Handle the left parenthesis '('
            case '(':
                Stack_Push(stack, c);
                break;
                // Handle arithmetic operators: +, -, *, /
            case '+':
            case '-':
            case '*':
            case '/':
                doOperation(stack, c, postfixExpression, &postfixExpressionLength);
                break;
                // Handle the right parenthesis ')'
            case ')':
                untilLeftPar(stack, postfixExpression, &postfixExpressionLength);
                break;
                // Handle the assignment operator '='
            case '=':
                // Pop remaining operators and append '=' to postfix expression
                while (!Stack_IsEmpty(stack)) {
                    Stack_Top(stack, &postfixExpression[postfixExpressionLength++]);
                    Stack_Pop(stack);
                }
                postfixExpression[postfixExpressionLength++] = '=';
                break;
            default:
                // Handle operands (numbers and variables)
                if ((c >= '0' && c <= '9') ||
                    (c >= 'a' && c <= 'z') ||
                    (c >= 'A' && c <= 'Z')) {
                    postfixExpression[postfixExpressionLength++] = c;
                }
                break;
        }
    }

    // Null-terminate the postfix expression string
    postfixExpression[postfixExpressionLength] = '\0';

    // Clean up and free memory
    Stack_Dispose(stack);
    free(stack);

    return postfixExpression; // Return the postfix expression
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
    unsigned char byte;

    // Iterate through the four bytes of the integer value
    for (int i = 0; i < 4; i++) {
        // Extract each byte from the integer value and push it onto the stack
        byte = (unsigned char)((value >> i * 8) & 0xFF);
        Stack_Push(stack, byte);
    }
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
    *value = 0;
    char byte;
    int getValue;

    // Iterate through the four bytes of the integer value
    for (int i = 3; i >= 0; i--) {
        // Pop the top byte from the stack
        Stack_Top(stack, &byte);
        Stack_Pop(stack);

        // Convert the byte to an integer value and add it to the result
        getValue = (int)(byte & 0xFF);
        *value += getValue << (i * 8); // Shift and combine the bytes
    }
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
    // Convert the infix expression to postfix notation
    char *postfixExpression = infix2postfix(infixExpression);
    if (postfixExpression == NULL) {
        return false; // Memory allocation error
    }

    // Create a stack for intermediate calculations
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        free(postfixExpression);
        return false; // Memory allocation error
    }
    Stack_Init(stack);

    for (int i = 0; postfixExpression[i] != '\0'; i++) {
        char symbol = postfixExpression[i];
        int operand1, operand2, result;

        switch (symbol) {
            // Handle arithmetic operators: +, -, *, /
            case '+':
            case '-':
            case '*':
            case '/':
            {
                // Pop the top two values from the stack
                expr_value_pop(stack, &operand2);
                expr_value_pop(stack, &operand1);

                // Perform the operation based on the operator
                switch (symbol) {
                    case '+':
                        result = operand1 + operand2;
                        break;
                    case '-':
                        result = operand1 - operand2;
                        break;
                    case '*':
                        result = operand1 * operand2;
                        break;
                    case '/':
                        // Check for division by zero
                        if (operand2 != 0) {
                            result = operand1 / operand2;
                        } else {
                            // Division by zero error
                            free(stack);
                            free(postfixExpression);
                            return false;
                        }
                        break;
                }

                // Push the result back onto the stack
                expr_value_push(stack, result);
            }
                break;
                // Handle the assignment operator =
            case '=':
                // Pop the result from the stack and assign it to the value pointer
                expr_value_pop(stack, &result);
                *value = result;
                break;
                // Handle variables and constants
            default:
                // Search for the variable in the variableValues array
                for (int j = 0; j < variableValueCount; j++) {
                    if (symbol == variableValues[j].name) {
                        // Push the variable's value onto the stack
                        expr_value_push(stack, variableValues[j].value);
                        break;
                    }
                }
                break;
        }
    }

    // Clean up and free memory
    Stack_Dispose(stack);
    free(stack);
    free(postfixExpression);

    return true; // Evaluation successful
}

/* Konec c204.c */
