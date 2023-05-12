#pragma once
//#include "Briscola.h"
#include <string>


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
	std::string proprietario;
public:
	Carta(int segno, unsigned int val);
	Carta(Carta& c);
	void stampaCarta();
	std::string valoreToString();
	std::string segnoToString();
	void setProprietario(std::string nome_proprietario);
	std::string getProprietario() { return proprietario; }
	void stampaProprietario();
	Segno getSegno() { return this->segno; }
	unsigned int getValore() { return this->valore; }
	unsigned int getPunti() { return this->punti; }
	bool operator ==(Carta& c);
	//void operator =(Carta& c);
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
	void Stampa(bool proprietario = false);
	Carta& getCarta(int n);
	Carta& PrendiCarta(Carta& carta);
	Carta& PrendiCarta(int n);
	bool CartaInLista(Carta& carta);
	Carta* CercaInLista(Carta& carta);
	int getSize();
	nodoCarte*& getPrimo() { return this->coda.primo; }
	//codaCarte getCoda() { return this->coda; }
	void setCoda(nodoCarte* primo);
	//void operator=(Carte c);
	int totPunti();
};