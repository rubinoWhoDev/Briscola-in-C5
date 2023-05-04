#include "Briscola.h"
#include <iostream>
#include <random>

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

int GiroChiamanti(Giocatore**& giocatori, int punteggioMinimo) {
	for (int i = 0; moreChiamanti(giocatori); i++) {
		if (i == 5) i = 0;
		if (!giocatori[i]->chiamante()) continue;

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

int InizioGioco(Giocatore**& giocatori, int punteggioMinimo) {
	inizializzaGiocatori(giocatori);

	while (punteggioMinimo == 74) {
		Carte mazzo(40);

		for (int i = 0; i < 5; i++) {
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
			std::cout << "Un giocatore ha in mano tutti lisci." << std::endl;
			StampaManoGiocatori(giocatori);
			continue;
		}
		punteggioMinimo = GiroChiamanti(giocatori, punteggioMinimo);
		for (int i = 0; i < 5; i++)
			giocatori[i]->getMano().~Carte();
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