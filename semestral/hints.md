# Hints - Semestrální práce

## Struktura ovládací aplikace

Aplikaci lze implementovat rozličným způsobem a lze také začít úplně od začátku, avšak doporučený postup je využít řešení předešlého domácího úkolu HW 8, který obsahují komunikaci pomocí roury, vícevláknovou aplikaci a vyčítání klávesnice.

Doporučených postup při implementaci ovládací aplikace může být následující:

1. Navrhnout strukturu více-vláknové ovládací aplikace. Dedikovat jedno vlákno na každý zdroj asynchronních událostí, tj. vstup od uživatele (klávesnice), komunikace s Modul aplikací pomocí roury, a jedno hlavní vlákno s frontou zpráv, ve které bude řešena aplikační logika. Jedná se o architekturu boss/worker, ve které hlavní vlákno přistupuje ke zdrojům (např. posílání zprávy) a ostatní vlákna komunikují s hlavním vláknem zejména prostřednictvím zasílání zpráv. Např. při stisku klávesy 'a' vlákno ošetřující vstup z klávesnice zasílá zprávu EV_ABORT hlavnímu vláknu, které následně odesílá zprávu MSG_ABORT Modul aplikaci.
2. Navrhnout vhodné datové struktury pro reprezentaci výpočtu, rozsahu části komplexní roviny, velikosti obrázku, výpočetního gridu a vlastního obrázku.
3. Navrhnout vhodný způsob a konvenci reportování stavů, např. zavedením logovacích tříd: INFO, DEBUG, WARN a ERROR.
4. Využít funkce pro vytvoření grafického okna xwin_init() a uvolnění zdrojů xwin_close() spolu s otestováním generování obrázku, např. barevné pruhy, červený obrázek, zelený obrázek atd., s využitím funkce xwin_redraw().
5. Následně implementujte vlastní výpočet fraktálu v rámci aplikace a ověřte zobrazení výpočetního gridu (tj. pole hodnot k pro jednotlivé pixely odpovídající bodům komplexní roviny) do barevného obrázku, např. funkcí compute() <pre>uint8_t compute(double cx, double cy, double px, double py, uint8_t max_iteration);</pre>.
6. Aplikaci otestujte na “memory-leaks”, korektní ukončení po stisku klávesy 'q', možnost přerušení výpočtu, atd.
7. Implementaci upravte, odstraňte nepotřebné části nebo reimplementujte méně čitelné části. Zkontrolujte text vypisovaných zpráv. Zkontrolujte dodržování zvolené kódovací konvence a případně kód upravte.
8. Aplikaci řádně otestujte.
9. Rozhodněte zdali aplikaci budete dále rozšiřovat či odevzdáte základní řešení
10. V případě dalšího rozšiřování zvažte uložení zdrojových kódu v repositáři, např. https://gitlab.fel.cvut.cz/

Pokud to bude možné konzultujte svůj postup se svým/sovu cvičící(m) na cvičení. Zdrojové kódy včas odevzdejte do odevzdávacího systému a domluvte časného odevdzání odevzdání, abyste mohli zapracovat zpětnou vazbu do úkolu.

## Vlákna

Použití vláken není vyloženě nutné, nicméně je velmi doporučené, protože umožňuje organizovat a zpřehlednit kód. V každém vláknu je možné se soustředit na konkrétní části odpovídající obsluze zdroje asynchroních zpráv, případně vizualizaci. Lze využít realizovaného programu [b3b36prg-lec09-codes.zip](files/b3b36prg-lec09-codes.zip), který v zásadě odpovídá ukázce na 8. přednášce.

Například lze realizovat vlákna pro každý zdroj asynchronních události, které jsou v aplikaci v zásadě dva.

1. Interaktivní vstup uživatele z klávesnice.
2. Příjem zpráv ze roury.

Přirozeně se nabízí vytvořit jedno hlavní vlákno, ve kterém můžeme implementovat aplikační logiku a po jednom vláknu pro každý zdroj asynchronních události (vstup z klávesnice a vyčítání dat ze roury).

Jelikož chceme umožnit okamžité (nebo velmi rychlé) ukončení aplikace při stisku klávesy q, použijeme načítání dat ze roury, dostupné v modulu prg_io_nonblock.c.

Komunikace přes roury je v našem případě plně duplexní, proto můžeme bezpečně v jednom vláknu pouze číst ze roury a v jiném vláknu zapisovat. Určitě je dobré číst vstup pouze v jediném vláknu (platí jak pro rouru, tak pro klávesnici). Z hlediska zápisu do roury je obecně vhodné též přistupovat k zápisu z jediného vlákna a to zejména z důvodu zapsání kompletní zprávy. Proto je nutné v přístupu k rouře z více vláken využít kritické sekce (tj. exkluzivní přístup řízený mutexem). V našem případě je plně postačující zapisovat do roury například pouze z hlavního vlákna, které řeší kompletní aplikační logiku programu.

Komunikaci mezi vlákny lze realizovat opět různými způsoby. Nicméně v případě hlavního vlákna a dvou vláken pro vyčítání interaktivního uživatelského vstupu z klávesnice a čtení dat ze roury probíhá komunikace pouze směrem od čtecích vláken do hlavního vlákna. Proto hlavní vlákno realizujeme jako smyčku, ve které zpracováváme zprávy z fronty, do které zapisují zprávy vyčítací vlákna. Hlavní vlákno reaguje na uživatelský vstup vytvořením příslušené zprávy pro Modul aplikaci nebo výpisem či aktualizací grafického výstupu po zpracování zpráv od Modulu.

> [!TIP]
> Přestože se může na první pohled zdát použití fronty a dalších zpráv složité, např. z hlediska definovaných struktur a délky kódu, umožňuje tento přístup organizovat kód do dílčích nezávislých částí. Definujeme pouze novou zprávu, její zaslání a zpracování. Vše jednotně a podobně jako ostatních zprávy. Přirozeně závisí na vkusu a zkušenosti, nicméně přehlednost a udržitelnost kódu je u takto explicitně definovaného rozhraní mnohem vyšší, neboť pracuje s datovou abstrakcí a vlastně i menším počtem sdílených proměnných. Zcela jistě svou roli také hraje zvládnutí editace textu zdrojových kódů. Při zvládnutí svého oblíbenoho prostředí je režie pro napsání kódu zanedbatelné v porovnání s případnou nutností ladění a řešení návazností synchronizačních funkcí.

## Komunikace mezi vlákny frontou zpráv
Komunikace mezi vlákny probíhá pomocí fronty zpráv s rozhraním popsaným v event_queue.h, který implementačně (event_queue.c) odpovídá např. kruhové frontě realizované v [HW 06 - Kruhová fronta v poli](../homeworks/hw_6/hw_6.md). Hlavní rozdíl je v přístupu k datové struktuře z více vláken. Frontu používáme ve stylu producent/konzument, kdy hlavní vlákno je konzumentem a vyčítací vlákna produkují zprávy. Proto je nutné zajistit exkluzivní přístup do fronty prostřednictvím kritické sekce (mutexu).

V případě, že je fronta prázdná, očekáváme, že konzument pozastaví svou činnost do doby, než se objeví nová zpráv, tak abychom zbytečně nevyužívali výpočetní zdroje. Vynucené ukončení od uživatele v tomto případě pošle zprávu, proto nepředstavuje žádný problém. V opačném případě, kdy konzument nestíhá zpracovat zprávy a fronta je plná, je nutné pozastavit činnost konzumentů a v případě zpracování zprávy (uvolnění místa ve frontě) je vhodné takového konzumenta znovou aktivovat. Proto použijeme podmíněné proměnné a mutex. Obecně očekáváme, že hlavní vlákno je dostatečně rychlé a k přeplnění by nemělo docházat často, případně jen nárazově. Podle toho volíme velikost fronty.

Vlastní zprávy ve frontě lze realizovat různě, od jednoduchého ukazatele na typ void až například po samostatné typy zpráv ve výčtovém typu a rozlišení zdroje zprávy jak je použito v dostupném souboru event_queue.h.

> [!TIP]
> Opět může tento přístup vypadat složitý a zbytečně dlouhý. Výhoda je v relativní explicitnosti a tím čitelnosti. Pedevším však udržitelnosti a také snadnějšímu ladění, neboť každá zpráva je jednoznačně identifikovatelná a dokumentovaná v definicich typu v event_queue.h.

Konkrétně v event_queue.h definujeme dva zdroje:

```
typedef enum {
   EV_NUCLEO,
   EV_KEYBOARD,
   EV_NUM
} event_source;
```

a pro každý zdroj zprávy definujeme samostatný obsah zprávy strukturami

```
typedef struct {
   int param;
} event_keyboard;
 
typedef struct {
   message *msg;
} event_serial;
```

Tedy pro vstup z klávesnice máme pouze datovou část v podobně int hodnoty. V případě zprávy ze sériového portu používáme přímo datovou strukturu message, kterou předáváme jako ukazatel, abychom se vyhnuli zbytečnému kopírování kompletní zprávy, která může být několik (desítek) bajtů dlouhá.

Samotnou zprávu pak definujeme jako zdroje zprávy (event_source) a typ zpráv (event_type), které jsou následně doplněny o vlastní zprávu a to buď struct event_keyboard nebo struct event_serial podle toho jestli je typ zprávy EV_KEYBOARD nebo EV_NUCLEO. Proto zde s výhodou použijeme typ union.

```
typedef struct {
   event_source source;
   event_type type;
   union {
      int param;
      message *msg;
   } data;
} event;
```

Položka data struktury event tak obsahuje param i msg. To zdali je v proměnné typu event zpráva od klávesnice (param) nebo ze sériového portu (msg) rozlišíme při vytváření zprávy nastavením příslušné datové části a typu zprávy. Příklad použití může vypadat například

```
void process_event(event *ev)
{
   if (ev->type == EV_KEYBOARD) {
      printf("Param value %\d\n", ev->data.param);
   } else if (ev->type == EV_NUCLEO) {
      printf("Msg ptr %p\n", ev->data.msg);
   }
}
```

## Komunikace proudem dat a využití union

Komunikace proudem dat je zápis a čtení posloupností bajtů, které jsou organizovány do zpráv. Ve zvoleném případě zpráv se jedná o zprávy definovaných (fixních) délek. Zcela jistě lze přistupovat ke zprávám jako posloupnosti bajtů a k jednolivým částech přistupovat prostřednictvím indexů prvků. Takový přístup však není příliš čitelný, neboť vyžaduje detailní znalost organizace jednotlivých zpráv. Proto může být výhodnější definovat si pro každý typ zprávy samostatnou strukturu tak, jak je to provedeno v přiloženém souboru messages.h. Tím získáme relativně čitelný přístup na dílčí položky jednotlivých zpráv. Komunikační zpráva pro Modul aplikaci pak odpovídá struktuře message, která obsahuje jednobajtový indikátor typu zprávy type a union možných dílčích zpráv

```
typedef struct {
   uint8_t type;   // message type
   union {
      msg_version version;
      msg_startup startup;
      msg_set_compute set_compute;
      msg_compute compute;
      msg_compute_data compute_data;
   } data;
   uint8_t cksum; // message command
} message;
```

Kromě typu a dílčího obsahu zprávy v položce data dále zpráva obsahuje položku cksum, která představuje primitivní způsob jak kontrolovat konzistenci a správnost došlých bajtů. Hodnota cksum je spočítána tak, aby součet jednotlivých bajtů (uint8_t) zprávy včetně cksum byl 0xFF. Tedy pokud se stane nějaká chyba, je možné ji detekovat. Jedná se o základní detekčních mechanismus a v praktických úloha se zpravidla navrhují [cycklické redundatní kódy](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) podle charakteru možných chyb. V našem případě nám postačuje tento jednoduchý kontrolní součet cksum, který je součástí transformace struktury zprávy do pole bajtů, která se provádí ve funkci fill_message_buf() v modulu messages.c ve druhé části funkce.

```
 // 2nd - send the message buffer
   if (ret) { // message recognized
      buf[0] = msg->type;
      buf[*len] = 0; // cksum
      for (int i = 0; i < *len; ++i) {
         buf[*len] += buf[i];
      }
      buf[*len] = 255 - buf[*len]; // compute cksum
      *len += 1; // add cksum to buffer
   }
```

neboť kontrolní součet je poslední bajt zpráv.

> [!TIP]
> Při posílání zpráv jako posloupnosti bajtů po sériové lince vás může napadnou co se stane, když se třeba jeden bajt ze zprávy nepřečte, případně se přenese chybně. V takovém případě může zafungovat právě cksum. Obecně však předpokládáme nějaký výchozí definovaný stav, např. běží ovládací aplikace a resetujeme Modul aplikaci. V takovém případě čteme ze roury první znak, na základě kterého identifikuje typ zprávy, který definuje délku zprávy. Proto dále čteme očekávaný počet bajtů. Následně kontrolujeme cksum. Pokud vše sedí, pokračujeme dále. Pokud ne, hlasíme chybu. Může se však stát taková chyba, kterou nedokážeme jednoduchým cksum detekovat, případně dojde k výpadku např. jednoho bajtu. Pak interpretujeme přijatá chybná data jako nějaký konkrétní typ zprávy, což může stále fungovat, jen to nebude podle našich původních představ.

## Zápis zprávy jako pole bajtů

Se zprávou jako strukturou se z hlediska přístupu k jednotlivým položkám zprávy pracuje mnohem intuitivněji než indexování do pole bajtů. Nicméně přenos zprávy po sériové lince je přenos definovaného počtu bajtů odpovídající jednotlivým položkám. Převodu zprávy (objektu / datové struktury) na posloupnost bajtů se říká [marshalling](https://en.wikipedia.org/wiki/Marshalling_(computer_science)) a v dostupném modulu messages.c je implementován ve funkci fill_message_buf(), která je jednou ze tří funkcí vyžadující detailní znalost organizace jendotlivých zpráv jako posloupnosti bajtů. Princip můžeme demonstrovat na zprávě MSG_VERSION, která je 5 bajtů dlouhá (typ zprávy, tři datové položky - major, minor, patch a cksum). Vyplnění prvního batju a posledního bajtu je pro všechny zprávy identické, a proto je součástí druhé části funkce uvedené výše. Vlastní datová část pak odpovída případu kdy msg→type je MSG_VERSION, např.

```
switch(msg->type) {
   ...
      case MSG_VERSION:
         buf[1] = msg->data.version.major;
         buf[2] = msg->data.version.minor;
         buf[3] = msg->data.version.patch;
         *len = 4;
         break;
   ...
   buf[0] = msg->type; 
   ...
   buf[*len] = 255 - buf[*len]; 
```

kde je demonstrován přístup k datové položce data typu union, která je v tomto případě validní jako data.version, neboť type je MSG_VERSION, který používáme právě pro indikaci typu zprávy.

## Vytvoření zprávy z pole bajtů

Při čtení zprávy ze sériového portu očekáváme, že ja vstupní buffer prázdný a první znak, který přečteme, odpovídá typu zprávy. Na základě typu zprávy můžeme určit počet bajtů zprávy, které je nutné přečíst, abychom mohli vytvořit zprávu konkrétního typu jako proměnnou typu struct. V modulu messages.c je definice funkce get_message_size(), která vrací očekávanou délku zprávy podle typu prostřednictvím předáváného ukazatele len. Například pro zprávu typu MSG_VERSION může funkce vypadat

```
bool get_message_size(uint8_t msg_type, int *len)
{
   bool ret = true;
   switch(msg_type) {
      ...
      case MSG_VERSION:
         *len = 2 + 3 * sizeof(uint8_t); // 2 + major, minor, patch
         break;
      ...
      default:
         ret = false;
         break;
   }
   return ret;
}
```

což je explicitní zápis pro 2 bajty společné pro všechny zprávy a 3 bajty pro major, minor a patch. Nic nám nebrání definovat například

``` *len = 5;```

Případně s využitím číslování typu zpráv jako pole. Konkrétní způsob je tak spíše věcí vkusu a představy o čitelnosti.

V okamžiku, že načteme očekávaný počet bajtů, můžeme pole bajtů interpretovat jako konkrétní zprávu. Příklad implementce je opět dostupný v modulu messages.c ve funkci parse_message_buf(), která může například pro MSG_VERSION vypadat následovně.

```
bool parse_message_buf(const uint8_t *buf, int size, message *msg)
{
   uint8_t cksum = 0;
   for (int i = 0; i < size; ++i) {
      cksum += buf[i];
   }
   bool ret = false;
   int message_size;
   if (
         size > 0 && cksum == 0xff && // sum of all bytes must be 255
         ((msg->type = buf[0]) >= 0) && msg->type < MSG_NBR &&
         get_message_size(msg->type, &message_size) && size == message_size) {
      ret = true;
      switch(msg->type) {
          ...
          case MSG_VERSION:
            msg->data.version.major = buf[1];
            msg->data.version.minor = buf[2];
            msg->data.version.patch = buf[3];
            break;
          ...            
          default: // unknown message type
            ret = false;
            break;
      } // end switch
   }
   return ret;
}
```

Opět vycházíme ze znalosti definice komunikačních zpráv jako posloupnosti bajtů. Testujeme cksum a případně reportujeme chybu návratovou hodnout false.

> [!TIP]
> Jedna z hlavních výhod použití modulu messages.c je, že pouze v tomto modulu je nutné použít znalost definice komunikačních zpráv jako posloupnosti bajtů. Máme tak pouze jedno místo, které zvyšuje přehlednost a případné ladění chyb.





















