/*

Задатак 2 – алгоритми за рад са ретким матрицама (50 поена)
Написати интерактиван програм на програмском језику C или Python који илуструје рад
са ретким матрицама. Ретке матрице су врста матрица код којих већина елемената има
подразумевану вредност. Код оваквих матрица се могу направити значајне уштеде у
простору потребном за њихово складиштење уколико се експлицитно памте само
вредности које нису подразумеване. У наставку је дат кратак опис три технике смештања
ретких матрица.
Вектор записа од три поља (Coordinate Of Objects – COO) је начин смештања ретке
матрице димензија MxN код кога се неподразумеване вредности памте експлицитним
чувањем тројки које чине врсту, колону и вредност елемента. Тројке у вектору су уређене
најпре по редном броју врсте, а затим по редном броју колоне.
Compressed row storage (CSR) формат чува ретку матрицу димензија MxN помоћу три
независна вектора (V, C, R). Вектор V садржи вредности и има онолико елемената колико
има неподразумеваних елемената матрице. Вектор C садржи број колоне одговарајућег
елемента вектора V. Сваки елемент вектора R одговара једној врсти матрице и садржи
индекс првог елемента вектора C и V који одговара тој врсти. Вектор R има дужину М+1,
а у последњем елементу се чува укупан број неподразумеваних елемената матрице.
Compressed sparse column (CSC) формат чува ретку матрицу димензија MxN на сличан
начин као CSR формат, осим што вектори R и C замењују места и значење. Вектор V
садржи вредности и има онолико елемената колико има неподразумеваних елемената
матрице. Вектор R садржи број врсте одговарајућег елемента вектора V. Сваки елемент
вектора C одговара једној колони матрице и садржи индекс првог елемента вектора R и
V који одговара тој колони. Вектор C има дужину N+1, а у последњем елементу се чува
укупан број неподразумеваних елемената матрице.
Зависно од редног броја проблема i, саставити један од следећих програма, који
представља ретку матрицу помоћу:
0. једног вектора записа од три поља
1. три независна вектора у CSR формату
2. три независна вектора у CSC формату
Програм треба да омогући стварање матрице и унос ретке матрице задатих димензија и
задатог броја ненултих елемената, дохватање вредности елемента задате врсте и колоне,
испис матрице (укључујући и елементе подразумеване вредности). Посебно, зависно од
редног броја проблема j, саставити један од следећих потпрограма који омогућавају
следећу операцију за рад са ретком матрицом:
0. сабирање две матрице уз проверу да ли се могу сабрати
1. транспоновање матрице
2. сума подматрице која је одређена координатама горње левог и доње десног темена
3. ротирање матрице за 90 у смеру казаљке на сату*/

#include <stdio.h>


void IspisiMatricu(int n, int m, int x, int *v, int *r, int *c){
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++) {
            int provera = 1;
            for (int k = c[j]; k < c[j+1]; k++)
                if (r[k] == i)
                {
                    provera = 0;
                    printf("%d ", v[k]);
                    break;
                }
            if (provera)
                printf("0 ");
        }
        printf("\n");
    }
    printf("\n");
}


void IspisCSC(int x,int m,int *v, int *r, int *c){
    for (int i = 0; i < x; i++)
    {
        printf("%d ",v[i]);
    }
    printf("\n");
    for (int i = 0; i < x; i++)
    {
        printf("%d ", r[i]);
    }
    printf("\n");
    for (int i = 0; i <= m; i++)
    {
        printf("%d ",c[i]);
    }
    printf("\n");
}


void IspisiTransponovanu(int n, int m, int x, int *v, int *r, int *c){
    int p = 0, br = 1;
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if (p == x)
            {
                printf("0 ");
                continue;
            }
            if(p == c[br])
                br++;
            if(i == br-1 &&  j == r[p])
            {
                printf("%d ", v[p]);
                p++;
            }
            else
                printf("0 ");
        }
        printf("\n");
    }
    printf("\n");
}


int VratiVrednost(int *v, int *r, int *c, int vrsta, int kolona){
    for (int i = c[kolona]; i < c[kolona+1]; i++)
        if (vrsta == r[i])
            return v[i];
    return 0;
}


int main() {
    int n = 0, m = 0, x = 0, pom, u = 1;
    int v[1000], r[1000], c[1000];
    printf("Dobar dan!");
    while(1)
    {
        printf("\nIzaberite jednu od sledecih stavki za obradu matrice:\n\n(1) Unos nove matrice\n(2) Ispis matrice\n(3) Ispis matrice u CSC formatu\n(4) Ispis transponovane matrice\n(5) Dohvatanje vrednosti iz odredjene vrste i kolone\n(0) Kraj rada programa\n");
        scanf("%d", &pom);
        if(pom > 5 || pom < 0)
        {
            printf("Izabrana opcija ne postoji\n");
            continue;
        }

        switch (pom)
        {
            //prekid programa
            case 0:{
                printf("Prijatan dan! :)");
                break;
            }

                //unos nove matrice
            case 1:{
                int k = 0;


                printf("\nIzaberite jednu od sledece dve opcije za unos matrice:\n\n(1) Unos matrice preko indeksa nepodrazumevanih elemenata\n(2) Unos matrice kao niz kolona\n");
                scanf("%d", &pom);
                if(pom > 2 || pom < 1)
                {
                    printf("Greska pri unosu\n");
                    u = 1;
                    continue;
                }
                printf("\nUnesite broj vrsta matrice:");
                scanf("%d", &n);
                printf("Unesite broj kolona matrice:");
                scanf("%d", &m);
                printf("Unesite broj nepodrazumevanih elemenata:");
                scanf("%d", &x);

                for (int i = 0; i <= m; i++)
                    c[i] = 0;

                //formiranje CSC formata sa ulaza preko koordinata
                if (pom == 1)
                {
                    printf("\nUnesite koordinate nepodrazumevanih elemenata, prolazeci kroz matricu kroz kolone\nElementi se unose kao vrednost -> vrsta -> kolona:\n");

                    for (int i = 0; i < x; i++)
                    {
                        scanf("%d", &v[i]);
                        scanf("%d", &r[i]);
                        scanf("%d", &pom);
                        c[pom+1]++;
                    }
                    for (int i = 1; i <= m; i++)
                        c[i] += c[i-1];
                    u = 0;
                    continue;
                }

                //unos matrice kao niz kolona
                if (pom == 2)
                {
                    printf("\nUnesite matricu kao niz kolona:\n");

                    for (int i = 0; i < m; i++)
                        for(int j = 0; j < n; j++)
                        {
                            scanf("%d", &pom);
                            if(pom)
                            {
                                v[k] = pom;
                                r[k] = j;
                                c[i+1]++;
                                k++;
                            }
                        }
                    for (int i = 1; i <= m; i++)
                        c[i] += c[i-1];
                    u = 0;
                }
                continue;
            }

            //ispis matrice
            case 2:{
                if(u)
                {
                    printf("Neophodno je prvo uneti matricu\n");
                    continue;
                }
                IspisiMatricu(n, m, x, v, r, c);
                continue;
            }

            //ispis CSC
            case 3:{
                if(u)
                {
                    printf("Neophodno je prvo uneti matricu\n");
                    continue;
                }
                IspisCSC(x, m, v, r, c);
                continue;
            }

            //ispis transponovane
            case 4:{
                if(u)
                {
                    printf("Neophodno je prvo uneti matricu\n");
                    continue;
                }
                IspisiTransponovanu(n, m, x, v, r, c);
                continue;
            }

            //dohvatanje vrednosti
            case 5:{
                int pom1, pom2;

                if(u)
                {
                    printf("Neophodno je prvo uneti matricu\n");
                    continue;
                }
                printf("Unesite vrednosti zeljene vrste i kolone:\n");
                scanf("%d %d",&pom1, &pom2);
                if(pom1 > n || pom1 < 0 || pom2 > m || pom2 < 0)
                {
                    printf("Zadate koordinate se ne nalaze u matrici\n");
                    continue;
                }
                printf("\nVrednost elementa u %d. vrsti i %d. koloni je:\n%d\n", pom1, pom2, VratiVrednost(v, r, c, pom1, pom2));
                continue;
            }

        }
        break;
    }


    return 0;
}
