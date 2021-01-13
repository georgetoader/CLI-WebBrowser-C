# basic-webbrowser-C
Programul simuleaza functionalitatea unui browser web minimal în linia de
comanda. In functie de comanda introdusa am apelat diversele functii construite. 
Fisierul "structures.h" contine structurile implementate. Programul foloseste
doar operatiile specifice stivelor si cozilor, fara a itera prin elementele acestora. 

* `INIT_BR` -> Initializeaza o noua structura de tip Browser.

* `set_band` -> Seteaza valoarea bandwidth-ului.

* `new_tab` -> Initializeaza o noua structura de tip Tab si o adauga stivei de 
		taburi existente din Browser.
		
* `del_tab` -> Sterge ultimul tab deschis prin scoaterea acestuia stiva de taburi.
* `change_tab` -> Schimba tabul curent in tabul cu indexul "tab_index" dat ca parametru
		functiei. Copiez stiva de taburi intr-o stiva auxiliara "aux" prin functia 
		"CopiereS" implementata in "TStiva.c" ce returneaza o copie a stivei date.
		Prin operatii de "Pop" parcurg elementele stivei si dupa ce extrag "nr_pasi"
		elemente stiu ca am ajuns la pozitia ceruta. Functia returneaza un pointer
		la aceasta pozitie.
* `print_open_tabs`	->	Functia afiseaza informatii despre taburile deschise. Verific
		stiva cu taburi existente din structura Browser transmisa ca parametru daca 
		este goala. Daca nu este atunci o copiez intr-o stiva auxiliara "aux" pe
		care o parcurg prin operatii de "Pop" si stochez elementele extrase, afisand 
		apoi informatiile acestora despre pagina curenta.
* `go_to` -> Functia deschide in tabul curent o noua pagina web. Initializez un nou
		element de tip "Webpage" cu informatiile primite ca parametru si obtinute
		in urma apelarii functiei "get_page_resources" oferita in scheletul temei.
		Adaug pagina curenta in stiva "back_stack" deoarece aceasta va fi inlocuita
		de noul element creat, ce il voi stoca apoi in "current_page". Golesc apoi stiva
		"forward_stack" deoarece paginile stocate nu mai sunt valabile. Adaug pagina
		creata la coada cu istoricul de cautari "history" din Browser, dar apelez
		si functia "wait" deoarece deschiderea unei noi pagini web dureaza 1 secunda.
* `back` -> Functia schimba pagina curenta cu varful stivei "back_stack". Daca nu
		exista elemente in stiva atunci programul va afisa un mesaj de eroare, altfel
		va adauga in stiva "forward_stack" pagina curenta iar apoi aceasta va fi 
		inlocuita de cea mai recenta intrare din stiva "back_stack" prin folosirea
		functiei "Pop".
* `forward` -> Functia schimba pagina curenta cu varful stivei "forward_stack". Daca nu
		exista elemente in stiva atunci programul va afisa un mesaj de eroare, altfel
		va adauga in stiva "back_stack" pagina curenta iar apoi aceasta va fi 
		inlocuita de cea mai recenta intrare din stiva "forward_stack" prin folosirea
		functiei "Pop".
* `history` -> Functia afiseaza coada ce contine istoricul de cautari din Browser prin
		apelarea functiei "AfisareQ" implementata in "TCoada.c".
* `del_history` -> Functia sterge primele "nr" intrari din coada "history" prin apelarea
		de "nr" ori a functiei "Dequeue".
* `list_dl` -> Functia afiseaza lista de resurse descarcabile prin parcurgerea numarului
		de resurse din pagina curenta a tabului curent.
* `print_downloads` -> Functia afiseaza stadiul curent al descarcarilor. Verifica daca
		exista elemente in stiva de "downloads" din Browser, si daca da atunci face
		o copie a stivei in "aux" prin apelarea functiei "CopiereQ" din "TCoada.c".
		Parcurg stiva "aux", extrag elementele si pentru fiecare afisez stadiul curent
		de descarcare. Dupa ce am terminat de parcurs coada o distrug si creez o noua
		coada "aux" ce va fi copia cozii "finished" din Browser ce retine descarcarile
		deja terminate. Parcurg si elementele acestei cozi si le afisez.
* `download` -> Functia adauga in coada "downloads" resursa cu indexul "nr" transmis ca
		parametru din pagina curenta a tabului curent. Pentru adaugare in coada 
		folosesc functia "pushInQueue" din "TCoada.c" ce are rolul de a adauga 
		elemente in coada respectand prioritatea elementelor prin folosirea functiei
		de comparatie "fCmp". Aceasta calculeaza dimensiunea de descarcat ramasa
		a elementului si il adauga in coada pe pozitia corespunzatoare, ramanand
		astfel sortata crescatoar.
* `wait` -> Functia realizeaza o trecere fictiva a timpului pentru a se putea descarca
		resursele. Calculez totalul de Bytes ce pot fi descarcati in timpul de "nr"
		secunde transmise ca parametru. Verific cat mai are de descarcat elementul
		din varful cozii de "downloads", iar daca acest numar este mai mic decat
		"bytes" calculat mai devreme atunci voi scadea din "bytes" acest numar, iar
		elementul din varful cozii va fi extras si adaugat in coada de "finished".
		Daca nu este mai mic atunci "bytes" ia valoarea 0 iar "currently_downloaded"
		creste cu acest numar. Se repeta procesul pana cand coada de "downloads"
		este nula sau pana cand "bytes" devine 0. 
