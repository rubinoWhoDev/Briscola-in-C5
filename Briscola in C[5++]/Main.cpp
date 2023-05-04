#include <iostream>
#include "Carte.h"
#include "Briscola.h"

using namespace std;

int main() {
	Giocatore** giocatori = nullptr;
	int punteggioMinimo = 74;
	int primoAGiocare = Random(0, 4);

	punteggioMinimo = InizioGioco(giocatori, punteggioMinimo);
	cout << giocatori[WhoIsChiamante(giocatori)]->getNome() << " e' il chiamante e ha chiamato " << punteggioMinimo << endl;

	delete[] giocatori;
	
	return 0;
}