#pragma once
//#include "Briscola.h"

enum Segno {
	ORO,
	MAZZE,
	COPPE,
	SPADE
};

class Carta {
private:
	Segno segno;
	unsigned int valore;
	unsigned int punti;
	//Giocatore* proprietario;
	char proprietario[30];
public:
	Carta(int segno, unsigned int val);
	Carta(Carta& c);
	void stampaCarta();
	const char* valoreToString();
	const char* segnoToString();
	void setProprietario(const char nome_proprietario[30]);
	void stampaProprietario();
	Segno getSegno() { return this->segno; }
	unsigned int getValore() { return this->valore; }
	unsigned int getPunti() { return this->punti; }
	bool operator ==(Carta c);
};

struct nodoCarte {
	nodoCarte(Carta& c);
	Carta carta;
	struct nodoCarte* next;
};

struct codaCarte {
	struct nodoCarte* primo;
	struct nodoCarte* ultimo;
};

class Carte {
private:
	codaCarte coda;
public:
	Carte(int num_carte = 0);
	Carte(Carta& carta);
	Carte(Carte& carte);
	~Carte();
	void CancellaCarta(nodoCarte** coda, Carta& carta);
	void AggiungiInCoda(Carta& carta);
	void Stampa();
	Carta& PrendiCarta(Carta& carta);
	Carta& PrendiCarta(int n);
	int getSize();
	nodoCarte*& getPrimo() { return this->coda.primo; }
	//codaCarte getCoda() { return this->coda; }
	void setCoda(nodoCarte* primo);
	//void operator=(Carte c);
};