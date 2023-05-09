#include "Briscola.h"
#include <iostream>
#include <random>

using namespace std;

int Random(int min, int max) {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(min, max);

	return distr(gen);
}

void Giocatore::setName(int n) {
	std::cout << "Giocatore " << n << " come ti chiami: ";
	std::cin.getline(nome, 30);
}

Giocatore::Giocatore(int n) : mano() {
	this->isChiamante = true;
	this->isCompagno = false;
	punteggio = 0;
	setName(n);
}

void Giocatore::Pesca(Carte& mazzo, int num_carte) {
	srand(time(NULL));

	for (int i = 0; i < num_carte; i++) {
		int random = Random(0, mazzo.getSize() - 1);
		if (random > mazzo.getSize() - 1) random = mazzo.getSize() - 1;
		Carta* c = new Carta(mazzo.PrendiCarta(random));
		c->setProprietario(this->nome);
		this->mano.AggiungiInCoda(*c);
	}
}

Carte& Giocatore::getMano() {
	return mano;
}

void Giocatore::stampaNome() {
	std::cout << "Carte di " << nome << ":" << std::endl;
}

void Giocatore::stampaMano() {
	mano.Stampa();
}

Giocatore::~Giocatore() {
	mano.~Carte();
	delete[] nome;
}

void Scambia(nodoCarte** primo, nodoCarte** secondo) {
	nodoCarte* aux = *primo, * aux2 = *secondo;
	Carta temp = aux->carta;
	aux->carta = aux2->carta;
	aux2->carta = temp;
}

void ordinaLista(nodoCarte*& lis){
	for(nodoCarte* aux = lis; aux != nullptr; aux = aux->next)
		for (nodoCarte* aux2 = aux->next; aux2 != nullptr; aux2 = aux2->next) {
			if (aux->carta.getPunti() == 0)
				if (aux->carta.getValore() < aux2->carta.getValore()) {
					Scambia(&aux, &aux2);
					continue;
				}
			if (aux->carta.getPunti() < aux2->carta.getPunti())
				Scambia(&aux, &aux2);
		}
}

void Giocatore::OrdinaCarte() {
	Carte carteSegno[4];
	Carte* finale = new Carte;

	for (int i = 0; i < 4; i++) {
		for (nodoCarte* aux = this->mano.getPrimo(); aux != nullptr; aux = aux->next)
			if (aux->carta.getSegno() == (Segno)i)
				carteSegno[i].AggiungiInCoda(aux->carta);
	}

	for (int i = 0; i < 4; i++) {
		ordinaLista(carteSegno[i].getPrimo());
		while (carteSegno[i].getPrimo() != nullptr) {
			finale->AggiungiInCoda(carteSegno[i].PrendiCarta(0));
		}
	}
	nodoCarte* nodo = finale->getPrimo();
	mano.setCoda(nodo);
}

bool nomeDoppione(const char* nome, Giocatore**& giocatori, int j) {
	for (int i = 0; i < j; i++) {
		if (strcmp(giocatori[i]->getNome(), nome) == 0)
			return true;
	}

	return false;
}

void inizializzaGiocatori(Giocatore**& giocatori) {
	giocatori = new Giocatore* [5];
	for (int i = 0; i < 5; i++) {
		giocatori[i] = new Giocatore(i + 1);
		while(nomeDoppione(giocatori[i]->getNome(), giocatori, i)) {
			char nuovoNome[30];
			std::cout << "Un altro giocatore ha gia' questo nome, inserirne uno diverso." << std::endl;
			giocatori[i]->setName(i+1);
		}
	}
}

bool TuttiLisci(Giocatore**& giocatori) {
	for (int i = 0; i < 5; i++) {
		int totPunti = 0;
		for (nodoCarte* aux = giocatori[i]->getMano().getPrimo(); aux != nullptr; aux = aux->next)
			totPunti += aux->carta.getPunti();

		if (totPunti == 0)
			return true;
	}

	return false;
}

void StampaManoGiocatori(Giocatore**& giocatori) {
	for (int i = 0; i < 5; i++) {
		giocatori[i]->stampaNome();
		giocatori[i]->stampaMano();
	}
}

bool moreChiamanti(Giocatore**& giocatori) {
	int counter = 0;
	for (int i = 0; i < 5; i++) {
		if (giocatori[i]->chiamante())
			counter++;
	}

	return (counter > 1);
}

void UIGiro(Giocatore**& giocatori, int primoAGiocare, int indice) {
	int count = 0;
	cout << endl << endl << "Giro:" << endl;
	for (int i = primoAGiocare; count < 5; i++) {
		if (i == 5) i = 0;
		if (i == indice)
			cout << "   ->  " << giocatori[i]->getNome();

		else
			cout << "       " << giocatori[i]->getNome();
		count++;
	}
	cout << endl << endl;
}

int GiroChiamanti(Giocatore**& giocatori, int punteggioMinimo, int primoAGiocare) {
	for (int i = primoAGiocare; moreChiamanti(giocatori); i++) {
		if (i == 5) i = 0;
		if (!giocatori[i]->chiamante()) continue;
		system("cls");
		UIGiro(giocatori, primoAGiocare, i);
		int input;
		std::cout << "La mano di " << giocatori[i]->getNome() << ":" << std::endl << std::endl;
		giocatori[i]->stampaMano();
		std::cout << std::endl;
		do {
			std::cout << "Chiami? (Si parte da " << punteggioMinimo + 1 << ", inserisci un numero piu' basso per lasciare) (MAX 118) ";
			std::cin >> input;
		} while (input > 118);

		if (input < punteggioMinimo) {
			giocatori[i]->setChiamante(false);
			system("cls");
			continue;
		}

		punteggioMinimo = input;

		if (punteggioMinimo == 118) {
			for (int j = 0; j < 5; j++) {
				if (j == i) continue;
				giocatori[j]->setChiamante(false);
			}
		}
		system("cls");
	}

	if (punteggioMinimo == 74) {
		int input;
		UIGiro(giocatori, primoAGiocare, 4);
		std::cout << "La mano di " << giocatori[4]->getNome() << ":" << std::endl << std::endl;
		giocatori[4]->stampaMano();
		std::cout << std::endl;
		std::cout << "Chiami? (Si parte da " << punteggioMinimo + 1 << ", inserisci un numero piu' basso per lasciare) (MAX 118) ";
		std::cin >> input;
		if (input < punteggioMinimo) {
			giocatori[4]->setChiamante(false);
			system("cls");
			return punteggioMinimo;
		}
		punteggioMinimo = input;
		system("cls");
	}

	return punteggioMinimo;
}

bool nessunChiamante(Giocatore**& giocatori) {
	bool noChiamante = true;
	for (int i = 0; i < 5; i++) {
		if (giocatori[i]->chiamante())
			noChiamante = false;
	}

	return noChiamante;
}

int InizioGioco(Giocatore**& giocatori, int punteggioMinimo, int& primoAGiocare) {
	inizializzaGiocatori(giocatori);

	cout << endl << giocatori[(primoAGiocare - 1 < 0) ? 4 : primoAGiocare - 1]->getNome() << " mischia il mazzo..." << endl;
	system("pause");

	while (punteggioMinimo == 74) {
		Carte mazzo(40);

		for (int i = 0; i < 5; i++) {
			giocatori[i]->CancellaCarte();
			giocatori[i]->Pesca(mazzo, 8);
			giocatori[i]->OrdinaCarte();
			giocatori[i]->setChiamante(true);
			/*
			giocatori[i]->stampaNome();
			giocatori[i]->stampaMano();
			*/
			std::cout << std::endl;
		}

		if (TuttiLisci(giocatori)) {
			std::cout << endl << "Un giocatore ha in mano tutti lisci." << std::endl;
			StampaManoGiocatori(giocatori);
			continue;
		}
		punteggioMinimo = GiroChiamanti(giocatori, punteggioMinimo, primoAGiocare);
		{
			if (nessunChiamante(giocatori)) {
				primoAGiocare = Random(0, 4);
				cout << giocatori[(primoAGiocare - 1 < 0) ? 4 : primoAGiocare - 1]->getNome() << " mischia il mazzo..." << endl;
				system("pause");
			}
		}
	}

	return punteggioMinimo;
}

int WhoIsChiamante(Giocatore**& giocatori) {
	for (int i = 0; i < 5; i++) {
		if (giocatori[i]->chiamante())
			return i;
	}
	return -1;
}

int ScegliCarta(Giocatore* giocatore) {
	int scelta;

	do {
		cout << "Scegliere una carta da giocare: ";
		cin >> scelta;
		scelta--;
	} while (scelta + 1 > giocatore->getMano().getSize() || scelta + 1 <= 0);

	return scelta;
}

void GiocaCarta(Giocatore**& giocatori, int i, int& primoAGiocare, Carte& terra, int punteggioMinimo, bool giroMorto = false) {
	int scelta;
	if (giroMorto) {
		do {
			cout << giocatori[WhoIsChiamante(giocatori)]->getNome() << " e' il chiamante e ha chiamato " << punteggioMinimo;
			UIGiro(giocatori, primoAGiocare, i);
			if (terra.getSize() > 0) {
				cout << endl << "Carte a terra: " << endl << endl;;
				terra.Stampa(true);
				cout << endl << endl << endl;
			}


			giocatori[i]->stampaNome();
			cout << endl;
			giocatori[i]->stampaMano();
			cout << endl << endl;
			scelta = ScegliCarta(giocatori[i]);
			if (giocatori[i]->getMano().getCarta(scelta).getPunti() + terra.totPunti() > (120) - punteggioMinimo) {
				system("cls");
				cout << "Non puoi giocare questa carta perche' il chiamante non puo' perdere nel giro morto." << endl << endl;
			}
		} while (giocatori[i]->getMano().getCarta(scelta).getPunti() + terra.totPunti() > (120) - punteggioMinimo);
	}

	else {
		cout << giocatori[WhoIsChiamante(giocatori)]->getNome() << " e' il chiamante e ha chiamato " << punteggioMinimo;
		UIGiro(giocatori, primoAGiocare, i);
		if (terra.getSize() > 0) {
			cout << "Carte a terra: " << endl;;
			terra.Stampa(true);
			cout << endl << endl;
		}


		giocatori[i]->stampaNome();
		cout << endl;
		giocatori[i]->stampaMano();
		cout << endl << endl;
		scelta = ScegliCarta(giocatori[i]);
		cout << endl << endl;
	}

	terra.AggiungiInCoda(giocatori[i]->getMano().PrendiCarta(scelta));
}

const char* segnoToString(Segno segno) {
	switch (segno) {
	case ORO:
		return "oro";
	case MAZZE:
		return "mazze";
	case COPPE:
		return "coppe";
	case SPADE:
		return "spade";
	}
}

const char* valoreToString(int valore) {
	switch (valore) {
	case 10:
		return "Re";
	case 9:
		return "Cavallo";
	case 8:
		return "Donna";
	case 1:
		return "Asso";
	default:
		char* num = new char[2];
		sprintf_s(num, 2, "%d\0", valore);
		return num;
	}
}

Segno ChiamaCarta(Giocatore**& giocatori, Carte& terra, Carta& chiamata) {
	int sceltaSegno, sceltaCarta, indiceChiamante = WhoIsChiamante(giocatori);

	cout << "Carte a terra: " << endl << endl;
	terra.Stampa(true);

	cout << endl << giocatori[indiceChiamante]->getNome() << " la tua mano:" << endl << endl;
	giocatori[indiceChiamante]->stampaMano();

	cout << endl << endl <<"Scegli il segno della briscola:" << endl << endl;

	for (int i = 0; i < 4; i++) 
		cout << i+1 << ".\t" << segnoToString((Segno)i) << endl;
	
	do {
		cout << endl << endl << "Scelta: ";
		cin >> sceltaSegno;
	} while (sceltaSegno > 4 || sceltaSegno < 1);

	Segno briscola = (Segno)(sceltaSegno-1);

	system("cls");
	cout << "Carte a terra: " << endl << endl;
	terra.Stampa(true);
	cout << endl << giocatori[indiceChiamante]->getNome() << " la tua mano:" << endl << endl;
	giocatori[indiceChiamante]->stampaMano();
	cout << endl << giocatori[indiceChiamante]->getNome() << " scegli la carta da chiamare:" << endl << endl;

	for (int i = 1; i <= 10; i++) 
		cout << i << ".\t" << valoreToString(i) << " di " << segnoToString(briscola) << endl;
	

	do {
		cout << endl << endl << "Scelta: ";
		cin >> sceltaCarta;
	} while (sceltaCarta > 10 || sceltaCarta < 1);

	Carta cartaChiamata(briscola, sceltaCarta);
	chiamata = cartaChiamata;

	cout << endl << giocatori[indiceChiamante]->getNome() << " ha chiamato:\t";
	cartaChiamata.stampaCarta();

	for (int i = 0; i < 5; i++) {
		if (giocatori[i]->getMano().CartaInLista(cartaChiamata)) {
			giocatori[i]->setCompagno(true);
		}
	}

	Carta* cartaATerra = terra.CercaInLista(cartaChiamata);

	if (cartaATerra != nullptr) {
		for (int i = 0; i < 5; i++) 
			if (strcmp(giocatori[i]->getNome(), cartaATerra->getProprietario()) == 0)
				giocatori[i]->setCompagno(true);
	}

	return briscola;
}

int AssegnaPunti(Giocatore**& giocatori, Carte& terra, Segno briscola) {
	Carta* max = &terra.getPrimo()->carta;
	
	for (nodoCarte* aux = terra.getPrimo(); aux != nullptr; aux = aux->next) {
		if (aux->carta.getSegno() == briscola) {
			if (max->getSegno() != briscola)
				max = &aux->carta;
			else {
				if (aux->carta.getPunti() == 0 && max->getPunti() == 0) {
					if (aux->carta.getValore() > max->getValore())
						max = &aux->carta;
				}
				else
					if (aux->carta.getPunti() > max->getPunti())
						max = &aux->carta;
			}
		}

		if (aux->carta.getSegno() == max->getSegno()) {
			if (aux->carta.getPunti() == 0 && max->getPunti() == 0) {
				if (aux->carta.getValore() > max->getValore())
					max = &aux->carta;
			}
			else
				if (aux->carta.getPunti() > max->getPunti())
					max = &aux->carta;
		}
	}

	for (int i = 0; i < 5; i++) {
		if (strcmp(giocatori[i]->getNome(), max->getProprietario()) == 0) {
			giocatori[i]->prendiPunti(terra.totPunti());
			return i;
		}
	}
}

Segno GiroMorto(Giocatore**& giocatori, int punteggioMinimo, int& primoAGiocare, Carta& chiamata) {
	Carte terra;
	Segno briscola;
	int count = 0;

	for (int i = primoAGiocare; count < 5; i++) {
		if (i == 5) i = 0;
		GiocaCarta(giocatori, i, primoAGiocare, terra, punteggioMinimo, true);
		system("cls");
		cout << "Carte a terra:" << endl << endl;
		terra.Stampa(true);
		cout << endl << "Tocca a " << ((i + 1 == 5) ? giocatori[0]->getNome() : giocatori[i + 1]->getNome()) << endl << endl;
		system("pause");
		system("cls");
		count++;
	}

	briscola = ChiamaCarta(giocatori, terra, chiamata);
	primoAGiocare = AssegnaPunti(giocatori, terra, briscola);
	cout << endl << giocatori[primoAGiocare]->getNome() << " ha preso " << terra.totPunti() << " punti." << endl << endl;
	system("pause");
	system("cls");
	terra.~Carte();
	return briscola;
}

void GiroStardard(Giocatore**& giocatori, int& primoAGiocare, Segno briscola, int punteggioMinimo, Carta& chiamata) {
	Carte terra;
	int count = 0;

	for (int i = primoAGiocare; count < 5; i++) {
		if (i == 5) i = 0;
		cout << giocatori[i]->getNome() << " hai in totale " << giocatori[i]->getPunteggio() << " punti." << endl << endl;
		cout << "La carta chiamata e': " << chiamata.valoreToString() << " di " << chiamata.segnoToString() << endl << endl;
		GiocaCarta(giocatori, i, primoAGiocare, terra, punteggioMinimo);
		system("cls");
		cout << "Carte a terra:" << endl << endl;
		terra.Stampa(true);
		cout << endl << "Tocca a " << ((i + 1 == 5) ? giocatori[0]->getNome() : giocatori[i + 1]->getNome()) << endl << endl;
		system("pause");
		system("cls");
		count++;
	}

	primoAGiocare = AssegnaPunti(giocatori, terra, briscola);
	cout << "Carte a terra:" << endl << endl;
	terra.Stampa(true);
	cout << endl << endl << giocatori[primoAGiocare]->getNome() << " ha preso " << terra.totPunti() << " punti." << endl << endl;
	system("pause");
	system("cls");
	terra.~Carte();
}

void StampaVincitori(Giocatore**& giocatori, int punteggioMinimo) {
	int punteggioChiamante = 0, punteggioGiocatori;
	
	cout << "PUNTI FINALI:" << endl << endl;

	for (int i = 0; i < 5; i++) {
		if (giocatori[i]->chiamante() || giocatori[i]->compagno()) {
			punteggioChiamante += giocatori[i]->getPunteggio();
		}

		cout << "Punteggio di " << giocatori[i]->getNome() << ": " << giocatori[i]->getPunteggio();
		if (giocatori[i]->chiamante()) cout << "\t (chiamante)";
		if (giocatori[i]->compagno()) cout << "\t (compagno)";
		cout << endl;
	}

	punteggioGiocatori = 120 - punteggioChiamante;
	
	cout << endl << "Punteggio della squadra chiamante: " << punteggioChiamante;
	cout << endl << "Punteggio della squadra giocatori: " << punteggioGiocatori;

	cout << endl << endl << ((punteggioChiamante >= punteggioMinimo) ? "La squadra chiamante ha vinto!" : "La squadra giocatori ha vinto!") << endl;
}