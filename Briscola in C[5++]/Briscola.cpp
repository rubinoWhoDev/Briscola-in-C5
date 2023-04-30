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

Carte Giocatore::getMano() {
	return mano;
}

void Giocatore::stampaNome() {
	std::cout << nome << std::endl;
}

void Giocatore::stampaMano() {
	mano.Stampa();
}

Giocatore::~Giocatore() {
	mano.~Carte();
	delete[] nome;
}

void inizializzaGiocatori(Giocatore**& giocatori) {
	giocatori = new Giocatore * [5];
	for (int i = 0; i < 5; i++)
		giocatori[i] = new Giocatore(i + 1);
}