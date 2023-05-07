//#define _CRT_SECURE_NO_WARNINGS
#include "Carte.h"
#include <iostream>
#include <string>

Carta::Carta(int segno, unsigned int val) : valore(val) {
	this->segno = (Segno)segno;
	strcpy_s(this->proprietario, "");

	switch (val) {
	case 10:
		this->punti = 4;
		break;
	case 9:
		this->punti = 3;
		break;
	case 8:
		this->punti = 2;
		break;
	case 3:
		this->punti = 10;
		break;
	case 1:
		this->punti = 11;
		break;
	default:
		this->punti = 0;
		break;
	}
}

Carta::Carta(Carta& c) : segno(c.segno), valore(c.valore), punti(c.punti) {
	strcpy_s(this->proprietario, c.proprietario);
}

const char* Carta::valoreToString() {
	switch (this->valore) {
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
		sprintf_s(num, 2 , "%d\0", this->valore);
		return num;
	}
}

const char* Carta::segnoToString() {
	switch (this->segno) {
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

nodoCarte::nodoCarte(Carta& c) : carta(c){
	next = nullptr;
}

void Carta::stampaCarta() {
	std::cout << valoreToString() << " di " << segnoToString();
}

void Carte::AggiungiInCoda(Carta& carta) {
	nodoCarte* aux = new nodoCarte(carta);

	if (coda.primo == nullptr) {
		coda.primo = aux;
		coda.ultimo = aux;
		return;
	}
	
	coda.ultimo->next = aux;
	coda.ultimo = aux;
}

void Carta::setProprietario(const char nome_proprietario[30]) {
	strcpy_s(this->proprietario, nome_proprietario);
}

void Carta::stampaProprietario() {
	std::cout << proprietario;
}

Carte::Carte(int num_carte) {
	coda.primo = nullptr;
	coda.ultimo = nullptr;
	/*
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= 10; j++)
			AggiungiInCoda(Carta(i, j));
	}
	*/

	for (int i = 1; i <= num_carte; i++) {
		int decine = (int)((i) / 10), unita = i - (decine*10);
		if (i % 10 == 0) { unita = 10; decine -= 1; }
		Carta c(decine, unita);
		AggiungiInCoda(c);
	}
}

Carte::Carte(Carta& carta) {
	coda.primo = nullptr;
	coda.ultimo = nullptr;

	AggiungiInCoda(carta);
}

Carte::Carte(Carte& carte) : coda(carte.coda) {}

/*
void Carte::operator=(Carte c) {
	this->coda = c.coda;
}
*/

Carta& Carte::PrendiCarta(int n) {
	nodoCarte* aux = this->coda.primo;
	Carta* carta = nullptr;
	int i = 0;
	while (aux != nullptr) {
		if (i == n) {
			carta = new Carta(aux->carta);
			break;
		}
		i++;
		aux = aux->next;
	}

	CancellaCarta(&this->coda.primo, *carta);
	return *carta;
}

Carta& Carte::PrendiCarta(Carta& carta) {
	CancellaCarta(&this->coda.primo, carta);
	return carta;
}

bool Carta::operator==(Carta& c) {
	return (this->segno == c.segno && this->valore == c.valore);
}
/*
void Carta::operator=(Carta& c) {
	this->punti = c.punti;
	this->segno = c.segno;
	this->valore = c.valore;
}
*/

void Carte::CancellaCarta(nodoCarte** coda, Carta& carta) {
	nodoCarte* aux = *coda,*aux2 = nullptr;

	while (!(aux->carta == carta)) {
		aux2 = aux;
		aux = aux->next;
	}

	if (aux == this->coda.primo) {
		this->coda.primo = this->coda.primo->next;
		delete aux;
		return;
	}
	/*
	if (aux == this->coda.ultimo) {
		aux->next = nullptr;
		this->coda.ultimo = aux2;
		delete aux;
		return;
	}
	*/

	aux2->next = aux->next;
	delete aux;
}

Carte::~Carte() {
	nodoCarte* aux = coda.primo;
	nodoCarte* aux2;
	while (aux != nullptr) {
		aux2 = aux->next;
		CancellaCarta(&aux, coda.primo->carta);
		aux = aux2;
	}

	/*
	Carte::Stampa();
	std::cout << "ahaha ho cancellato tutto" << std::endl;
	*/
}

void Carte::Stampa(bool proprietario) {
	nodoCarte* aux = coda.primo;

	int count = 1;

	while (aux != nullptr) {
		std::cout << count << ".\t";
		aux->carta.stampaCarta();
		if (proprietario) {
			std::cout << "\t(giocata da ";
			aux->carta.stampaProprietario();
			std::cout << ") ";
		}
		std::cout << std::endl;
		aux = aux->next;
		count++;
	}
}

int Carte::getSize() {
	int count = 0;
	nodoCarte* aux = coda.primo;

	while (aux != nullptr) {
		count++;
		aux = aux->next;
	}

	return count;
}

void Carte::setCoda(nodoCarte* primo) {
	this->coda.primo = primo;

	
	for (nodoCarte* aux = primo; aux != nullptr; aux = aux->next)
		if (aux->next == nullptr) {
			this->coda.ultimo = aux;
			break;
		}
}

Carta& Carte::getCarta(int n) {
	nodoCarte* aux = this->coda.primo;
	Carta* carta = nullptr;
	int i = 0;
	while (aux != nullptr) {
		if (i == n) {
			carta = new Carta(aux->carta);
			break;
		}
		i++;
		aux = aux->next;
	}

	return *carta;
}

int Carte::totPunti() {
	nodoCarte* aux = this->coda.primo;
	int totale = 0;

	while (aux != nullptr) {
		totale += aux->carta.getPunti();
		aux = aux->next;
	}

	return totale;
}

bool Carte::CartaInLista(Carta& carta) {
	nodoCarte* aux = coda.primo;

	while (aux != nullptr) {
		if (aux->carta == carta)
			return true;
		aux = aux->next;
	}

	return false;
}

Carta* Carte::CercaInLista(Carta& carta) {
	nodoCarte* aux = coda.primo;

	while (aux != nullptr) {
		if (aux->carta == carta)
			return &aux->carta;
		aux = aux->next;
	}

	return nullptr;
}