

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struktura predmeti
typedef struct PREDMETI{
    char sifra[50];
    int ocena;
    struct PREDMETI *sl, *pret;
}predmeti;


//struktura ucenik
typedef struct UCENIK{
    char indeks[50];
    struct PREDMETI *pred;
    struct UCENIK *sl;
}ucenik;


//fja krece od zaglavlja neke liste predmeta i broji koliko elemenata lista ima, ako je samo zaglavlje vraca n = 0
int PrebrojPredmete(predmeti *zag){
    predmeti *tren;
    int n = 0;

    if (zag -> sl == zag)
        return 0;

    tren = zag -> sl;
    while(strcmp(tren -> sifra,"zaglavlje"))
    {
        tren = tren -> sl;
        n++;
    }
    return n;
}


//fja leksikografski ubacuje predmet sifre x, ocene y, u listu predmeta odredjenu zaglavljem, tako sto nadje prvi el
// leksikografski manji od x novog elementa, zatim oslobadja prostor i dvostruko ulancava novi el na tu poziciju
int UbaciPredmetLeks(char x[], int y, predmeti *zaglavlje){
    predmeti *novi, *sledeci, *poslednji, *tren;

    poslednji = zaglavlje;

    if (PrebrojPredmete(zaglavlje) == 0)
        sledeci = poslednji;

    else
    {
        tren = zaglavlje -> sl;
        while (strcmp(tren -> sifra, "zaglavlje")) {
            if (strcmp(x, tren -> sifra) < 0)
                break;
            if (strcmp(x, tren -> sifra) == 0)
            {
                printf("Predmet sa unetom sifrom vec postoji\n");
                return 0;
            }
            tren = tren -> sl;
        }
        poslednji = tren -> pret;
        sledeci = tren;
    }

    novi = malloc(sizeof(predmeti));
    strcpy(novi -> sifra, x);
    novi -> ocena = y;
    novi -> sl = sledeci;
    novi -> pret = poslednji;

    sledeci -> pret = novi;
    poslednji -> sl = novi;

    return 1;
}


//fja oslobadja prostor koju zauzima el na adresi x, i dvostruko ulancava prethodnika i sledbenika
void ObrisiPredmet(predmeti *x){
    if (strcmp(x -> sifra, "zaglavlje"))
    {
        predmeti *prethodni, *sledeci;
        prethodni = x -> pret;
        sledeci = x -> sl;

        prethodni -> sl = sledeci;
        sledeci -> pret = prethodni;
        //free(x -> sifra);
        free(x);
    }
}


//fja redom od zaglavlja brise sve el u listi
void ObrisiSvePredmete(predmeti *zaglavlje){
    predmeti *tren, *brisi;
    if(PrebrojPredmete(zaglavlje) != 0)
    {
        tren = zaglavlje -> sl;
        while(strcmp(tren -> sifra, "zaglavlje"))
        {
            brisi = tren;
            tren = tren -> sl;
            //free(brisi -> sifra);
            free(brisi);
        }
        zaglavlje -> sl = zaglavlje;
        zaglavlje -> pret = zaglavlje;
    }
}


//fja vradja adresu predmeta sa sifrom x, koristi se u drugim funkcijama i u mainu
predmeti *NadjiPredmet(char x[], predmeti *zaglavlje){
    predmeti *tren;
    if (PrebrojPredmete(zaglavlje) == 0)
        return zaglavlje;

    tren = zaglavlje -> sl;
    while(strcmp(tren -> sifra, "zaglavlje"))
    {
        if (strcmp(x, tren -> sifra) == 0)
            break;
        tren = tren -> sl;
    }
    return tren;
}


//fja ispisuje predmete sa ocenom o
void IspisiPredmeteSaOcenom(int o, predmeti *zaglavlje) {
    predmeti *tren;
    int x = 0;
    if (PrebrojPredmete(zaglavlje) == 0)
    {
        printf("Nema polozenih predmeta za ovog studenta\n");
        return;
    }

    tren = zaglavlje -> sl;
    while(strcmp(tren -> sifra, "zaglavlje"))
    {
        if (o == tren -> ocena)
        {
            if(x == 0)
                printf("Sifre polozenih predmeta sa ocenom %d su\n", o);
            printf("%s\n", tren -> sifra);
            x = 1;
        }
        tren = tren -> sl;
    }
    if(!x)
        printf("Nema polozenih predmeta sa tom ocenom\n");
}


//fja ispisuje sve predmete
void IspisiSvePredmete(predmeti *zaglavlje){
    predmeti *tren;
    if (zaglavlje -> sl != zaglavlje)
    {
        tren = zaglavlje -> sl;
        while(strcmp(tren -> sifra, "zaglavlje"))
        {
            printf("%s :  %d\n",tren -> sifra, tren -> ocena);
            tren = tren -> sl;
        }
    }
    else
        printf("Nema polozenih predmeta\n");
}


//fja pravi listu razlike dve liste predmeta
void RazlikaDveListe(predmeti *z1, predmeti *z2){
    predmeti *levi, *desni;
    int x = 0;
    if(z1 -> sl == z1 && z2 -> sl == z2)
    {
        printf("Liste su prazne\n");
        return;
    }
    if(z1 -> sl == z1)
        levi = z1;
    else
        levi = z1 -> sl;
    if(z2 -> sl == z2)
        desni = z2;
    else
        desni = z2 -> sl;

    while (strcmp(levi -> sifra, "zaglavlje") || strcmp(desni -> sifra, "zaglavlje"))
    {
        if (strcmp(levi -> sifra, "zaglavlje") == 0 || strcmp(levi -> sifra, desni -> sifra) > 0)
        {
            x = 1;
            printf("%s\n", desni -> sifra);
            desni = desni -> sl;
            continue;
        }
        if (strcmp(desni -> sifra, "zaglavlje") == 0 || strcmp(levi -> sifra, desni -> sifra) < 0)
        {
            x = 1;
            printf("%s\n", levi -> sifra);
            levi = levi -> sl;
            continue;
        }
        if (strcmp(levi -> sifra, desni -> sifra) == 0)
        {
            levi = levi -> sl;
            desni = desni -> sl;
            continue;
        }
    }
    if (x == 0)
        printf("Svi predmeti su isti");

}


//fja ubacuje novi el sa indeksom x u jednostruko ulancanu nekruznu listu sa zaglavljem
ucenik *UbaciUcenika(char x[], ucenik *zagl){
    ucenik *novi, *tren;

    novi = malloc(sizeof(ucenik));
    strcpy(novi -> indeks, x);
    novi -> sl = NULL;

    novi -> pred = malloc(sizeof(predmeti));
    strcpy(novi -> pred -> sifra, "zaglavlje");
    novi -> pred -> ocena = 0;
    novi -> pred -> sl = novi -> pred;
    novi -> pred -> pret = novi -> pred;

    if (zagl -> sl == NULL)
        zagl -> sl = novi;
    else
    {
        tren = zagl -> sl;
        while(tren -> sl != NULL)
        {
            tren = tren -> sl;
        }
        tren -> sl = novi;
    }
    return novi;
}


//fja ispisuje sve ucenike
void IspisiSveUcenike(ucenik *zagl){
    ucenik *tren;
    if (zagl -> sl != NULL)
    {
        tren = zagl;
        while(tren -> sl != NULL)
        {
            printf("Polozeni predmeti studenta %s su:\n",tren -> sl -> indeks);
            IspisiSvePredmete(tren -> sl -> pred);
            tren = tren -> sl;
            printf("\n");
        }
    }
    else
        printf("Nema unetih studenata\n");
}


//fja vraca adresu el sa indeksom x, tj NULL ako takva adresa ne postoji
ucenik *NadjiUcenika(char x[], ucenik *zagl){
    ucenik *tren;
    if (zagl -> sl == NULL)
        return NULL;
    else
    {
        tren = zagl;
        while(tren -> sl != NULL)
        {
            if(strcmp(tren -> sl -> indeks, x) == 0)
            {
                return tren -> sl;
            }
            tren = tren -> sl;
        }
        return NULL;
    }
}


int main() {
    int n, y, c;
    char x[50], id[50], id1[50], id2[50];
    predmeti  *levi = NULL, *desni = NULL, *pom = NULL;
    ucenik *uzag, *utren;

    //stavranje zaglavlja za ulancanu listu studenata
    uzag = malloc(sizeof(ucenik));
    strcpy(uzag -> indeks, "br_indeksa");
    uzag -> sl = NULL;
    uzag -> pred = NULL;

    while(1) {
        printf("\nDobar dan! Molim Vas da u konzolu unesete broj stavke za koju ste zainteresovani\n\n");
        printf("(1) Stvaranje novog studenta\n(2) Dodavanje polozenih predmeta za odredjenog studenta\n(3) Prikaz svih predmeta sa odredjenom ocenom za studenta\n(4) Prikaz razlike polozenih predmeta za dva studenta\n(5) Ponistavanje (brisanje) polozenog predmeta za studenta\n(6) Brisanje cele liste polozenih predmeta za studenta\n(7) Ispis broja polozenih predmeta za studenta\n(8) Ispisivanje svih polozenih predmeta za studenta\n(9) Ispisivanje svih polozenih predmeta za sve studente\n(0) Kraj programa\n");
        scanf("%d", &c);
        if (c == 0) {
            printf("Prijatan dan! :)\n");
            break;
        }
        if (c > 9 || c < 1) printf("Izabrali ste nepostojecu opciju, pokusajte ponovo\n");
        else
            switch (c) {

                //Stvaranje novog studenta
                case 1:
                {
                    printf("Unesite broj indeksa studenta:\n");
                    scanf("%s", id);
                    if (NadjiUcenika(id, uzag))
                        printf("Student sa unesenim indeksom vec postoji\n");
                    else
                    {
                        utren = UbaciUcenika(id, uzag);
                        printf("Uspesno ste stvorili novog studenta\n");
                    }
                    continue;
                }

                    //Dodavanje n polozenih predmeta za odredjenog studenta
                case 2:
                {
                    printf("Unesite broj indeksa studenta:\n");
                    scanf("%s", id);
                    utren = NadjiUcenika(id, uzag);
                    if (utren == NULL)
                        printf("Student sa unesenim indeksom ne postoji\n");
                    else
                    {
                        printf("Unesite sifru predmeta:\n");
                        scanf("%s", x);
                        pom = NadjiPredmet(x, utren -> pred);

                        printf("Unesite ocenu iz predmeta sa sifrom %s:\n", x);
                        scanf("%d", &y);
                        if (y > 10 || y < 5)
                        {
                            printf("Greska prilikom unosa ocene\n");
                            continue;
                        }
                        if (y == 5)
                        {
                            printf("Predmet  nije polozen\n");
                            continue;
                        }
                        n = UbaciPredmetLeks(x, y, utren->pred);
                        if(n)
                            printf("Uspesno ste uneli polozen predmet\n");

                    }
                    continue;
                }

                    //Prikaz svih polozenih predmeta sa odredjenom ocenom za studenta
                case 3: {
                    printf("Unesite broj indeksa studenta:\n");
                    scanf("%s", id);
                    utren = NadjiUcenika(id, uzag);
                    if (utren == NULL)
                        printf("Student sa unesenim indeksom ne postoji\n");
                    else
                    {
                        printf("Unesite ocenu kojom zelite da filtrirate listu polozenih predmeta za studenta %s:\n", utren -> indeks);
                        scanf("%d", &n);
                        if (n > 10 || n < 5)
                        {
                            printf("Greska prilikom unosa ocene\n");
                            continue;
                        }
                        if (y == 5)
                        {
                            printf("Predmet  nije polozen\n");
                            continue;
                        }
                        IspisiPredmeteSaOcenom(n, utren -> pred);
                    }
                    continue;
                }

                    //Prikaz razlike polozenih predmeta dva studenta
                case 4: {
                    printf("Unesite broj indeksa prvog studenta:\n");
                    scanf("%s", id1);
                    utren = NadjiUcenika(id1, uzag);
                    if (utren == NULL)
                    {
                        printf("Student sa unesenim indeksom ne postoji\n");
                        continue;
                    }
                    else
                        levi = utren -> pred;

                    printf("Unesite broj indeksa drugog studenta:\n");
                    scanf("%s", id2);
                    utren = NadjiUcenika(id2, uzag);
                    if (utren == NULL)
                    {
                        printf("Student sa unesenim indeksom ne postoji\n");
                        continue;
                    }
                    else
                        desni = utren -> pred;

                    RazlikaDveListe(levi, desni);

                    continue;
                }

                    //Ponistavanje (brisanje) polozenog predmeta
                case 5: {
                    printf("Unesite broj indeksa studenta:\n");
                    scanf("%s", id);
                    utren = NadjiUcenika(id, uzag);
                    if (utren == NULL)
                        printf("Student sa unesenim indeksom ne postoji\n");
                    else
                    {
                        printf("Unesite sifru predmeta koji zelite da ponistite za studenta %s:\n", utren -> indeks);
                        scanf("%s", x);
                        pom = NadjiPredmet(x, utren -> pred);
                        if (strcmp(pom -> sifra, "zaglavlje") == 0)
                            printf("Ne postoji predmet sa unetom sifrom\n");
                        else
                        {
                            ObrisiPredmet(pom);
                            printf("Uspesno ste obrisali predmet\n");
                        }
                    }
                    continue;
                }

                    //brisanje cele liste polozenih predmeta za studenta
                case 6: {
                    printf("Unesite broj indeksa studenta:\n");
                    scanf("%s", id);
                    utren = NadjiUcenika(id, uzag);
                    if (utren == NULL)
                        printf("Student sa unesenim indeksom ne postoji\n");
                    else
                    {
                        ObrisiSvePredmete(utren -> pred);
                        printf("Uspesno ste obrisali celu listu predmeta\n");
                    }
                    continue;
                }

                    //Ispis broja polozenih predmeta za odredjenog sudenta
                case 7: {
                    printf("Unesite broj indeksa studenta:\n");
                    scanf("%s", id);
                    utren = NadjiUcenika(id, uzag);
                    if (utren == NULL)
                        printf("Student sa unesenim indeksom ne postoji\n");
                    else
                    {
                        printf("Broj polozenih predmeta za studenta sa indeksom %s je\n%d\n", id, PrebrojPredmete(utren -> pred));
                    }
                    continue;
                }

                    //Ispisivanje svih polozenih predmeta za studenta
                case 8: {
                    printf("Unesite broj indeksa studenta:\n");
                    scanf("%s", id);
                    utren = NadjiUcenika(id, uzag);
                    if (utren == NULL)
                        printf("Student sa unesenim indeksom ne postoji\n");
                    else
                    {
                        printf("Polozeni predmeti studenta %s su:\n",id);
                        IspisiSvePredmete(utren -> pred);
                    }
                    continue;
                }

                    //Ispisivanje svih polozenih predmeta za SVE studente
                case 9: {
                    IspisiSveUcenike(uzag);

                    continue;
                }
            }
    }

    return 0;
}
