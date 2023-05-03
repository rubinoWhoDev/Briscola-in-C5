#pragma once
#include "Carte.h"

class Giocatore {
private:
	char nome[30];
	Carte mano;
	unsigned punteggio;
	bool isChiamante;
	bool isCompagno;

public:
	Giocatore(int n);
	~Giocatore();
	void setName(int n);
	void stampaNome();
	void Pesca(Carte& mazzo, int num_carte);
	void stampaMano();
	Carte& getMano();
	void OrdinaCarte();
	const char* getNome() { return nome; }
	//void setMano(Carte nuovaMano) { mano = nuovaMano; }
	bool chiamante() { return isChiamante; }
	void setChiamante(bool b) { isChiamante = b; }
};

void inizializzaGiocatori(Giocatore**& giocatori);
int Random(int min, int max);
bool TuttiLisci(Giocatore**& giocatori);
void StampaManoGiocatori(Giocatore**& giocatori);
int GiroChiamanti(Giocatore**& giocatori, int punteggioMinimo);