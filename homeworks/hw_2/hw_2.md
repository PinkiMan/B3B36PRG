# Úkol 2 - Prvočíselný rozklad



Implementujte program, který načte ze standardního vstupu seznam celých kladných čísel zakončený nulou a provede jejich prvočíselný rozklad. Vstupní čísla jsou na jednotlivých řádcích. Číslo 1 je speciální případ vstupu, při kterém vypište 1 jako “prvočíselný rozklad”.

Pokud bude na vstupu záporné číslo nebo jiný neočekávaný vstup, vypiště chybovou hlášku “Error: Chybny vstup!” na standardní chybový výstup a ukončete program s návratovou hodnotou 100.



Příklad 1 - pub01-m

| Vstupní hodnoty (stdin)                     | Výstup (stdout)                                                                                                                                                                                       | Chybový výstup (stderr) | Návratová hodnota |
|---------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------|-------------------|
| <pre>1<br />11<br />120<br />8<br />0</pre> | <pre>Prvociselny rozklad cisla 1 je:<br />1<br />Prvociselny rozklad cisla 11 je:<br />11<br />Prvociselny rozklad cisla 120 je:<br />2^3 x 3 x 5<br />Prvociselny rozklad cisla 8 je:<br />2^3</pre> | žádný                   | 0                 |

Příklad 2 - pub02-m

| Vstupní hodnoty (stdin)               | Výstup (stdout)                                          | Chybový výstup (stderr)     | Návratová hodnota |
|---------------------------------------|----------------------------------------------------------|-----------------------------|-------------------|
| <pre>12<br />-2<br />100<br />0</pre> | <pre>Prvociselny rozklad cisla 12 je:<br />2^2 x 3</pre> | ```Error: Chybny vstup!```  | 100               |

Příklad 3 - pub03-m

| Vstupní hodnoty (stdin)              | Výstup (stdout)                                          | Chybový výstup (stderr)     | Návratová hodnota |
|--------------------------------------|----------------------------------------------------------|-----------------------------|-------------------|
| <pre>12<br />b<br />100<br />0</pre> | <pre>Prvociselny rozklad cisla 12 je:<br />2^2 x 3</pre> | ```Error: Chybny vstup!```  | 100               |

Příklad 4 - pub04-m

| Vstupní hodnoty (stdin)        | Výstup (stdout)                                                            | Chybový výstup (stderr) | Návratová hodnota |
|--------------------------------|----------------------------------------------------------------------------|-------------------------|-------------------|
| <pre>991350783547<br />0</pre> | <pre>Prvociselny rozklad cisla 991350783547 je:<br />995663 x 995669</pre> | žádný                   | 0                 |


## Povinné zadání

Na vstupu jsou pouze celá čísla, která jsou reprezentovatelná pomocí 64-bitového celočíselného znaménkového typu. Vzhledem k náročnosti úkolu předpokládáme, že největší číslo v prvočíselném rozkladu je menší než 10^6. Aby byl váš algoritmus efektivní pro opakovaná volání, tak si předpočítejte tabulku všech prvočísel do hodnoty 10^6 algoritmem Eratosthenovo síto. Následně je možné zkoušet dělení pouze nalezenými prvočísly a významně tak výpočet urychlit.

Doporučení: Pro uložení nalezených prvočísel používejte “pouze” 32-bitový celočíselný znaménkový typ.

## Příklad časové náročnosti

Rychlost vašecho programu pro volitelnou část můžete testovat například na rozkladu číselné řady od 1 do N. Výpočetní časy různých verzí programu spuštěných na standardním počítači1) a zkompilovaných se zapnutou optimalizací (-O3) jsou zaznamenány v následující tabulce. Pro představu jsme otestovali i instance opt01 až opt03 z odevzdávacího systému. V přepočtu na rychlost počítače v odevzdávacím systému odpovídá časový limit přibližně 1.8 až 2 s pro volitelné úlohy opt*.

| Řešení / Vstupní soubor | 1-10000 | 1-50000 | 1-100000 | opt01  | opt02  | opt03  |
|-------------------------|---------|---------|----------|--------|--------|--------|
| Naivní I                | 0.18 s  | 3.0 s   | 11.6 s   | 4.6 s  | DNF 2) | DNF 3) |
| Naivní II               | 0.16 s  | 2.3 s   | 8.8 s    | 4.3 s  | 4.4 s  | 4.6 s  |
| Referenční              | 0.04 s  | 0.3 s   | 1.1 s    | 0.4 s  | 0.4 s  | 0.4 s  |


## Naivní řešení I
Vstupní číslo zkouší dělit všemi čísly menšími než je vstupní číslo.

## Naivní řešení II
Vstupní číslo zkouší dělit všemi menšími čísly. Algoritmus končí, když je dělenec menší než dělitel.

## Referenční řešení
Využívá předpočítaná prvočísla do hodnoty 10^6. Algoritmus končí, když je dělenec menší než dělitel.

## Testovací instance na rychlost výpočtu

| Zkratka testu | Popis                                                                                             |
|---------------|---------------------------------------------------------------------------------------------------|
| man05         | Přibližně 8000 čísel, které jsou většinou prvočísly z intervalu 2 až 100000.                      |
| man06         | Přibližně 7000 čísel, které jsou většinou složené ze dvou prvočísel z intervalu 2 až 100000.      |
| man07         | Přibližně 300 čísel, které jsou většinou složené ze dvou prvočísel v intervalu 900000 až 1000000. |

> [!WARNING]
> Neukládejte prvočísla přímo do zdrojového kódu, maximální velikost souboru main.c je 50 kB.


## Bonusové zadání

Na vstupu bonusového zadání mohou být celá kladná čísla dlouhá až 100 cifer. Je tedy nutné vytvořit jejich vlastní reprezentaci v počítači spolu s příslušnými operacemi celočíselného dělení se zbytkem. Největší číslo v prvočíselném rozkladu bude vždy menší než 10^6. Při implementaci nepoužívejte cízí kód ani žádnou specializovanou knihovnu pro práci s velkými čísly. V tomto úkolu nemusíte používat Eratostenovo síto, protože časový limit nebude nijak přísný. Cílem je především práce s velkými čísly.

Příklad 5 - pub01-b
Rozkládané číslo4) je (995663∗995669)^8:


| Vstup  | <pre>932865073719992059629773513614789388266580305083920591925740371392254317064584855785088915745761<br />0</pre>                                                 |
|--------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Výstup | <pre>Prvociselny rozklad cisla 932865073719992059629773513614789388266580305083920591925740371392254317064584855785088915745761 je:<br />995663^8 x 995669^8</pre> |

Výpočetní čas na běžném počítači (bez optimalizace):

```
real    0m1.279s
user    0m1.264s
sys     0m0.004s
```

Časový limit v odevzdávacím systému je přibližně desetinásobek referenčního programu (10s).

## Odevzdání

> [!TIP]
> U tohoto domácího úkolu je linkována matematická knihovna <math.h> a to přepínačem -lm.


