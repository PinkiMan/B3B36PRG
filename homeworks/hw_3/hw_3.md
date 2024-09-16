# Úkol 3 - Caesarova šifra

| Termín odevzdání | 01.04.2023 23:59 AoE                                                                           |
|------------------|------------------------------------------------------------------------------------------------|
| Povinné zadání   | 2b kontrola Coding Stylu                                                                       |
| Volitelné zadání | Není                                                                                           |
| Bonusové zadání  | 2b 26.05.2023 23:59 CEST                                                                       |
| Počet uploadů    | 20                                                                                             |
| Výchozí soubory  | [b3b36prg-hw03.zip](files/b3b36prg-hw03.zip)<br>[b3b36prg-hw03b.zip](files/b3b36prg-hw03b.zip) |


## Povinné zadaní

Naším úkolem je rozluštit zprávu zakódovanou [Caesarovou šifrou](https://en.wikipedia.org/wiki/Caesar_cipher), která funguje na principu záměny písmene z abecedy za písmeno posunuté o pevně určený počet míst. V našem případě máme k dispozici nejen zašifrovaný text, ale také nespolehlivě odposlechnuté originální znění zprávy, tj. některé znaky originální zprávy jsou chybně zapsány. Řešení tak můžeme založit na testování všech možností kódování Caesarovou šifrou a porovnání s odposlechnutým textem. Posun s největší shodou dekódovaného textu s odposlechnutou zprávu budeme považovat za správný a takto dekódovaný text je náš požadovaný výstup. Obecně nemusí tento postup vést na unikátní správné řešení, nicméně v testovaných případech vždy existuje unikátní řešení.

Jedním z cílů tohoto domácího úkolu je procvičení dynamické alokace paměti na základě velikosti vstupu. Proto délka vstupního textu není dopředu známa a je nutné v případě vyčerpání počáteční velikost (např. 10-100 znaků) dynamicky alokovat prostor větší, např. funkcí realloc() pro dvojnásobnou délku. V programu používejte pouze množství paměti, které řádově odpovídá zadanému vstupu. Odevzdávací systém kontroluje velikost dynamicky alokované paměti.

Použitá abeceda se skládá pouze z malých a velkých písmen bez diakritiky, tj. znaky v rozsahu [a-zA-Z].

> [!TIP]
> Pro načítání vstupu používejte pouze funkci getchar() nebo scanf(“%c”,…) tak, abyste si vyzkoušeli dynamickou alokaci paměti podle aktuálně potřeby odpovídající načítanému vstupu.
> Použití metody dynamického načítání textového řetězce prostřednictvím rozšíření scanf(“%ms”) nebo scanf(“%as”) není z tohoto důvodu povoleno.


## Vstup

Na standardním vstupu očekávejte dvě posloupnosti znaků (texty) na samostatných řádcích. První text je zakódovaná zpráva a druhý text je nespolehlivě odposlechnutý text.

- Pokud vstupní text neodpovídá abecedě [a-zA-Z] vypíše program na standardní chybový výstup “Error: Chybny vstup!” a skončí s návratovou hodnotou 100.
- V případě povinné části jsou oba vstupní texty stejné délky a pokud mají délku různou, program vypíše na stderr “Error: Chybna delka vstupu!” a skončí s návratovou hodnotou 101.
- Pokud nastanou obě chyby 100 a 101 současně, program vypíše pouze chybovou hlášku “Error: Chybny vstup!” a skončí s návratovou hodnotou 100.
- V případě řešení bonusové části mohou být délky vstupních textů různé a program tak chybou nekončí.

## Výstup

Na standardní výstup vypište dekódovanou zprávu jako posloupnost znaků zakončenou znakem konce řádku. Program končí s návratovou hodnotou 0.

## Implementance

Program vhodně dekomponujte na jednotlivé funkce, např. funkce pro dekódování textu o definovaný posun a výpočet vzdálenosti dvou textových řetězců:

```
void shift(const char *src, char *dst, int offset);
 
int compare(const char *str1, const char *str2);
```

Dále může být vhodné napsat funkci pro šifrování jednoho písmene, která bude respektovat zadanou abecedu [a-zA-Z]. Například:

```
char rotate(char original, int offset);
```

Příklad 1 - pub01-m

V prvním příkladu je zašifrován text “Helloworld”. Posun je zde o 42 písmen. (16 míst je mezi 'h' a 'x'; 26 pak mezi malými a velkými písmeny). Zašifrovaný text je tedy “xUbbemehbT”.

Odposlechnutý text může být například “??lloworld”, kde písmena na místě otazníku špatně odposlechneme a dostaneme jiné náhodné písmeno, například “XYlloworld”.

| Standardní vstup                    | Očekávaný výstup   | Očekávaný chybový výstup | Návratová hodnota |
|-------------------------------------|--------------------|--------------------------|-------------------|
| <pre>xUbbemehbT<br>XYlloworld</pre> | ```Helloworld```   | žádný                    | 0                 |

Příklad vyhodnocení. Největší počet shodných písmen (8/10) má posun o 10, proto jej vyhodnotíme jako správný výsledek.

```
00: xUbbemehbT ~ XYlloworld > correct letters: 0
01: yVccfnficU ~ XYlloworld > correct letters: 0
02: zWddgogjdV ~ XYlloworld > correct letters: 0
03: AXeehphkeW ~ XYlloworld > correct letters: 0
04: BYffiqilfX ~ XYlloworld > correct letters: 1
05: CZggjrjmgY ~ XYlloworld > correct letters: 0
06: DahhksknhZ ~ XYlloworld > correct letters: 0
07: Ebiiltloia ~ XYlloworld > correct letters: 0
08: Fcjjmumpjb ~ XYlloworld > correct letters: 0
09: Gdkknvnqkc ~ XYlloworld > correct letters: 0
10: Helloworld ~ XYlloworld > correct letters: 8 -- result
11: Ifmmpxpsme ~ XYlloworld > correct letters: 0
12: Jgnnqyqtnf ~ XYlloworld > correct letters: 0
...
50: vSZZckcfZR ~ XYlloworld > correct letters: 0
51: wTaadldgaS ~ XYlloworld > correct letters: 0
52: xUbbemehbT ~ XYlloworld > correct letters: 0
```

Příklad 2 - pub02-m

| Standardní vstup                    | Očekávaný výstup   | Očekávaný chybový výstup | Návratová hodnota |
|-------------------------------------|--------------------|--------------------------|-------------------|
| <pre>mnoXYhnJLA<br>JCudvgtXRi</pre> | ```studentPRG```   | žádný                    | 0                 |

Příklad 3 - pub03-m

| Standardní vstup                    | Očekávaný výstup | Očekávaný chybový výstup    | Návratová hodnota |
|-------------------------------------|------------------|-----------------------------|-------------------|
| <pre>fghQRa-CEC<br>scbdeMKARZ</pre> | žádný            | ```Error: Chybny vstup!```  | 100               |

Příklad 4 - pub04-m

| Standardní vstup               | Očekávaný výstup | Očekávaný chybový výstup            | Návratová hodnota |
|--------------------------------|------------------|-------------------------------------|-------------------|
| <pre>fghQRa<br>scbdeMK</pre>   | žádný            | ```Error: Chybna delka vstupu!```   | 101               |


> [!TIP]
> Jednou z častých chyb je zjišťování délky textu v každé iteraci. Výsledný program pak má časovou složitost O(n^2), viz jedno studentské řešení:
> ```for (int i = 0; i < strlen(text); i++) { ... }```

## Bonusové zadání

U volitelné části úlohu rozšiřujeme o možnost úplné ztráty písmene nebo naopak odposlechnutí písmene, které nebylo vůbec vysláno. Vstupní texty tak mohou mít různou délku. V tomto případě věrohodný text určíme na základě [Levenštejnovy vzdálenosti](https://en.wikipedia.org/wiki/Levenshtein_distance) vypočtené pomocí [Wagner-Fisher algoritmu](https://en.wikipedia.org/wiki/Wagner%E2%80%93Fischer_algorithm). Vstup bonusového zadání může obsahovat dva texty různé délky, proto při spuštění nevypisujeme chybu 101. Použití Levenštejnovy vzdálenosti by fungovalo i pro povinné zadání, ale bylo by výrazně pomalejší, používalo více paměti a odevzdávací systém by ho neakceptoval. Používejte proto prosím Levenštejnovu vzdálenost výhradně pro testovací vstupy z volitelného zadání. Používejte dynamickou alokaci paměti.

Příklad 1 - pub01-b

| Standardní vstup                      | Očekávaný výstup  | Očekávaný chybový výstup | Návratová hodnota |
|---------------------------------------|-------------------|--------------------------|-------------------|
| <pre>xUbbemehbT<br>Hellwooorld</pre>  | ```Helloworld```  | žádný                    | 0                 |

Vzdálenostní matice při výpočtu vzdálenosti [Wagner-Fisher algoritmeme](https://en.wikipedia.org/wiki/Wagner%E2%80%93Fischer_algorithm) je vyplňována postupně po řádcích s využitím předchozích hodnot (dynamické programování). Např. pro zprávy Helloworld a Hellowoorld.

|       | -  | **H** | **e** | **l** | **l** | **o** | **w** | **o** | **o** | **r** | **l** | **d** |
|-------|----|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
| \-    | 0  | 1     | 2     | 3     | 4     | 5     | 6     | 7     | 8     | 9     | 10    | 11    |
| **H** | 1  | 0     | 1     | 2     | 3     | 4     | 5     | 6     | 7     | 8     | 9     | 10    |
| **e** | 2  | 1     | 0     | 1     | 2     | 3     | 4     | 5     | 6     | 7     | 8     | 9     |
| **l** | 3  | 2     | 1     | 0     | 1     | 2     | 3     | 4     | 5     | 6     | 7     | 8     |
| **l** | 4  | 3     | 2     | 1     | 0     | 1     | 2     | 3     | 4     | 5     | 6     | 7     |
| **o** | 5  | 4     | 3     | 2     | 1     | 0     | 1     | 2     | 3     | 4     | 5     | 6     |
| **w** | 6  | 5     | 4     | 3     | 2     | 1     | 0     | 1     | 2     | 3     | 4     | 5     |
| **o** | 7  | 6     | 5     | 4     | 3     | 2     | 1     | 0     | 1     | 2     | 3     | 4     |
| **r** | 8  | 7     | 6     | 5     | 4     | 3     | 2     | 1     | 1     | 1     | 2     | 3     |
| **l** | 9  | 8     | 7     | 6     | 5     | 4     | 3     | 2     | 2     | 2     | 1     | 2     |
| **d** | 10 | 9     | 8     | 7     | 6     | 5     | 4     | 3     | 3     | 3     | 2     | **1** | 

Levenštejnova vzdálenost je zde rovna jedné, protože stačí pouze jedna operace odebrání písmena.


|                                 | Povinné zadání                                                                          | Bonusové zadání                                    |
|---------------------------------|-----------------------------------------------------------------------------------------|----------------------------------------------------|
| Název v BRUTE                   | HW03                                                                                    | HW03B                                              |    
| Odevzdávané soubory             | main.c, *.h [^1]                                                                        | main.c, *.h [^1]                                   |
| Argumenty při spuštění          | žádné                                                                                   | žádné                                              |
| Kompilace pomocí                | clang -pedantic -Wall -Werror -std=c99                                                  | clang -pedantic -Wall -Werror -std=c99             |
| Očekávaná časová složitost [^2] | O(n)                                                                                    | O(n^2)                                             |
| Očekávaná paměťová složitost    | O(n)                                                                                    | O(n^2)                                             |
| Paměťový limit (stack) [b]      | 50 000                                                                                  | 50 000                                             |
| Paměťový limit (heap) [b]       | INPUT * 20 + 20000 [^3]                                                                 | INPUT^2 * 10 + 10000                               |
| Procvičované oblasti            | práce s textem,<br>ASCII tabulka,<br>dynamická alokace paměti<br>podle velikosti vstupu | dynamické programování<br>Levenštajnova vzdálenost |

[^1]: Jeden soubor main.c může používat více hlavičkových souborů. Nevytvářejte žádné složky.
[^2]: V závislosti na počtu písmen n zašifrovaného textu
[^3]: INPUT vyjadřuje velikost vstupního souboru v bytech. Doplněno 18. 11. 2017 v reakci na studentský dotaz. Limit byl také navýšen.

