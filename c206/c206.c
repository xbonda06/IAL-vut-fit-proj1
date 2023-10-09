/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    // set all pointers to NULL
	list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    list->activeElement = list->firstElement;
    while(list->activeElement != NULL){
        DLLElementPtr tmp = list->activeElement; // temporary pointer to active element
        list->activeElement = list->activeElement->nextElement; // move active element to next element
        free(tmp); // free memory of temporary pointer
    }

    DLL_Init(list);
}



/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // allocate memory for new element
    if(newElement == NULL){ // check if memory was allocated
        DLL_Error();
        return;
    }

    // set new element values
    newElement->data = data;
    newElement->nextElement = NULL;
    newElement->previousElement = NULL;

    // if list is empty set new element as first element
    if(list->firstElement == NULL){
        list->firstElement = newElement;
        list->lastElement = newElement;
    } else {
        // if list is not empty set new element as first element and set pointers to other elements
        newElement->nextElement = list->firstElement;
        list->firstElement->previousElement = newElement;
        list->firstElement = newElement;
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    // allocate memory for new elemen
    DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement)); t
    // check if memory was allocated
    if (newElement == NULL) {
        DLL_Error();
        return;
    }

    // set new element values
    newElement->data = data;
    newElement->nextElement = NULL;
    newElement->previousElement = NULL;

    // if list is empty set new element as last element
    if (list->lastElement == NULL) {
        list->firstElement = newElement;
        list->lastElement = newElement;
    } else {
        // if list is not empty set new element as last element and set pointers to other elements
        newElement->previousElement = list->lastElement;
        list->lastElement->nextElement = newElement;
        list->lastElement = newElement;
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement; // set active element to first element
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement; // set active element to last element
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
    // check if list is empty
    if(list->firstElement == NULL){
        DLL_Error();
        return;
    } else {
        // return data of first element via dataPtr
        *dataPtr = list->firstElement->data;
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
    // check if list is empty
    if(list->lastElement == NULL){
        DLL_Error();
        return;
    } else {
        // return data of last element via dataPtr
        *dataPtr = list->lastElement->data;
    }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    // check if the first element is active and cancel activity if it is
    if(list->activeElement == list->firstElement) list->activeElement = NULL;

    if(list->firstElement != NULL){
        DLLElementPtr temp = list->firstElement; // create temporary pointer to first element
        if(list->firstElement == list->lastElement){
            list->lastElement = NULL; // if there is only one element in list set last element to NULL
        } else {
            list->firstElement->nextElement->previousElement = NULL; // delete pointer to first element from second element
        }
        list->firstElement = list->firstElement->nextElement; // set first element to second element
        free(temp); // free memory of deleted element
    }

}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    // check if the last element is active and cancel activity if it is
    if(list->activeElement == list->lastElement) list->activeElement = NULL;

    if(list->lastElement != NULL){
        DLLElementPtr temp = list->lastElement; // create temporary pointer to last element
        if(list->firstElement == list->lastElement){
            list->firstElement = NULL; // if there is only one element in list set first element to NULL
        } else {
            list->lastElement->previousElement->nextElement = NULL; // delete pointer to last element from second to last element
        }
        list->lastElement = list->lastElement->previousElement; // set last element to second to last element
        free(temp); // free memory of deleted element
    }

}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    // check if list is active
	if(list->activeElement != NULL){
        // check if active element is not last element
        if(list->activeElement->nextElement != NULL){
            // create temporary pointer to element after active element
            DLLElementPtr temp = list->activeElement->nextElement;

            // set pointer to element after active element to element after element after active element
            list->activeElement->nextElement = temp->nextElement;
            if(temp == list->lastElement){
                // if element after active element is last element set last element to active element
                list->lastElement = list->activeElement;
            } else {
                // if element after active element is not last element
                // set pointer to previous element of element after element after active element to active element
                temp->nextElement->previousElement = list->activeElement;
            }
            free(temp); // free memory of deleted element
        }
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    // check if list is active
    if(list->activeElement != NULL){
        // check if active element is not first element
        if(list->activeElement->previousElement != NULL){
            // create temporary pointer to element before active element
            DLLElementPtr temp = list->activeElement->previousElement;

            // set pointer to element before active element to element before element before active element
            list->activeElement->previousElement = temp->previousElement;
            if(temp == list->firstElement){
                // if element before active element is first element set first element to active element
                list->firstElement = list->activeElement;
            } else {
                // if element before active element is not first element
                temp->previousElement->nextElement = list->activeElement;
            }
            free(temp); // free memory of deleted element
        }
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	if(list->activeElement != NULL){
        DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // create new element
        if(newElement == NULL){ // check if malloc was successful
            DLL_Error();
            return;
        }

        // set new element
        newElement->data = data;
        newElement->nextElement = list->activeElement->nextElement;
        newElement->previousElement = list->activeElement;

        if(list->activeElement == list->lastElement){
            // if active element is last element set last element to new element
            list->lastElement = newElement;
        } else {
            // if active element is not last element
            // set pointer to previous element of element after active element to new element
            list->activeElement->nextElement->previousElement = newElement;
        }
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
    if(list->activeElement != NULL){
        // create new element
        DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));
        if(newElement == NULL){ // check if malloc was successful
            DLL_Error();
            return;
        }

        // set new element
        newElement->data = data;
        newElement->nextElement = list->activeElement;
        newElement->previousElement = list->activeElement->previousElement;

        if(list->activeElement == list->firstElement){
            // if active element is first element set first element to new element
            list->firstElement = newElement;
        } else {
            // if active element is not first element
            // set pointer to next element of element before active element to new element
            list->activeElement->previousElement->nextElement = newElement;
        }
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if(list->activeElement == NULL){ // check if list is active
        DLL_Error();
        return;
    } else {
        *dataPtr = list->activeElement->data; // return value of active element to dataPtr
    }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if(list->activeElement != NULL){
        // set value of active element to data
        list->activeElement->data = data;
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if(list->activeElement != NULL){
        list->activeElement = list->activeElement->nextElement;
    }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    if(list->activeElement != NULL){
        list->activeElement = list->activeElement->previousElement;
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    // return 1 if list is active, 0 if not
    return (list != NULL && list->activeElement != NULL) ? 1 : 0;
}

/* Konec c206.c */
