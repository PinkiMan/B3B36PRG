# Úkol 7 - Fronta spojovým seznamem s řazením

| Termín odevzdání | 29.04.2023 23:59 AoE                         |
|------------------|----------------------------------------------|
| Povinné zadání   | 2b                                           |
| Volitelné zadání | 2b                                           |
| Bonusové zadání  | Není                                         |
| Počet uploadů    | 20                                           |
| Výchozí soubory  | [b3b36prg-hw07.zip](files/b3b36prg-hw07.zip) |


> [!TIP]
> Případnou inspirace k spojovým strukturám v Cčku můžete najít v podkladech https://cw.fel.cvut.cz/wiki/courses/b0b36prp/lectures/start#spojove_struktury a to včetně zdrojových souborů. V případě významné inspirace uveďte zdroj.

## Povinné zadaní
Cílem úlohy je seznámit se se strukturou spojového seznamu a procvičit si dynamickou alokaci. Nejdříve je úkolem implementovat datovou strukturu realizující frontu spojovými seznamem pro uložení celých čísel typu int. Spojový seznam bude uložen v globálních proměnných, a proto nebude možné vytvořit více instancí spojového seznamu. Datová struktura fronty poskytuje rozhraní:

- Funkce push() vloží prvek na konec fronty
- Funkce pop() vyjme prvek z čela fronty
- Funkce insert() vloží prvek před nejbližší menší nebo stejný prvek ve frontě směr od čela fronty na její konec. V případě, že žádné takové prvky ve frontě nejsou, je prvek zařazen na konec fronty. Jeli hodnota prvku nejvyšší číslo ve frontě, je prvek umístěn do čela fronty.
- Funkce erase() smaže všechny prvky ve frontě jejichž hodnota je identická s hodnotu předávaného prvku.
- Funkce getEntry() vrací hodnotu prvku ve frontě na pozici idx
- Funkce size() vrací aktuální počet prvků ve frontě.
- Funkce clear() smaže všechny prvky ve frontě.


> [!WARNING]
> Abychom při volání funkce pop() mohli z návratové hodnoty rozlišit, že je fronta prázdná, povolujeme vkládání pouze nezáporných hodnot. Záporná návratová hodnota funkce pop() a podobně též getEntry() indikuje, že je fronta prázdná nebo prvek na dané pozici není. Alternativním řešením může být předávání ukazatele pro vyplnění příznaku chyby nebo ukončení program. Pro jednoduchost a přehlednost programu je zvoleno řešení se zápornou hodnotou.

Spojový seznam implementujte v souboru linked_list.c s rozhraním definovaným v souboru linked_list.h.

> [!TIP]
> Datovou strukturu pro spojový seznam definujte v souboru linked_list.c jako modulovou “globální” proměnnou, kterou při deklaraci řádně incializujte.

## Rozhraní definované v souboru ''linked_list.h''

```
/*
 * Push the entry value to the queue. The value of the entry must be >= 0.
 * return: true on success and false otherwise.
 */
_Bool push(int entry);
 
/*
 * Pop an entry from the head of the queue
 * return: the stored int value or value less than 0 indicating the queue is empty
 */
int pop(void);
 
/*
 * Insert the given entry to the queue in the InsertSort style.
 *
 * Since push and insert functions can be combined, it cannot be 
 * guaranteed, the internal structure of the queue is always sorted.
 *
 * The expected behaviour is that insert proceeds from the head of
 * the queue to the tail in such a way that it is insert before the entry 
 * with the lower value, i.e., it becomes a new head if its value is the
 * new maximum or a new tail if its value is a new minimum of the values
 * in the queue.
 *
 * return: true on success; false otherwise
 */
_Bool insert(int entry);
 
/*
 * Erase all entries with the value entry, if such exists
 * return: true on success; false to indicate no such value has been removed
 */
_Bool erase(int entry);
 
/*
 * For idx >= 0 and idx < size(queue), it returns the particular item
 * stored at the idx-th position of the queue. The head of the queue
 * is the entry at idx = 0.
 *
 * return: the particular value of the entry at the idx-th position or
 * value less than 0 to indicate the requested position is not presented
 * in the queue 
 */
int getEntry(int idx);
 
/*
 * return: the number of stored items in the queue
 */
int size(void);
 
/*
 * Remove all entries in the linked list
 */
void clear();
```

## Volitelné zadání
Volitelné zadání generalizuje datovou strukturu pro ukládání obecného datového typu předávaného jako ukazatel na void* a také zobecňuje použití fronty nikoliv jako globální proměnné, ale jako nově vytvářené datové struktury. Proto jsou pro práci s frontou předepsány funkce:

- create() - alokuje potřebnou navrženou datovou strukturu pro uložení spojového seznamu a příslušných proměnných.
- clear() - uvolní paměť datové struktury pro spojový seznam a případně uvolní paměť všech vložených datových položek do spojového seznamu.
- setClear() - umožňuje nastavit funkci pro uvolnění paměti jednotlivé datové položky ukládané do spojového seznamu
- setCompare() - umožňuje nastavit funkci pro porovnání dvou datových položek dat1 a dat2

Samotné rozhraní pro práci s frontou je podobné předchozímu případu jen s tím rozdílem, že funkce:

- insert() vloží prvek před první prvek, pro který platí že porovnání prvků funkcí compare()(nový prvek, nějaký prvek fronty) má návratovou hodnotou 1 nebo 0 indikující, že vkládaný prvek je větší než prvek fronty. V případě shody prvků indikované nulovou návratovou hodnotou funkce compare() je prvek vložen před první takový prvek fronty. V případě, že žádné takové prvky ve frontě nejsou, je prvek zařazen na konec fronty. Máli vkládaný prvek nejvyšší hodnotou (dle funkce compare() ) je umístěn do čela fronty. Jeli jeho hodnota naopak nejmenší, je umístěn na konec fronty, tj. bude odebrán z fronty funkcí pop() jako poslední.

Spojový seznam pro ukládání obecného datového typu implementujte v souboru queue.c s rozhraním definovaným v souboru queue.h.

## Rozhraní definované v ''souboru queue.h''

```
/* 
 * Allocate a new queue structure or return NULL on an error.
 * Particular type is implementation dependent
 */
void* create();
 
/*
 * Release all memory accessible from the queue, i.e., all dynamic
 * data entries stored in the individual queue entries. The clear 
 * function must be properly set, see setClear() below.
 */
void clear(void *queue);
 
/*
 * Push the given item into the queue. The pointer to the entry
 * should not be NULL, i.e., storing NULL entries is not allowed.
 * return: true on success and false otherwise.
 */
_Bool push(void *queue, void *entry);
 
/*
 * Pop an entry from the head of the queue
 * return: the stored pointer to the entry or NULL if the queue is empty
 */
void* pop(void *queue);
 
/*
 * Insert the given entry to the queue in the InsertSort style using 
 * the set compare function (by the setCompare() ). If such a function
 * is not set or an error occurs during the insertion it returns false.
 *
 * Since push and insert functions can be combined, it cannot be 
 * guaranteed, the internal structure of the queue is always sorted.
 *
 * The expected behaviour is that insert proceeds from the head of
 * the queue to the tail in such a way that it is insert before the entry
 * with the lower value, i.e., it might become a new head or a new tail if necessary.
 *
 * return: true on success; false otherwise
 */
_Bool insert(void *queue, void *entry);
 
/*
 * Erase all entries with the value entry, if such exists
 * return: true on success; false to indicate no such value has been removed
 */
_Bool erase(void *queue, void *entry);
 
/*
 * For idx >= 0 and idx < size(queue), it returns the particular item
 * stored at the idx-th position of the queue. The head of the queue
 * is the entry at idx = 0.
 *
 * return: pointer to the stored item at the idx position of the queue
 * or NULL if such an entry does not exists at such a position
 */
void* getEntry(const void *queue, int idx);
 
/*
 * return: the number of stored items in the queue
 */
int size(const void *queue);
 
/*
 * Set a pointer to function for comparing particular inserted items
 * to the queue. It is similar to the function used in qsort, see man qsort:
 * "The comparison function must return an integer less than, equal to, or
 * greater than zero if the first argument is considered to be respectively
 * less than, equal to, or greater than the second."
 */
void setCompare(void *queue, int (*compare)(const void *, const void *));
 
/*
 * Set a pointer to function which can properly delete the inserted 
 * items to the queue. If it is not set, all the items stored in the
 * queue are deleted calling standard free() in the clear() 
 */
void setClear(void *queue, void (*clear)(void *));
```

## Odevzdávané soubory
Odevzdávejte pouze soubory linked_list.c a queue.c s implementací fronty int hodnot a obecné fronty. Všechny soubory uložte přímo do zip archivu a nevytvářejte žádné složky. Správnost implementace bude ověřena testovacím programem, který bude využívat odezdané implementace fronty hodnot int a obecné fronty implementované spojovým seznamem.

Povinné soubory:
- linked_list.c - implementace rozhraní pro práci se spojovým seznamem hodnot typu int z poviného zadání
Soubory volitelného zadání:
- queue.c - implementace rozhraní pro práci s frontou implementovanou spojovým seznamem obecných hodnot (typu void*) dle výše uvedeného rozhraní queue.h.

## Způsob testování
Pro jednodušší testování jsme připravili testovací programy, které načtou požadované operace ze standartního vstupu, interpretují je a vypíší výsledek. Zatímco v povinné části se pracuje pouze s celými čísly, tak fronta z volitelné části může obsahovat libovolná data. Proto jsme pro volitelnou část připravili hned 3 implementace spolu s odpovídajícími veřejnými příklady:
- opt-int – data obsahují celá čísla (obdoba povinné části)
- opt-str – data obsahují textové řetězce
- opt-stc – data obsahují strukturu složenou z jednoho celého čísla a jednoho textového řetězce

|                        | Povinné zadání                                | Volitelné zadání                       |
|------------------------|-----------------------------------------------|----------------------------------------|
| Název v BRUTE          | HW07                                          | HW07                                   |   
| Odevzdávané soubory    | linked_list.c                                 | queue.c                                |
| Argumenty při spuštění | žádné                                         | žádné                                  |
| Kompilace              | clang -pedantic -Wall -Werror -std=c99        | clang -pedantic -Wall -Werror -std=c99 |
| Procvičované oblasti   | dynamická alokace paměti<br>práce s ukazately | ukazatele na funkce                    |






