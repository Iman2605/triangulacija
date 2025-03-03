//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "prozor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	zavrsen_poligon = false;
	Slika->Canvas->Brush->Color=clWhite;
	Slika->Canvas->FillRect(TRect(0,0,Slika->Width,Slika->Height));
	Slika->Canvas->Brush->Color=clBlack;
	Slika->Canvas->FrameRect(TRect(0,0,Slika->Width,Slika->Height));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SlikaMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	Tacka nova = Tacka(X, Y);

	if (RadioDodajDuz->Checked) {
		if (drugi_klik) {
			Duz nova_duz(tacke[tacke.size()-1], nova);
			nova_duz.Crtaj(Slika);
            duzi.push_back(nova_duz);
		}
		drugi_klik = !drugi_klik;
	}
	else if (RadioTackaUnutarKonveksnog->Checked) {
		if(konveksni_omotac.size() != 0) {
			if (tackaUnutarKonveksnog(nova, konveksni_omotac)) {
				ShowMessage("Tacka je unutra!");
			}
			else {
				ShowMessage("Tacka je van!");
			}
		}
	}
	else if (RadioTangente->Checked) {
		if(konveksni_omotac.size() != 0) {
			if (!tackaUnutarKonveksnog(nova, konveksni_omotac)) {
				pair<int, int> tangente = nadjiTangente(nova, konveksni_omotac);
				int donja = tangente.first;
				int gornja = tangente.second;

				Duz(nova, konveksni_omotac[gornja]).Crtaj(Slika);
				Duz(nova, konveksni_omotac[donja]).Crtaj(Slika);
			}
		}
	}
	else if (RadioCrtajPoligon->Checked) {
		if(poligon.size() > 2 && (nova - poligon[0]) < 10) {
			Duz d = Duz(poligon[poligon.size() - 1], poligon[0]);
			d.Crtaj(Slika);
			zavrsen_poligon = true;
		}
		else {
			if (zavrsen_poligon) {
				poligon.clear();
                dijagonale.clear();
                zavrsen_poligon = false;
			}
			poligon.push_back(nova);
			int n = poligon.size();
			if(n > 1) {
				Duz d = Duz(poligon[n-1], poligon[n-2]);
				d.Crtaj(Slika);
			}
			nova.Crtaj(Slika, clBlue);
		}
		return;
	}

	tacke.push_back(nova);
	nova.Crtaj(Slika, clBlue);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonProstiMnogougaoClick(TObject *Sender)
{
	for (int i = 1; i < tacke.size(); i++) {
		if (tacke[i] < tacke[0]) {
			swap(tacke[0], tacke[i]);
		}
	}
	Tacka lijeva(tacke[0]);
	sort(tacke.begin() + 1, tacke.end(), [lijeva](Tacka A, Tacka B) { return Orijentacija(lijeva, A, B) < 0; });
	iscrtajPoligon(tacke, Slika);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonOcistiClick(TObject *Sender)
{
	Slika->Canvas->Brush->Color=clWhite;
	Slika->Canvas->FillRect(TRect(0,0,Slika->Width,Slika->Height));
	Slika->Canvas->Brush->Color=clBlack;
	Slika->Canvas->FrameRect(TRect(0,0,Slika->Width,Slika->Height));
	tacke.clear();
	konveksni_omotac.clear();
	poligon.clear();
	zavrsen_poligon = false;
	dijagonale.clear();
	duzi.clear();
	stranice.clear();
    slojevi.clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonGenerisiTackeClick(TObject *Sender)
{
	int broj_tacaka = EditBrojTacaka->Text.ToInt();
	for (int i = 0; i < broj_tacaka; i++) {
		int x = rand() % Slika->Width;
		int y = rand() % Slika->Height;
		Tacka nova(x, y);
		nova.Crtaj(Slika, clBlue);
		tacke.push_back(nova);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonPresjekDuziClick(TObject *Sender)
{
	int n(duzi.size());

	if (n < 2) {
		return;
	}

	Duz duz1(duzi[n-1]), duz2(duzi[n-2]);
	if (daLiSeDuziSijeku(duz1, duz2)) {
		ShowMessage("Duzi se sijeku!");
	}
	else {
        ShowMessage("Duzi se ne sijeku!");
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonUvijanjePoklonaClick(TObject *Sender)
{
	if (tacke.size() < 3) {
		return;
	}
	for (int i = 1; i < tacke.size(); i++) {
		if (tacke[i] < tacke[0]) {
			swap(tacke[0], tacke[i]);
		}
	}
	Tacka lijeva(tacke[0]);
	konveksni_omotac.push_back(lijeva);

	while(true) {
		int iduca(0);
        Tacka zadnja_dodana(konveksni_omotac[konveksni_omotac.size()-1]);

		if (tacke[iduca] == zadnja_dodana) {
			iduca = 1;
		}
		for (int i = 0; i < tacke.size(); i++) {
			if (zadnja_dodana == tacke[i]) {
				continue;
			}
			if(Orijentacija(zadnja_dodana, tacke[iduca], tacke[i]) > 0) {
				iduca = i;
			}
		}
		if (iduca == 0) {
			break;
		}
		konveksni_omotac.push_back(tacke[iduca]);
	}
	string poruka = "Broj tacaka na konveksnom: " + to_string(konveksni_omotac.size());
	ShowMessage(poruka.c_str());
	iscrtajPoligon(konveksni_omotac, Slika);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ButtomGrahamScanClick(TObject *Sender)
{
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

	sort(tacke.begin() + 1, tacke.end(), [lijeva](Tacka A, Tacka B) { return Orijentacija(lijeva, A, B) > 0; });
	konveksni_omotac.push_back(tacke[1]);

	for(int i = 2; i < tacke.size(); i++) {
		int k = konveksni_omotac.size();
		Tacka prethodna(konveksni_omotac[k-2]), trenutna(konveksni_omotac[k-1]);
		Tacka sljedeca(tacke[i]);

		while(Orijentacija(prethodna, trenutna, sljedeca) < 0) {
			konveksni_omotac.pop_back();
			int k = konveksni_omotac.size();
			prethodna = konveksni_omotac[k-2];
			trenutna = konveksni_omotac[k-1];
		}

		konveksni_omotac.push_back(sljedeca);
	}

	//string poruka = "Broj tacaka na konveksnom: " + to_string(konveksni_omotac.size());
	//ShowMessage(poruka.c_str());
	iscrtajPoligon(konveksni_omotac, Slika);
}


//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonKonveksniInduktivnoClick(TObject *Sender)
{
	if (tacke.size() < 3) {
		return;
	}
	konveksni_omotac = {tacke[0], tacke[1], tacke[2]};
	if (Orijentacija(tacke[0], tacke[1], tacke[2]) > 0) {
		swap(konveksni_omotac[1], konveksni_omotac[2]);
	}

	for(int i=3; i<tacke.size(); i++) {
		Tacka trenutna(tacke[i]);

		if (tackaUnutarKonveksnog(trenutna, konveksni_omotac)) {
			continue;
		}

		pair<int, int> tangente = nadjiTangente(trenutna, konveksni_omotac);
		int donja = tangente.first;
		int gornja = tangente.second;

		if (donja > gornja) {
			konveksni_omotac.erase(konveksni_omotac.begin() + donja + 1, konveksni_omotac.end());
			if (gornja > 0) {
				konveksni_omotac.erase(konveksni_omotac.begin(), konveksni_omotac.begin() + gornja);
			}
			konveksni_omotac.insert(konveksni_omotac.begin(), trenutna);
		}
		else {
			konveksni_omotac.erase(konveksni_omotac.begin() + donja + 1, konveksni_omotac.begin() + gornja);
			konveksni_omotac.insert(konveksni_omotac.begin() + donja + 1, trenutna);
		}
	}
	string poruka = "Broj tacaka na konveksnom: " + to_string(konveksni_omotac.size());
	ShowMessage(poruka.c_str());
	iscrtajPoligon(konveksni_omotac, Slika);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonTriangulacijaClick(TObject *Sender)
{
	if (poligon.size() < 3) {
		return;
	}
	int velicina = poligon.size();
	list<int> lista;

	for(int i=0; i<velicina; i++)
		lista.push_back(i);

	int broj_trouglova = 0;

	auto p = lista.begin();
	auto t = ++lista.begin();
	auto n = ++(++lista.begin());

	while(lista.size() > 3) {
		if (Orijentacija(poligon[*p], poligon[*t], poligon[*n]) < 0) {
			// dobra orijentacija, provjeri da li je ijedna tacka u trouglu
			bool uho = true;
			auto i = n;
			pomjeriIteratorNaprijed(i, lista);
			while(i != p) {
				if(tackaUTrouglu(poligon[*i], Trokut(poligon[*p], poligon[*t], poligon[*n]))) {
					uho = false;
					break;
				}
                pomjeriIteratorNaprijed(i, lista);
			}

			if(uho) {
				dijagonale.push_back({*p, *n});
				lista.erase(t);
				t = p;
				pomjeriIteratorNazad(p, lista);
				continue;
			}
		}
		pomjeriIteratorNaprijed(p, lista);
		pomjeriIteratorNaprijed(t, lista);
		pomjeriIteratorNaprijed(n, lista);
	}
	for(auto par: dijagonale) {
		Tacka t1 = poligon[par.first];
		Tacka t2 = poligon[par.second];
		Duz(t1, t2).Crtaj(Slika);
	}
    string poruka = "Broj trouglova: " + to_string(dijagonale.size() + 1);
	ShowMessage(poruka.c_str());

}
//---------------------------------------------------------------------------

void __fastcall TForm1::SlikaMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
	string koordinate = "X: " + to_string(X) + " Y: " + to_string(Y);
    TextKoordinate->Text = koordinate.c_str();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonGenerisiHorVerDuziClick(TObject *Sender)
{
	int broj_duzi = EditBrojTacaka->Text.ToInt();
	for (int i = 0; i < broj_duzi; i++) {
		int x1,x2,y1,y2;
		if (rand() % 2 == 0) { // horizontalna
			x1 = rand() % Slika->Width;
			x2 = rand() % Slika->Width;
			y1 = rand() % Slika->Height;
			y2 = y1;
		}
		else { // vertikalna
			y1 = rand() % Slika->Height;
			y2 = rand() % Slika->Height;
			x1 = rand() % Slika->Width;
			x2 = x1;
		}
		Duz nova(Tacka(x1, y1), Tacka(x2, y2));
		nova.Crtaj(Slika, clBlue);
		nova.A.Crtaj(Slika, clRed);
		nova.B.Crtaj(Slika, clRed);
		duzi.push_back(nova);
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ButtonPresjekHorVerDuziClick(TObject *Sender)
{
	priority_queue<pair<Duz*, Tacka>, vector<pair<Duz*, Tacka>>, HorVerDuziPoX> eventi;

	for(int i=0; i<duzi.size(); i++) {
		if (duzi[i].horizontalna()) {
			eventi.push({&duzi[i], duzi[i].A});
			eventi.push({&duzi[i], duzi[i].B});
		}
		else {
			eventi.push({&duzi[i], duzi[i].A});
		}
	}

	set<Duz*, AktivneHorVerPoY> aktivne;
	vector<Tacka> presjeci;

	while(!eventi.empty()) {
		pair<Duz*, Tacka> trenutni = eventi.top();
		eventi.pop();

		Duz* trenutna_duz = trenutni.first;
		Tacka trenutna_tacka = trenutni.second;

		if (trenutna_duz->horizontalna()) {
			if (trenutna_tacka == trenutna_duz->A) { // pocetak H
				aktivne.insert(trenutna_duz);
			}
			else { // kraj H
				aktivne.erase(trenutna_duz);
            }
		}
		else {  // vertikalna
			double gornji_y = trenutna_duz->A.y;
			double donji_y = trenutna_duz->B.y;
			Duz temp_d1 = Duz(Tacka(0, gornji_y), Tacka(0, gornji_y));
			Duz temp_d2 = Duz(Tacka(0, donji_y), Tacka(0, donji_y));

			auto it_gornji = aktivne.lower_bound(&temp_d1);
			auto it_donji = aktivne.upper_bound(&temp_d2);

			for(auto it = it_gornji; it != it_donji; it++) {
				Tacka presjek(trenutna_duz->A.x, (*it)->A.y);
				presjeci.push_back(presjek);
				presjek.Crtaj(Slika, clYellow, 5);
            }
		}

	}

	if (presjeci.size() == 0) {
        ShowMessage("Nema presjeka!");
	}
}

//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonNadjiPresjekeDuziClick(TObject *Sender)
{
	priority_queue<pair<Tacka, pair<Duz*, Duz*>>, vector<pair<Tacka, pair<Duz*, Duz*>>>, DuziPoX> eventi;

	for(int i=0; i<duzi.size(); i++) {
		eventi.push({duzi[i].A, {&duzi[i], nullptr}});
		eventi.push({duzi[i].B, {&duzi[i], nullptr}});
	}

	set<Duz*, DuziPoY> aktivne;
	vector<Tacka> presjeci;

	while(!eventi.empty()) {
		auto e = eventi.top();
		eventi.pop();

		Tacka trenutna_tacka = e.first;
		int x_sweep_line = trenutna_tacka.x;
		Duz* trenutna_duz = e.second.first;
		Duz* druga_duz = e.second.second;

		if(trenutna_tacka == trenutna_duz->A && druga_duz == nullptr) {
			// pocetak duzi
			auto it = aktivne.insert(trenutna_duz).first;
			auto prethodna = it;
			auto naredna = it;

			if (aktivne.size() > 1) {
                if (it == aktivne.begin()) {
					naredna++;
					obradi_presjek(x_sweep_line, *it, *naredna, presjeci, eventi);
				}
				else if(it == --aktivne.end()) {
					prethodna--;
					obradi_presjek(x_sweep_line, *prethodna, *it, presjeci, eventi);
				}
				else {
					naredna++;
					obradi_presjek(x_sweep_line, *it, *naredna, presjeci, eventi);
					prethodna--;
					obradi_presjek(x_sweep_line, *prethodna, *it, presjeci, eventi);
				}
			}
		}
		else if(trenutna_tacka == trenutna_duz->B && druga_duz == nullptr) {
			// kraj duzi
			auto it = aktivne.find(trenutna_duz);
			if (it == aktivne.end()) {
				continue;
			}

			if(it != aktivne.begin() && it != --aktivne.end()) {
				auto prethodna = it;
				prethodna--;
				auto naredna = it;
				naredna++;

				obradi_presjek(x_sweep_line, *prethodna, *naredna, presjeci, eventi);
			}

			aktivne.erase(it);
		}
		else {
			// presjek duzi
			auto it1 = aktivne.find(trenutna_duz);
			auto it2 = aktivne.find(druga_duz);

			if (it1 == aktivne.end() || it2 == aktivne.end()) {
				continue;
			}

			aktivne.erase(it1);
			aktivne.erase(it2);

			Duz *nova_duz1 = new Duz(trenutna_tacka, trenutna_duz->B);
			Duz *nova_duz2 = new Duz(trenutna_tacka, druga_duz->B);

			auto it_ispod = aktivne.insert(nova_duz1).first;
			auto it_iznad = aktivne.insert(nova_duz2).first;

			auto it_prije = it_iznad;
			if (it_iznad != aktivne.begin()) {
				it_prije--;
				obradi_presjek(x_sweep_line, *it_prije, *it_iznad, presjeci, eventi);
			}

			auto it_poslije = it_ispod;
			if (it_poslije != --aktivne.end()) {
				it_poslije++;
				obradi_presjek(x_sweep_line, *it_ispod, *it_poslije, presjeci, eventi);
			}

			eventi.push({nova_duz1->B, {nova_duz1, nullptr}});
			eventi.push({nova_duz2->B, {nova_duz2, nullptr}});
		}
	}
	for (auto &presjek: presjeci) {
		presjek.Crtaj(Slika, clYellow, 5);
	}
}

//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonGenerisiDuziClick(TObject *Sender)
{
	int broj_duzi = EditBrojTacaka->Text.ToInt();
	for (int i = 0; i < broj_duzi; i++) {
		int x1,x2,y1,y2;
		x1 = rand() % Slika->Width;
		x2 = rand() % Slika->Width;
		y1 = rand() % Slika->Height;
		y2 = rand() % Slika->Height;

		Duz nova(Tacka(x1, y1), Tacka(x2, y2));
		nova.Crtaj(Slika, clBlue);
		nova.A.Crtaj(Slika, clRed);
		nova.B.Crtaj(Slika, clRed);
		duzi.push_back(nova);
	}
}

//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonKDStabloClick(TObject *Sender)
{
	 int xmin = p_xmin->Text.ToInt();
	 int xmax = p_xmax->Text.ToInt();
	 int ymin = p_ymin->Text.ToInt();
	 int ymax = p_ymax->Text.ToInt();
	 Pravougaonik query_p(xmin, xmax, ymin, ymax);
	 query_p.Crtaj(Slika, clRed);

	 KDStablo stablo(tacke, Slika->Width, Slika->Height);
	 stablo.iscrtaj(Slika);

	 vector<Tacka> query_tacke;
	 stablo.query(query_p, query_tacke);

	for(auto tacka: query_tacke) {
		tacka.Crtaj(Slika, clYellow, 4);
	}
	 // napravi stablo
	 // uradi query
}

//---------------------------------------------------------------------------

//PROJEKAT

//---------------------------------------------------------------------------

void __fastcall TForm1::DugmeTriangulacijaInduktivnoClick(TObject *Sender)
{
    triangulacijaTacakaRekurzivno(tacke, stranice);

	for(auto s : stranice) {
		s.Crtaj(Slika);
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::DugmeTriangulacijaSaKoracimaClick(TObject *Sender)
{
    if (tacke.size() < 3) {
        return;
    }

    sort(tacke.begin(), tacke.end());

    deque<Tacka> konveksni_omotac = {tacke[0], tacke[1], tacke[2]};

    stranice.push_back(Duz(tacke[0], tacke[1]));
    stranice.push_back(Duz(tacke[1], tacke[2]));
    stranice.push_back(Duz(tacke[0], tacke[2]));

    for (auto s : stranice) {
        s.Crtaj(Slika);
    }

    if (Orijentacija(tacke[0], tacke[1], tacke[2]) > 0) {
        swap(konveksni_omotac[1], konveksni_omotac[2]);
    }

    for (int i = 3; i < tacke.size(); i++) {
        Tacka trenutna(tacke[i]);

        pair<int, int> tangente = nadjiTangente(trenutna, konveksni_omotac);
        int donja = tangente.first;
        int gornja = tangente.second;

        ShowMessage("");
        stranice.push_back(Duz(trenutna, konveksni_omotac[donja]));
        Duz(trenutna, konveksni_omotac[donja]).Crtaj(Slika);

        ShowMessage("");
        stranice.push_back(Duz(trenutna, konveksni_omotac[gornja]));
        Duz(trenutna, konveksni_omotac[gornja]).Crtaj(Slika);

        if (donja > gornja) {
            for (auto it = konveksni_omotac.begin() + donja + 1; it != konveksni_omotac.end(); it++) {
                ShowMessage("");
                stranice.push_back(Duz(trenutna, *it));
                (Duz(trenutna, *it)).Crtaj(Slika, clRed);
            }

            konveksni_omotac.erase(konveksni_omotac.begin() + donja + 1, konveksni_omotac.end());

            if (gornja > 0) {
                for (auto it = konveksni_omotac.begin(); it != konveksni_omotac.begin() + gornja; it++) {
                    ShowMessage("");
                    stranice.push_back(Duz(trenutna, *it));
                    (Duz(trenutna, *it)).Crtaj(Slika, clRed);
                }
                konveksni_omotac.erase(konveksni_omotac.begin(), konveksni_omotac.begin() + gornja);
            }

            konveksni_omotac.insert(konveksni_omotac.begin(), trenutna);
        } else {
            for (auto it = konveksni_omotac.begin() + donja + 1; it != konveksni_omotac.begin() + gornja; it++) {
                ShowMessage("");
                stranice.push_back(Duz(trenutna, *it));
                (Duz(trenutna, *it)).Crtaj(Slika, clRed);
            }
            konveksni_omotac.erase(konveksni_omotac.begin() + donja + 1, konveksni_omotac.begin() + gornja);
            konveksni_omotac.insert(konveksni_omotac.begin() + donja + 1, trenutna);
        }
    }
}

//---------------------------------------------------------------------------

