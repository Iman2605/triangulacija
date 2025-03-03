//---------------------------------------------------------------------------

#pragma hdrstop

#include "pomocna.h"
//---------------------------------------------------------------------------

int Orijentacija(Tacka A, Tacka B, Tacka C) {
	double p = A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y);
	if (p > 0) {
		return -1;
	}
	if (p < 0) {
		return 1;
	}
	return 0;
}

bool Tacka::operator<(Tacka t2) {
	if (this->x == t2.x)
		return this->y < t2.y;
	return this->x < t2.x;
}

bool Tacka::operator==(Tacka t2) {
	return this->x == t2.x && this->y == t2.y;
}

bool Tacka::operator!=(Tacka t2) {
	return !(*this == t2);
	//return this->x == t2.x && this->y == t2.y;
}

double Tacka::operator-(Tacka t2) {
	return sqrt(pow((this->x - t2.x), 2) + pow((this->y - t2.y), 2));
}

void Tacka::Crtaj(TImage* Slika, TColor boja, int velicina) {
	Slika->Canvas->Brush->Color=boja;
	Slika->Canvas->Ellipse(x-velicina, y-velicina, x+velicina, y+velicina);
    Slika->Canvas->Brush->Color=clBlack;
}

void Duz::Crtaj(TImage* Slika, TColor boja, int velicina) {
	Slika->Canvas->Brush->Color=boja;
	Slika->Canvas->Pen->Color=boja;
	// A.Crtaj(Slika, boja, velicina);
	// B.Crtaj(Slika, boja, velicina);

	Slika->Canvas->MoveTo(A.x, A.y);
	Slika->Canvas->LineTo(B.x, B.y);

	Slika->Canvas->Brush->Color=clBlack;
    Slika->Canvas->Pen->Color=clBlack;
}

void Pravougaonik::Crtaj(TImage* Slika, TColor boja, int velicina) {
	Slika->Canvas->Brush->Color=boja;
	Slika->Canvas->Pen->Color=boja;
	Tacka gl(xmin, ymin), dl(xmin, ymax), gd(xmax, ymin), dd(xmax, ymax);

	Slika->Canvas->MoveTo(gl.x, gl.y);
	Slika->Canvas->LineTo(dl.x, dl.y);

	Slika->Canvas->MoveTo(dl.x, dl.y);
	Slika->Canvas->LineTo(dd.x, dd.y);

	Slika->Canvas->MoveTo(dd.x, dd.y);
	Slika->Canvas->LineTo(gd.x, gd.y);

	Slika->Canvas->MoveTo(gd.x, gd.y);
	Slika->Canvas->LineTo(gl.x, gl.y);

	Slika->Canvas->Brush->Color=clBlack;
    Slika->Canvas->Pen->Color=clBlack;
}

bool Duz::horizontalna() {
	return this->A.y == this->B.y;
}

bool Duz::vertikalna() {
	return this->A.x == this->B.x;
}

void iscrtajPoligon(vector<Tacka> &tacke, TImage *Slika) {

	for(int i=0; i < tacke.size(); i++) {
		tacke[i].Crtaj(Slika, clBlue);
		int naredna = (i + 1) % tacke.size();
		Duz(tacke[i], tacke[naredna]).Crtaj(Slika);
	   //	ShowMessage(" ");
	}
}


bool daLiSeDuziSijeku(Duz d1, Duz d2) {
	bool uslov1 = (Orijentacija(d2.A, d2.B, d1.A) != Orijentacija(d2.A, d2.B, d1.B));
	bool uslov2 = (Orijentacija(d1.A, d1.B, d2.A) != Orijentacija(d1.A, d1.B, d2.B));
	return uslov1 && uslov2;
}

bool tackaUTrouglu(Tacka t, Trokut tr) {
	int o1 = Orijentacija(tr.A, tr.B, t);
	int o2 = Orijentacija(tr.B, tr.C, t);
	int o3 = Orijentacija(tr.C, tr.A, t);

	return o1 == o2 && o2 == o3;
}

bool tackaUnutarKonveksnog(Tacka t, vector<Tacka> &konveksni) {
	int p=1, k=konveksni.size()-1;

	while(k-p > 1) {
		int s = (p + k) / 2;
		if (Orijentacija(konveksni[0], konveksni[s], t) > 0) {
			k = s;
		}
		else {
			p = s;
		}
	}

	return tackaUTrouglu(t, Trokut(konveksni[0], konveksni[p], konveksni[k]));
}

bool daLiJeGornjaTangenta(Tacka t, Tacka trenutna, Tacka prije, Tacka poslije) {
	return Orijentacija(t, trenutna, prije) < 0 && Orijentacija(t, trenutna, poslije) < 0;
}

bool daLiJeDonjaTangenta(Tacka t, Tacka trenutna, Tacka prije, Tacka poslije) {
	return Orijentacija(t, trenutna, prije) > 0 && Orijentacija(t, trenutna, poslije) > 0;
}

bool daLiJeNaVecemLuku(Tacka t, Tacka trenutna, Tacka poslije) {
	if (Orijentacija(t, trenutna, poslije) > 0) {
		return false;
	}
	return true;
}

pair<int, int> nadjiTangente(Tacka t, vector<Tacka> &konveksni) {
	int n = konveksni.size();
	int p=0, k=n-1;

	int donja=-1, gornja=-1;

	while(k - p > 1) {
		int s=(p+k)/2;
		int poslije=(s+1)%n;
		int prije=(s+n-1)%n;

		if(daLiJeGornjaTangenta(t, konveksni[s], konveksni[prije], konveksni[poslije])) {
			gornja = s;
            break;
		}
		// sredina je na vecem luku
		if (daLiJeNaVecemLuku(t, konveksni[s], konveksni[poslije])) {
			// prvi slucaj (crveni)
			if (Orijentacija(t, konveksni[s], konveksni[p]) > 0 && Orijentacija(t, konveksni[p], konveksni[(p+n-1)%n]) > 0) {
				p = s;
			}
			// drugi slucaj (zeleni)
			else {
				k = s;
			}
		}
		// sredina je manjem luku
		else {
			// prvi slucaj (zeleni)
			if (Orijentacija(t, konveksni[s], konveksni[p]) > 0 && Orijentacija(t, konveksni[p], konveksni[(p+n-1)%n]) < 0) {
				k = s;
			}
			// drugi slucaj (crveni)
			else {
				p = s;
			}
		}
	}

	if (gornja == -1) {
		if (daLiJeGornjaTangenta(t, konveksni[p], konveksni[(p+n-1)%n], konveksni[(p+1)%n])) {
			gornja = p;
		}
		else {
			gornja = k;
		}
	}

	p = 0;
	k = n-1;

	while(k - p > 1) {
		int s=(p+k)/2;
		int poslije=(s+1)%n;
		int prije=(s+n-1)%n;

		if(daLiJeDonjaTangenta(t, konveksni[s], konveksni[prije], konveksni[poslije])) {
			donja = s;
			break;
		}
		// sredina je na vecem luku
		if (daLiJeNaVecemLuku(t, konveksni[s], konveksni[poslije])) {
			// prvi slucaj (crveni)
			if (Orijentacija(t, konveksni[s], konveksni[p]) < 0 && Orijentacija(t, konveksni[p], konveksni[(p+n-1)%n]) > 0) {
				k = s;
			}
			// drugi slucaj (zeleni)
			else {
				p = s;
			}
		}
		// sredina je manjem luku
		else {
			// prvi slucaj (zeleni)
			if (Orijentacija(t, konveksni[s], konveksni[p]) < 0 && Orijentacija(t, konveksni[p], konveksni[(p+n-1)%n]) < 0) {
				p = s;
			}
			// drugi slucaj (crveni)
			else {
				k = s;
			}
		}
	}

	if (donja == -1) {
		if (daLiJeDonjaTangenta(t, konveksni[p], konveksni[(p+n-1)%n], konveksni[(p+1)%n])) {
			donja = p;
		}
		else {
			donja = k;
		}
	}

	return {donja, gornja};
}

void pomjeriIteratorNaprijed(list<int>::iterator &it, list<int> &lista) {
	it++;
	if (it == lista.end()) {
		it = lista.begin();
	}
}

void pomjeriIteratorNazad(list<int>::iterator &it, list<int> &lista) {
	if (it == lista.begin()) {
		it = --lista.end();
	}
	else {
		it--;
	}
}

void GrahamScan(vector<Tacka> tacke, vector<Tacka>& konveksni_omotac) {
	if (tacke.size() < 3) {
		return;
	}

	for (int i = 1; i < tacke.size(); i++) {
		if (tacke[i] < tacke[0]) {
			swap(tacke[0], tacke[i]);
		}
	}

	Tacka lijeva = tacke[0];
	konveksni_omotac.push_back(lijeva);

	sort(tacke.begin() + 1, tacke.end(), [lijeva](Tacka A, Tacka B) { return Orijentacija(lijeva, A, B) < 0; });
	konveksni_omotac.push_back(tacke[1]);

	for(int i = 2; i < tacke.size(); i++) {
		int k = konveksni_omotac.size();
		Tacka prethodna(konveksni_omotac[k-2]), trenutna(konveksni_omotac[k-1]);
		Tacka sljedeca(tacke[i]);

		while(Orijentacija(prethodna, trenutna, sljedeca) > 0) {
			konveksni_omotac.pop_back();
			int k = konveksni_omotac.size();
			prethodna = konveksni_omotac[k-2];
			trenutna = konveksni_omotac[k-1];
		}

		konveksni_omotac.push_back(sljedeca);
	}
}


Tacka presjekPravih(Duz d1, Duz d2) {
	Tacka A=d1.A, B=d1.B, C=d2.A, D=d2.B;

	// Line AB represented as a1x + b1y = c1
	double a1 = B.y - A.y;
	double b1 = A.x - B.x;
	double c1 = a1*(A.x) + b1*(A.y);

	// Line CD represented as a2x + b2y = c2
	double a2 = D.y - C.y;
	double b2 = C.x - D.x;
	double c2 = a2*(C.x)+ b2*(C.y);

	double determinant = a1*b2 - a2*b1;

	if (determinant == 0)
	{
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
		return Tacka(FLT_MAX, FLT_MAX);
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
		return Tacka(x, y);
	}
}

bool izmedju(int x, int xmin, int xmax) {
    return x >= xmin && x <= xmax;
}

TipPresjeka presjekPravougaonika(Pravougaonik p1, Pravougaonik p2) {

	int min_lijeva = min(p1.xmin, p2.xmin);
	int max_lijeva = max(p1.xmin, p2.xmin);
	int min_desna = min(p1.xmax, p2.xmax);
	int max_desna = max(p1.xmax, p2.xmax);

	if (max_lijeva > min_desna) {
		return PRAZAN;
	}

	int min_gornja = min(p1.ymin, p2.ymin);
	int max_gornja = max(p1.ymin, p2.ymin);
	int min_donja = min(p1.ymax, p2.ymax);
	int max_donja = max(p1.ymax, p2.ymax);

	if (max_gornja > min_donja) {
		return PRAZAN;
	}

	// provjera da li je p1 unutar p2
	if (!izmedju(p1.xmin, p2.xmin, p2.xmax) ||
		!izmedju(p1.xmax, p2.xmin, p2.xmax) ||
		!izmedju(p1.ymin, p2.ymin, p2.ymax) ||
		!izmedju(p1.ymax, p2.ymin, p2.ymax)) {
        return DJELIMICNI;
	}
    return POTPUNI;
}

bool tackaUnutarPravougaonika(Tacka t, Pravougaonik p) {
	return izmedju(t.x, p.xmin, p.xmax) && izmedju(t.y, p.ymin, p.ymax);
}

KDStablo::KDStablo(vector<Tacka>& tacke, int duzina, int sirina) {
	int n = tacke.size();
	if(n == 0) {
		return;
	}
	if(n == 1) {
		korijen = new KDCvor(nullptr, LIST, Pravougaonik(0, duzina, 0, sirina), nullptr, nullptr, tacke[0]);
	}
	else {
		sort(tacke.begin(), tacke.end());
		int sredina = (n-1) / 2;
		vector<Tacka> lijeve(sredina + 1);
		vector<Tacka> desne(n - sredina - 1);

		copy(tacke.begin(), tacke.begin() + sredina + 1, lijeve.begin());
		copy(tacke.begin() + sredina + 1, tacke.end(), desne.begin());

		korijen = new KDCvor(nullptr, VERTIKALNI, Pravougaonik(0, duzina, 0, sirina), nullptr, nullptr, tacke[sredina]);
		KDCvor* lijevo = napravi_stablo(korijen, lijeve, true);
		KDCvor* desno = napravi_stablo(korijen, desne, false);

		korijen->lijevo = lijevo;
		korijen->desno = desno;
	}
}

KDCvor* KDStablo::napravi_stablo(KDCvor* roditelj, vector<Tacka>& tacke, bool lijevo_dijete) {
	int n = tacke.size();
	KDCvor* cvor;

	if(n == 0) {
		return nullptr;
	}

	Pravougaonik regija_roditelj = roditelj->regija;
	Pravougaonik* regija;
	TipCvora tip;

	if(roditelj->tip == VERTIKALNI) {
        tip = HORIZONTALNI;
		if(lijevo_dijete) {
			regija = new Pravougaonik(regija_roditelj.xmin, roditelj->tacka.x, regija_roditelj.ymin, regija_roditelj.ymax);
		}
		else {
			regija = new Pravougaonik(roditelj->tacka.x, regija_roditelj.xmax, regija_roditelj.ymin, regija_roditelj.ymax);
		}
	}
	else {
    	tip = VERTIKALNI;
		if(lijevo_dijete) {
			regija = new Pravougaonik(regija_roditelj.xmin, regija_roditelj.xmax, regija_roditelj.ymin, roditelj->tacka.y);
		}
		else {
			regija = new Pravougaonik(regija_roditelj.xmin, regija_roditelj.xmax, roditelj->tacka.y, regija_roditelj.ymax);
		}
	}

	if(n == 1) {
		cvor = new KDCvor(roditelj, LIST, *regija, nullptr, nullptr, tacke[0]);
		return cvor;
	}


	if (tip == VERTIKALNI) {
		sort(tacke.begin(), tacke.end());
	}
	else {
		sort(tacke.begin(), tacke.end(), [](Tacka t1, Tacka t2) {return t1.y < t2.y;});
	}

	int sredina = (n-1) / 2;
	vector<Tacka> lijeve(sredina + 1);
	vector<Tacka> desne(n - sredina - 1);

	copy(tacke.begin(), tacke.begin() + sredina + 1, lijeve.begin());
	copy(tacke.begin() + sredina + 1, tacke.end(), desne.begin());

	cvor = new KDCvor(nullptr, tip, *regija, nullptr, nullptr, tacke[sredina]);
	KDCvor* lijevo = napravi_stablo(cvor, lijeve, true);
	KDCvor* desno = napravi_stablo(cvor, desne, false);

	cvor->lijevo = lijevo;
	cvor->desno = desno;
    return cvor;
}

void KDStablo::nacrtajPravu(TImage* Slika, KDCvor* cvor) {
	if (cvor) {
        if (cvor->tip == VERTIKALNI) {
			int x = cvor->tacka.x;
			int ymin = cvor->regija.ymin;
			int ymax = cvor->regija.ymax;

			Duz d(Tacka(x, ymin), Tacka(x, ymax));
			d.Crtaj(Slika, clBlue);
		}
		else if(cvor->tip == HORIZONTALNI) {
			int y = cvor->tacka.y;
			int xmin = cvor->regija.xmin;
			int xmax = cvor->regija.xmax;

			Duz d(Tacka(xmin, y), Tacka(xmax, y));
			d.Crtaj(Slika, clBlue);
		}
		else {
            cvor->tacka.Crtaj(Slika, clRed, 4);
		}
	}

}

void KDStablo::iscrtaj(TImage* Slika, KDCvor* cvor, bool pocetak) {

	if (pocetak) {
		cvor = korijen;
	}
	if (cvor == nullptr) {
		return;
	}

	nacrtajPravu(Slika, cvor);
	iscrtaj(Slika, cvor->lijevo, false);
	iscrtaj(Slika, cvor->desno, false);
}

void KDStablo::dodajListove(KDCvor* cvor, vector<Tacka> &tacke) {
	if (cvor) {
		tacke.push_back(cvor->tacka);
		dodajListove(cvor->lijevo, tacke);
		dodajListove(cvor->desno, tacke);
	}
}

void KDStablo::query(Pravougaonik p, vector<Tacka> &query_tacke, KDCvor* cvor, bool pocetak) {
	if (pocetak) {
		cvor = korijen;
	}
	if (cvor == nullptr) {
		return;
	}

	if(cvor->tip == LIST) {
		if (tackaUnutarPravougaonika(cvor->tacka, p)) {
            query_tacke.push_back(cvor->tacka);
		}
        return;
	}
	if(cvor->lijevo) {
		TipPresjeka tip = presjekPravougaonika(cvor->lijevo->regija, p);
		if (tip == DJELIMICNI) {
			query(p, query_tacke, cvor->lijevo, false);
		}
		else if (tip == POTPUNI) {
			dodajListove(cvor->lijevo, query_tacke);
            query_tacke.push_back(cvor->tacka);
		}
	}

	if(cvor->desno) {
		TipPresjeka tip = presjekPravougaonika(cvor->desno->regija, p);
		if (tip == DJELIMICNI) {
			query(p, query_tacke, cvor->desno, false);
		}
		else if (tip == POTPUNI) {
			dodajListove(cvor->desno, query_tacke);
		}
//		else {
//        	ShowMessage("NEMA PRESJEKA");
//        }
	}

}

//---------------------------------------------------------------------------

//PROJEKAT

//---------------------------------------------------------------------------

pair<int, int> nadjiTangente(Tacka t, deque<Tacka>& konveksni) {
    int n = konveksni.size();
    int p = 0, k = n - 1;

    int donja = -1, gornja = -1;

    while (k - p > 1) {
        int s = (p + k) / 2;
        int poslije = (s + 1) % n;
        int prije = (s + n - 1) % n;

        if (daLiJeGornjaTangenta(t, konveksni[s], konveksni[prije], konveksni[poslije])) {
            gornja = s;
            break;
        }
        // sredina je na vecem luku
        if (daLiJeNaVecemLuku(t, konveksni[s], konveksni[poslije])) {
            // prvi slucaj (crveni)
            if (Orijentacija(t, konveksni[s], konveksni[p]) > 0 && Orijentacija(t, konveksni[p], konveksni[(p + n - 1) % n]) > 0) {
                p = s;
            }
            // drugi slucaj (zeleni)
            else {
                k = s;
            }
        }
        // sredina je manjem luku
        else {
            // prvi slucaj (zeleni)
            if (Orijentacija(t, konveksni[s], konveksni[p]) > 0 && Orijentacija(t, konveksni[p], konveksni[(p + n - 1) % n]) < 0) {
                k = s;
            }
            // drugi slucaj (crveni)
            else {
                p = s;
            }
        }
    }

    if (gornja == -1) {
        if (daLiJeGornjaTangenta(t, konveksni[p], konveksni[(p + n - 1) % n], konveksni[(p + 1) % n])) {
            gornja = p;
        }
        else {
            gornja = k;
        }
    }

    p = 0;
    k = n - 1;

    while (k - p > 1) {
        int s = (p + k) / 2;
        int poslije = (s + 1) % n;
        int prije = (s + n - 1) % n;

        if (daLiJeDonjaTangenta(t, konveksni[s], konveksni[prije], konveksni[poslije])) {
            donja = s;
            break;
        }
        // sredina je na vecem luku
        if (daLiJeNaVecemLuku(t, konveksni[s], konveksni[poslije])) {
            // prvi slucaj (crveni)
            if (Orijentacija(t, konveksni[s], konveksni[p]) < 0 && Orijentacija(t, konveksni[p], konveksni[(p + n - 1) % n]) > 0) {
                k = s;
            }
            // drugi slucaj (zeleni)
            else {
                p = s;
            }
        }
        // sredina je manjem luku
        else {
            // prvi slucaj (zeleni)
            if (Orijentacija(t, konveksni[s], konveksni[p]) < 0 && Orijentacija(t, konveksni[p], konveksni[(p + n - 1) % n]) < 0) {
                p = s;
            }
            // drugi slucaj (crveni)
            else {
                k = s;
            }
        }
    }

    if (donja == -1) {
        if (daLiJeDonjaTangenta(t, konveksni[p], konveksni[(p + n - 1) % n], konveksni[(p + 1) % n])) {
            donja = p;
        }
        else {
            donja = k;
        }
    }

    return { donja, gornja };
}


void triangulacijaTacakaRekurzivno(std::vector<Tacka> tacke, std::vector<Duz>& stranice) {
	deque<Tacka> konveksni_omotac;

	if (tacke.size() < 3) {
		return;
	}

	std::sort(tacke.begin(), tacke.end());

	konveksni_omotac = {tacke[0], tacke[1], tacke[2]};

	stranice.push_back(Duz(tacke[0], tacke[1]));
	stranice.push_back(Duz(tacke[1], tacke[2]));
	stranice.push_back(Duz(tacke[0], tacke[2]));

	if (Orijentacija(tacke[0], tacke[1], tacke[2]) > 0) {
		std::swap(*next(konveksni_omotac.begin()), *prev(konveksni_omotac.end()));
	}

	for (int i = 3; i < tacke.size(); i++) {
		Tacka trenutna(tacke[i]);

		auto tangente = nadjiTangente(trenutna, konveksni_omotac);
		int donja = tangente.first;
        int gornja = tangente.second;

		stranice.push_back(Duz(trenutna, *next(konveksni_omotac.begin(), donja)));
		stranice.push_back(Duz(trenutna, *next(konveksni_omotac.begin(), gornja)));

        if (donja > gornja) {
            for (auto it = std::next(konveksni_omotac.begin(), donja + 1); it != konveksni_omotac.end(); ++it) {
                stranice.push_back(Duz(trenutna, *it));
            }
			konveksni_omotac.erase(next(konveksni_omotac.begin(), donja + 1), konveksni_omotac.end());

			if (gornja > 0) {
				for (auto it = konveksni_omotac.begin(); it != next(konveksni_omotac.begin(), gornja); ++it) {
					stranice.push_back(Duz(trenutna, *it));
                }
				konveksni_omotac.erase(konveksni_omotac.begin(), next(konveksni_omotac.begin(), gornja));
            }

			konveksni_omotac.insert(konveksni_omotac.begin(), trenutna);
		} else {
			for (auto it = next(konveksni_omotac.begin(), donja + 1); it != next(konveksni_omotac.begin(), gornja); ++it) {
                stranice.push_back(Duz(trenutna, *it));
            }
			konveksni_omotac.erase(next(konveksni_omotac.begin(), donja + 1), next(konveksni_omotac.begin(), gornja));
			konveksni_omotac.insert(next(konveksni_omotac.begin(), donja + 1), trenutna);
		}
    }
}


#pragma package(smart_init)
