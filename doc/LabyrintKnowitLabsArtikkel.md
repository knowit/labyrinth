# Datastyrt Labyrint
Knowit Hardware Guild har bygget videre på det klassiske BRIO labyrintspillet ved å montere motorer og sensorer og bruke forskjellige teknologier (bl.a. 3D-design og produksjon, mikrokontrollere, computer vision m.m.) slik at spillet styres ved hjelp av en joystick.

Spilleren bruker en Xbox spillkontroller til å styre labyrinten. Målet er å få ballen raskest mulig inn i det midterste hullet og samtidig unngå å falle ned i de andre hullene. Labyrinten vet hvor ballen er, hvilken vinkel det er på aksene og hvilken vinkel brukeren vil ha på brettet.
Spillet kan brukes for underholdning, som en testplattform for kunstig intelligens, virtual reality og programmeringsoppgaver.

## Hardware Guild
Spillet ble utviklet i Hardware-gruppa i Knowit, en faggruppe for folk som liker å lage fysiske ting og prototyper. Gruppen er åpen for alle Knowits ansatte og det arrangeres jevnlige fellesaktiviteter med kurs, workshops og mekkekvelder. Gruppen har eget rom med lab på hovedkontoret med utstyr som bl.a. loddebolt, 3D-skriver, strømforsyninger m.m.

## Hvordan spillet fungerer
Spillet fungerer på denne måten:

1. Ballen plasseres i startområdet.
2. En bluetooth-gamepad kommuniserer med en PC. Den ene joysticken angir vinkel (x og y) på brettet.
3. PC-en sender info om ønsket vinkel på brettet til Arduinoen (mikrokontroller).
4. Arduinoen har to PID-algoritmer (en for x og en for y) med gyroskop som input og motorer (continuous servo, en for x og en for y) som output.
5. Spillet står i ventemodus inntil spilleren beveger på joystick. Når joysticken beveges setter PC-en på stoppeklokke.
6. Et kamera er koblet til PC-en. PC-en beregner ballens posisjon ved hjelp av computer vision (OpenCV).
7. Computer vision-algoritmen har et rektangulært område i midten av brettet som indikerer hullet som er målet. Dersom ballen blir borte, har spilleren enten tapt eller vunnet. Spilleren har tapt dersom ballen var inne i målområdet da ballen ble borte, eller tapte dersom ballen var utenfor målområdet da ballen ble borte.
8. Dersom spilleren har tapt, går spillet tilbake til ventemodus.
9. Dersom spilleren har vunnet, beregnes tiden/poengene opp mot en highscore-liste. Dersom spilleren var blant de ti beste, kan spilleren registrere sine initialer og spillet går tilbake til ventemodus for å vente på ny spiller eller nytt forsøk.

## Prototyping
Prototyping ble brukt flittig under utvikling av labyrinten. Det er viktig å begynne tidlig med prototyper for å teste ut at en ide vil fungere i praksis.

CAD (Cardboard Assisted Design) er en god teknikk for å teste ut ideer til fysiske ting. Vi begynte derfor med å lage servofester av papp og testprogrammer å verifisere at motorene kunne kontrollere brettet. Vi lagde også papirversjoner av selve spillebrettet og versjoner av mikrokontrollerenheten ved hjelp av prototypekretskort (solderless breadboard).

## Implementasjon
Etterhvert så vi at ideen kom til å fungere og implementerte løsningen ved hjelp av flere teknikker.

1. **3D-design** er benyttet for å konstruere boks og kretskort for spillkontroller, spillebrett og servoholder, samt diverse overganger og mindre føtter/fester.
2. **Laserkutting**: Vi laget egenutviklet spillebrett (selve labyrinten) med Knowit-logo og ønsket vanskelighetsgrad i tre med laserkutter.
3. **3D-utskrift**: Produksjon av servoholder samt andre nødvendige deler (adaptere, føtter til labyrint m.m.) ble gjort med 3D-skriver.
4. **Mikrokontrollerprogrammering**: Implementasjon av mikrokontrollerprogram på Arduino ble gjort i objektorientert C++ for integrasjon mellom Arduino og gyroskop, integrasjon og konfigurasjon av PID-algoritme for motorstyring, OLED-skjerm for statusinformasjon og seriell-kommunikasjon med PC.
5. **Computer Vision**: For integrasjon og konfigurasjon av computer vision brukte vi Python og OpenCV. Dette gir lokalisering av ballen på brettet.
6. **Serverkomponent i node.js**: En sentral serverkomponent ble implementert i node.js. Denne holder orden på tilstanden på spillet (om f.eks. spillet er i gang, spiller har tapt, vunnet osv. og poenggivning) og kommunikasjon mellom de forskjellige softwarekomponentene samt bluetooth-controller.
7. **Dashboard**: Et dashboard implementert i React viser status på spillet inklusive poeng, holder orden på highscore og gir grensesnitt for kalibrering og overvåking av PID-algoritme for motorkontroll.


## Resultat
Etter å ha laget labyrinten og stresstestet den bl.a. i tre lange dager på konferanse, må vi si oss godt fornøyd med resultatet. Det er heldigvis morsomt å spille labyrinten, og vi ser at flere spillere har blitt veldig gode til å spille spillet. Det tok oss lang tid før vi selv kunne teste ut labyrinten i praksis, så dette er for oss ingen selvfølge.

Til nå har vi benyttet labyrinten i flere sammenhenger med Knowit, blant annet Norwegian Developer Conference (NDC), studenttreff, Knowits sommerskole (for barn av Knowitansatte) og JavaZone.

Vi må også innrømme at vi har hatt en god dose flaks under arbeidet med labyrinten. Underveis har vi utført mange oppgaver vi på forhånd ikke visste hvordan vi skulle løse, og vi har lært veldig mye.

## Videre arbeid
Selv om vi er fornøyde med resultatet så langt, jobber vi nå videre for å forbedre den og gi den nye bruksområder. Vi ser stort potensiale i å utvide labyrinten med bedre funksjonalitet, bruke labyrinten som testplattform for andre ideer slik som Virtual reality, kunstig intelligens, og som en plattform for programmeringsoppgaver.

Vi jobber nå med tre hovedretninger på den videre utviklingen. Vi vet ikke om noen av disse ideene vil være gjennomførbare, nyttige eller morsomme, og tiden vil vise om de er vellykkede.

### Virtual reality
Vi jobber med å lage VR-versjon av labyrinten. Man styrer ballen ved hjelp av VR-headset og viser den i en fiktiv verden. Målet er at spilleren skal oppleve å være inne ballen samtidig som spilleren spiller labyrinten i virkeligheten.

Faggruppen for Virtual Reality i Knowit, Virtual Guild, er involvert for å lage spilldelen i Unity.

Siden vi vet hvor ballen er ved hjelp av Computer Vision, hvilken vinkel det er på brettet ved hjelp av gyroskop, og vi har en 3D-modell av brettet bør dette være mulig. Vi laget 3D-modell av selve labyrinten for å kunne produsere den med laserkutter. Denne modellen har vi lagt inn i spillmotoren Unity. I skrivende stund er det mulig å spille labyrinten på PC/ Mac.

Vi jobber nå med å integrere dette spillet med den fysiske labyrinten slik at vinkelen på spillerens hode sendes fra spillmotoren til labyrinten og at ballens posisjon og brettets vinkel sendes tilbake til spillmotoren.

Vi vet pr. i dag ikke hvordan dette vil oppleves for spilleren og om dette er noe det er mulig og/eller morsomt å spille. Det er noe jobber vi med å finne ut.

### Byggesett
Vi jobber også med å lage et byggesett som gjør at folk selv kan lage sin egen datastyrte labyrint.

Tanken er at mange har en labyrint og spillkontroller liggende ubrukt hjemme og at disse sammen med et byggesett kan brukes slik å bygge sin egen datastyrte labyrint. Dette krever at vi forenkler løsningen, lager gode byggeinstruksjoner og rydder opp i kildefiler. All kildekode til programvare og fysiske deler ligger allerede på Github, så den som har maskiner til å lage delene selv, kan gjøre dette allerede.

### Kunstig intelligens
Vi ser et stort potensiale i å bruke spillet som plattform for eksperimentering med kunstig intelligens.

En algoritme kan trenes opp i å spille spillet ved at den belønnes etter hvor langt den kommer i labyrinten. Vi gir data om ballens plassering og vinkel på brettet (faktisk og ønsket vinkel), samt status på spillet) til algoritmen, og disse dataene kan så brukes til å lære opp kunstig intelligens.

For at dette skal bli praktisk, kreves det at vi lager en løsning for automatisk transport av ballen tilbake til startområdet etter at hvert spill er ferdig. Vi ser for oss en arm eller heis som flytter ballen fra depot til startpunkt når den kunstige intelligensen har tapt eller kommet i mål.

### Forbedring av balldeteksjon
Vi kommer til å utføre en del eksperimenter for å se på alternativer til computer vision for deteksjon av ballens posisjon.

I dagens løsning får spillet info fra OpenCV om ballen er på brettet eller ikke (har falt ned i hull eller ikke), og hvis den er på brettet, hvor ballen er. Dette systemet fungerer tilfredsstillende, men har noen svakheter. Det krever montering av kamera på stativ, noe som tar stor plass. Vi ser også at Computer Vision er sårbart for lysforhold, spesifikt fargegjengivelse, noe som krever medbrakt halogenpære.

Vi ønsker oss en løsning som kan monteres i/under selve spillet, er mindre følsomt for eksterne forhold og tar mindre plass slik at spillet blir lettere å montere og transportere. Det er også en stor fordel dersom løsningen kan kjøres direkte på mikrokontroller, Raspberry Pi e.l.

Mulige løsninger kan være å bruke touchskjermteknologi på labyrintbrettet og/eller måling av magnetfelt (noe som forutsetter at ballen er magnetisk).

Andre muligheter vi ser for oss, er f.eks.

- styre spillet med mobiltelefon
- teste forskjellige inputmetoder for labyrintens bevegelser, f.eks. gyroskop plassert på balansebrett
- bruke spillet til programmeringsoppgaver, ved at spilleren skal programmere bevegelsene til brettet

### Konklusjon
Labyrinten har hittil vært et veldig spennende prosjekt som har gitt oss mange spennende utfordringer og gitt oss mange timer med moro.
Hvis du vil lære mer om prosjektet, finner du kildekoden til ting og programvare (trenger litt opprydning) her: [https://github.com/knowit/labyrinth](https://github.com/knowit/labyrinth)

Dersom du har tilgang til 3D-skriver og laserkutter og kan lodde, kan du lage din egen datastyrte labyrint med filene på Github.

Vi jobber med fortsettelse av prosjektet, og er spent på hva tiden fremover vil vise.


## For den spesielt interesserte
Vi får ofte spørsmål om hvorfor vi har brukt Continuous servo med PID-algoritme fremfor å bare bruke servomotorer.

Planen var å bruke vanlige servomotorer siden det ville vært langt enklere, så vi gjorde en rekke tester på vanlige servomotorer i starten av prosjektet. Dessverre fungerte det ikke etter planen.

Årsaken er at det er “glipp” i spillet, slik at vinkelen på brettet ikke nødvendigvis samsvarer med vinkelen på styreknotten. Dette antar vi skyldes at det er strikker rundt en metallstang inne i spillet som igjen er festet til selve labyrintbrettet. Strikken sklir mer i den ene retningen enn den andre.

Vi valgte derfor å bruke continuous servo (som på mange måter fungerer som en helt vanlig motor) sammen med PID-algoritme med vinkler gyroskopet som input. PID-algoritmen får hele tiden data om den faktiske vinkelen på brettet og justerer kontinuerlig kraften til motoren slik at den ønskede vinkelen på brettet oppnås. Dette minner om når et menneske spiller spillet. Et menneske vrir på knotten til brettet uten å ta hensyn til den absolutte vinkelen på knotten.

Denne løsningen fungerer veldig bra.