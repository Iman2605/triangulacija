//---------------------------------------------------------------------------

#ifndef pomocnaH
#define pomocnaH
#include <algorithm>
#include <utility>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <unordered_set>
#include <math.h>
using namespace std;
#define PI 3.14159265
//---------------------------------------------------------------------------


struct Tacka {
	double x, y;
    Tacka(): x(0), y(0) {}
	Tacka(double x, double y): x(x), y(y) {}
	bool operator<(Tacka);
	bool operator==(Tacka);
    bool operator!=(Tacka);
	double operator-(Tacka);
	void Crtaj(TImage*, TColor = clBlack, int = 3);
};

int Orijentacija(Tacka, Tacka, Tacka);


struct Duz {
	Tacka A, B;
	Duz(Tacka a, Tacka b): A(a), B(b) {
		if (B < A) {
			swap(A, B);
		}
	}
	void Crtaj(TImage*, TColor = clBlack, int velicina = 3);
	bool horizontalna();
	bool vertikalna();
};

struct Trokut {
	Tacka A, B, C;
	Trokut(Tacka a, Tacka b, Tacka c): A(a), B(b), C(c) {
		if (Orijentacija(A, B, C) < 0) {
			swap(B, C);
		}
	}
};


enum TipPresjeka {PRAZAN, POTPUNI, DJELIMICNI};

struct Pravougaonik {
	int xmin, xmax, ymin, ymax;
	Pravougaonik(int xmin, int xmax, int ymin, int ymax): xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax) {}
    void Crtaj(TImage*, TColor = clBlack, int velicina = 3);
};

void iscrtajPoligon(vector<Tacka>&, TImage*);
bool daLiSeDuziSijeku(Duz, Duz);
bool tackaUTrouglu(Tacka, Trokut);
bool tackaUnutarKonveksnog(Tacka, vector<Tacka>&);
pair<int, int> nadjiTangente(Tacka, vector<Tacka>&);
void pomjeriIteratorNaprijed(list<int>::iterator&, list<int>&);
void pomjeriIteratorNazad(list<int>::iterator&, list<int>&);
Tacka presjekPravih(Duz, Duz);
void GrahamScan(vector<Tacka>, vector<Tacka>&);

struct HorVerDuziPoX {
	bool operator()(pair<Duz*, Tacka> d1, pair<Duz*, Tacka> d2) const {
		return d2.second < d1.second;
	}
};

struct AktivneHorVerPoY {
	bool operator()(Duz* d1, Duz *d2) const {
		if (d1->A.y == d2->A.y) {
			return d1->A.x < d2->A.x;
		}
		return d1->A.y < d2->A.y;
	}
};

struct DuziPoX {
	bool operator()(pair<Tacka, pair<Duz*, Duz*>> e1, pair<Tacka, pair<Duz*, Duz*>> e2) const {
		return e2.first < e1.first;
	}
};

struct DuziPoY {
	bool operator()(Duz* d1, Duz *d2) const {
		if (d1->A == d2->A) {
			return Orijentacija(d1->A, d1->B, d2->B) < 0;
		}
		if(d1->A < d2->A) {
			return Orijentacija(d1->A, d1->B, d2->A) < 0;
		}
		else {
			return Orijentacija(d2->A, d2->B, d1->A) > 0;
        }
	}
};

void obradi_presjek(int x_sweep_line, Duz *d1, Duz *d2, vector<Tacka> &presjeci, priority_queue<pair<Tacka, pair<Duz*, Duz*>>, vector<pair<Tacka, pair<Duz*, Duz*>>>, DuziPoX> &eventi) {
	if (daLiSeDuziSijeku(*d1, *d2)) {
		Tacka presjek = presjekPravih(*d1, *d2);

		if (presjek.x >= x_sweep_line) {
			presjeci.push_back(presjek);
			eventi.push({presjek, {d1, d2}});
		}
	}
}

enum TipCvora {HORIZONTALNI, VERTIKALNI, LIST};

class KDCvor {
	KDCvor *roditelj;
	KDCvor *lijevo, *desno;
	TipCvora tip;
	Pravougaonik regija;
	Tacka tacka;
	friend class KDStablo;
public:
	KDCvor(KDCvor* roditelj, TipCvora tip, Pravougaonik regija, KDCvor* lijevo, KDCvor* desno,  Tacka t): roditelj(roditelj), lijevo(lijevo), desno(desno), tip(tip), tacka(t), regija(regija) {}
};

class KDStablo {
	KDCvor* korijen;
	KDCvor* napravi_stablo(KDCvor*, vector<Tacka>&, bool);
	void nacrtajPravu(TImage*, KDCvor*);
    void dodajListove(KDCvor*, vector<Tacka>&);
public:
	KDStablo(vector<Tacka>&, int, int);
	void iscrtaj(TImage*, KDCvor* = nullptr, bool = true);
	void query(Pravougaonik, vector<Tacka> &query_tacke, KDCvor* = nullptr, bool = true);
};

//---------------------------------------------------------------------------
//PROJEKAT
//---------------------------------------------------------------------------

pair<int, int> nadjiTangente(Tacka, deque<Tacka>& );
void triangulacijaTacakaRekurzivno(vector<Tacka> , vector<Duz> &);

#endif
