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
