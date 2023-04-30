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
	std::cout << valoreToString() << " di " << segnoToString() << std::endl;
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
	std::cout << proprietario << std::endl;
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

bool Carta::operator==(Carta c) {
	return (this->segno == c.segno && this->valore == c.valore);
}

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

void Carte::Stampa() {
	nodoCarte* aux = coda.primo;

	while (aux != nullptr) {
		aux->carta.stampaCarta();
		aux = aux->next;
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