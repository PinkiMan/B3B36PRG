# Úkol 1 - Kreslení (ASCII art)

> [!TIP]
> Tato úloha slouží k procvičení vnořených cyklů a dekompozici programu na funkce. Není potřeba používat pole.



## Povinné zadání
- Program ze standardního vstupu načtěte dvě přirozená čísla v rozsahu 3 až 69, včetně obou těchto krajních hodnot.
- Z načtených čísel “vykreslete” s použitím znaků velké X (X), mezer ( ) a znaků nového řádku (\n) na standardní výstup domeček podle předlohy níže.
- První číslo udává šířku domečku, druhé výšku domečku. Viz Příklad 1.
- Pokud se nepodaří načíst obě celá čísla, tak vypište na standardní chybový výstup zprávu: “Error: Chybny vstup!” a program ukončete s návratovou hodnotou 100. Viz Příklad 2.
- Obě čísla (jak šířka, tak výška) musí být v daném intervalu. V případě, že tomu tak není, vypište na standardní chybový výstup zprávu: “Error: Vstup mimo interval!” a program ukončete s návratovou hodnotou 101. Viz Příklad 3.
- První číslo reprezentující šířku musí být vždy liché. Pokud tomu tak není vypište na standardní chybový výstup chybovou hlášku: “Error: Sirka neni liche cislo!”, v takovém případě je návratová hodnota programu 102. Viz Příklad 4.
- Každý řádek končí znakem nového řádku, který je ihned za posledním symbolem na řádku (X, nebo | v případě domečku s plotem).

V případě, že jsou první dvě čísla shodná, načtěte třetí kladné celé číslo, které udává délku a výšku plotu, viz Příklad 5.

- Velikost plotu musí být kladná a ostře menší než je výška domu. V případě, že tomu tak nebude, uvědomte o tom uživatele zprávou na standardní chybový výstup ve znění: “Error: Neplatna velikost plotu!” a program ukončete s návratovou hodnotou 103, viz Příklad 5, 6 a 7.
- Horizontální části plotu jsou vždy na prvním a posledním řádku plotu. Pravá část plotu končí vždy svislou plaňkou, takže první řádek plotu vypadá pro velikost 2 takto: “-|”, pro velikost 3: “|-|” a pro velikost 4: “-|-|” atd.
- Domeček je vyplněn znaky 'o' a '*', přičemž v levém horním rohu je vždy znak 'o' a znaky 'o' tvoří diagonály.

> [!WARNING]
> Ze standardního vstupu načítejte maximálně tři čísla a to pouze v případě, že jsou první dvě čísla shodná.

Pořadí kontroly vstupu:
Kontroly vstupu a případné vypsání chyby musí být provedeno v předem zadaném pořadí:

1. Zadaná šířka domu nebo výška domu nebo velikost plotu není platné číslo (100)
2. Zadaná šířka domu nebo výška domu není v požadovaném intervalu (101)
3. Zadaná šířka domu není liché číslo (102)
4. Zadaná velikost plotu musí být kladná a ostře menší než je výška domu (103)

> [!TIP]
> Chyba 100 indikuje, že očekávaný vstup není platné číslo, proto se vztahuje též na velikost plotu. Pokud se nepodaří načíst celé(á) číslo(a) neodpovídá vstup očekávání. Využijte návratovou hodnotu funkce scanf().

Příklad 1 - pub01-m

| Vstupní hodnoty (stdin) | Výstup (stdout)                                                                            | Chybový výstup (stderr) | Návratová hodnota |
|-------------------------|--------------------------------------------------------------------------------------------|-------------------------|-------------------|
| ```7 4```              | <pre>   X<br />  X X<br /> X   X<br />XXXXXXX<br />X     X<br />X     X<br />XXXXXXX</pre> | žádný                   | 0                 |

Příklad 2 - pub02-m

| Vstupní hodnoty (stdin) | Výstup (stdout) | Chybový výstup (stderr)     | Návratová hodnota |
|-------------------------|-----------------|-----------------------------|-------------------|
| ```x 16```              | žádný           | ```Error: Chybny vstup!```  | 100               |

Příklad 3 - pub03-m

| Vstupní hodnoty (stdin) | Výstup (stdout) | Chybový výstup (stderr)              | Návratová hodnota |
|-------------------------|-----------------|--------------------------------------|-------------------|
| ```-4 16```             | žádný           | ```Error: Vstup mimo interval!```    | 101               |

Příklad 4 - pub04-m

| Vstupní hodnoty (stdin) | Výstup (stdout) | Chybový výstup (stderr)                | Návratová hodnota |
|-------------------------|-----------------|----------------------------------------|-------------------|
| ```8 21```              | žádný           | ```Error: Sirka neni liche cislo!```   | 102               |

Příklad 5 - pub05-m

| Vstupní hodnoty (stdin) | Výstup (stdout)                                                                                                                                                                                                                                                              | Chybový výstup (stderr) | Návratová hodnota  |
|-------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------|--------------------|
| ```9 9 6```             | <pre>    X<br />   X X<br />  X   X<br /> X     X<br />XXXXXXXXX<br />Xo\*o\*o\*oX<br />X\*o\*o\*o\*X<br />Xo\*o\*o\*oX-\|-\|-\|<br />X\*o\*o\*o\*X \| \| \|<br />Xo\*o\*o\*oX \| \| \|<br />X\*o\*o\*o\*X \| \| \|<br />Xo\*o\*o\*oX \| \| \|<br />XXXXXXXXX-\|-\|-\|</pre> | žádný                   | 0                 |

Příklad 6 - pub06-m

| Vstupní hodnoty (stdin) | Výstup (stdout) | Chybový výstup (stderr)     | Návratová hodnota |
|-------------------------|-----------------|-----------------------------|-------------------|
| ```5 5 x```             | žádný           | ```Error: Chybny vstup!```  | 100               |

Příklad 7 - pub07-m

| Vstupní hodnoty (stdin) | Výstup (stdout) | Chybový výstup (stderr)                | Návratová hodnota |
|-------------------------|-----------------|----------------------------------------|------------------|
| ```5 5 10```            | žádný           | ```Error: Neplatna velikost plotu!```  | 103              |


> [!TIP]
> Ve výpisu domečku dbejte na bíle znaky (mezery), platný znak obrázku je zakončem symbolem nového řádku ('\n'). I znaky, které nejsou na první pohled viditelné jsou znaky, proto pro kontrolu můžete uložit standardní výstup programu do souboru a zobrazit jej v hexadecimálním tvaru například příkazem hexdump -C a porovnat s přiloženými vzorovými výstupy. Věnujte pozornost Testování HW programů před odevzdáním.





