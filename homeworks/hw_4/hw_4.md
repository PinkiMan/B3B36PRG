# Úkol 4 - Hledání textu v souborech

| Termín odevzdání | 08.04.2023 23:59 AoE                                                                         |
|------------------|------------------------------------------------------------------------------------------------|
| Povinné zadání   | 2b                                                                      |
| Volitelné zadání | 3b                                                                                        |
| Bonusové zadání  | není                                                                    |
| Počet uploadů    | 20                                                                                             |
| Výchozí soubory  | [b3b36prg-hw04.zip](files/b3b36prg-hw04.zip) |



## Povinné zadání

Bez použití knihovny <string.h> (nebo podobné) napište program, který ze zadaného souboru vypíše všechny řádky obsahující zadaný řetězec. Budete tak implementovat zjednodušenou verzi programu grep pro příkazový řádek, který je dostupný ve většině operačních systémů Unixového typu.

Program bude volán ve tvaru:

```
./program PATTERN FILE
```

kde PATTERN je hledaný řetězec, FILE název souboru.

V případě jakékoli chyby vypište smysluplný důvod chyby na standardní chybový výstup a program bude ukončen s nenulovou návratovou hodnotou.

Program implementujte v souboru grep.c

Pro rozpoznání konce řádku je možné používat znak '\n'.

V případě prázdeného výstupu ukončete program s návratovou hodnotou 1.

## Příklad 1

První příklad je motivován praktickou ukázkou, jak v Ubuntu zjistit velikost volné operační paměti z příkazové řádky.

```
grep MemFr /proc/meminfo
```

```
./program MemFr /proc/meminfo
```


| Obsah vstupního souboru                                                                                                                                                   | Řetězec PATTERN | Očekávaný výstup                   | Očekávaný chybový výstup | Návratová hodnota |
|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-----------------|------------------------------------|--------------------------|-------------------|
| <pre>MemTotal:       16430396 kB<br>MemFree:         4848016 kB<br>MemAvailable:   11697864 kB<br>Buffers:         1139932 kB<br>Cached:          5662728 kB<br>...</pre> | ```MemFr```     | ```MemFree:         4848016 kB```  | žádný                    | 0                 |

## Příklad 2
Prázdný výstup

```
grep NeexistujiciText /proc/meminfo
```

| Obsah vstupního souboru                                                                                                                                                   | Řetězec PATTERN        | Očekávaný výstup | Očekávaný chybový výstup | Návratová hodnota |
|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------|------------------------|------------------|--------------------------|-------------------|
| <pre>MemTotal:       16430396 kB<br>MemFree:         4848016 kB<br>MemAvailable:   11697864 kB<br>Buffers:         1139932 kB<br>Cached:          5662728 kB<br>...</pre> | ```NeexistujiciText``` | žádný            | žádný                    | 1                 |


## Volitelné zadání

Ve volitelné části můžete přidat tři různé funkcionality, za které můžete získat body. Počet argumentů tedy již nebude konstantní a program bude volán ve tvaru:

```
./program [OPTIONS] PATTERN [FILE]
```

kde PATTERN je hledaný řetězec, FILE název souboru a OPTIONS jsou volitelné argumenty pro volitelné zadání.

## A - Přesměrování na standartní vstup (0.5b)

Pokud nebude zadán argument volitelný argument FILE, bude jako vstup použit stdin. Díky tomu je možné přesměrovat výstup jiného programu a filtrovat tak jeho výstup.

```
dmesg | grep dvd
```

Příklad výstupu:

```
[    2.653604] sr 5:0:0:0: [sr0] scsi3-mmc drive: 48x/48x writer dvd-ram cd/rw xa/form2 cdda tray
```

## B - Regulární výrazy (1b)

Pouze speciální symboly [*,+,?] bez závorkování. Speciální symbol se vyskytuje maximálně jednou.

```
./program -E PATTERN [FILE]
```

[Regulární výrazy](https://en.wikipedia.org/wiki/Regular_expression)

## Příklad 2

| Obsah vstupního souboru                           | Řetězec 'colou?r'          | Řetězec 'colou*r'                                 | Řetězec 'colou+r'                        |
|---------------------------------------------------|----------------------------|---------------------------------------------------|------------------------------------------|
| <pre>color<br>colour<br>colouur<br>colouuur</pre> | <pre>color<br>colour</pre> | <pre>color<br>colour<br>colouur<br>colouuur</pre> | <pre>colour<br>colouur<br>colouuur</pre> |

## C - Obarvení výstupu (1.5b)

Obarvení PATTERN ve výstupu červenou barvou, stejně jako originální grep. Text je možné obarvit v terminálu pomocí přidání přesné sekvence znaku (Escape sequence) před a za obarvovaný text, viz. [ANSI escape code](https://en.wikipedia.org/wiki/ANSI_escape_code). Tato funkcionalita se zapíná následujícím přepínačem:

```
./program --color=always PATTERN [FILE]
```

Používejte prosím stejné escape sekvence jako systémový grep, aby bylo možné výstup jednoduše zkontrolovat.

|                     | Před obarvený test                   | Za obarvený text    |
|---------------------|--------------------------------------|---------------------|
| Textový zápis       | ESC[01;31m + ESC[K                   | ESC[m + ESC[K       |
| Hexadecimální zápis | 1b 5b 30 31 3b 33 31 6d + 1b 5b 4b   | 1b 5b 6d + 1b 5b 4b |

Příklad pub05-C

| Obsah vstupního souboru        | Řetězec PATTERN | Očekávaný výstup | Očekávaný výstup (hexdump -C)                                                                                                                                                                                                                                                     |
|--------------------------------|-----------------|------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| <pre>Hello, hello<br>Bye</pre> | <pre>el</pre>   | Hello, hello     | <pre>00000000  48 1b 5b 30 31 3b 33 31  6d 1b 5b 4b 65 6c 1b 5b  \|H.[01;31m.[Kel.[ \|<br>00000010  6d 1b 5b 4b 6c 6f 2c 20  68 1b 5b 30 31 3b 33 31  \|m.[Klo, h.[01;31 \|<br>00000020  6d 1b 5b 4b 65 6c 1b 5b  6d 1b 5b 4b 6c 6f 0a     \|m.[Kel.[m.[Klo. \|<br>0000002f</pre> |


|                                 | Povinné zadání                         | Bonusové zadání                        |
|---------------------------------|----------------------------------------|----------------------------------------|
| Název v BRUTE                   | HW04                                   | HW04                                   |    
| Odevzdávané soubory             | grep.c                                 | grep.c                                 |
| Argumenty při spuštění          | žádné                                  | –color=always -E                       |
| Kompilace pomocí                | clang -pedantic -Wall -Werror -std=c99 | clang -pedantic -Wall -Werror -std=c99 |
| Očekávaná časová složitost [^1] | O(n)                                   | O(n^2) [^2]                            |

[^1]: Kde n je velikost vstupního souboru. 
[^2]: Složitost je dána použitím reulárních výrazů, jinak O(n).




