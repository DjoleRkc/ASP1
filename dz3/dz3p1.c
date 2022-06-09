/*
 * Задатак 1 – Имплементација основних алгоритама за рад са графом
[40 поена]
Написати програм на програмском језику C који илуструје рад са усмереним графовима. Програм треба
да омогући следеће операције над графом:
• [5 поена] Креирање празне структуре података за граф задатих димензија (постоје чворови графа,
али не постоје гране)
• [10 поена] Додавање чвора у граф и уклањање чвора из графа
• [10 поена] Додавање и уклањање гране између два чвора у графу
• [5 поена] Испис репрезентације графа
• [5 поена] Брисање графа из меморије
[5 поена] Корисник са програмом интерагује путем једноставног менија. Програм треба да испише
садржај менија, а затим да чека да корисник изабере (унесе путем тастатуре) редни број неке од
понуђених ставки, након чега, пре извршења, од корисника очекује да по потреби унесе додатне
параметре. Поступак се понавља све док корисник у менију не изабере опцију за прекид програма. Све
наведене операције треба реализовати путем одговарајућих потпрограма чији је један од аргумената
показивач на структуру података која имплементира граф са којим се ради.
 
Задатак 2 – Мултилатерална компензација [60 поена]
Мултилатерална компензација представља начин пребијања дуговања (потраживања) између више
пословних субјеката. Извршавање компензације подразумева проналажење највећег могућег износа
који се може компензовати, односно пребити. Потраживања међу пословним субјектима се могу
моделовати усмереним тежинским графом. Грана од субјекта X ка субјекту Y тежине T означава
постојање дуга износу од T евра (или друге новчане јединице) пословног субјекта X субјекту Y.
На пример, нека су пре компензације дуговања следећа (слика 1, лево):
• Пословни субјект 1 има дуговање према пословном субјекту 2 у износу од 2500 евра,
• Пословни субјект 2 има дуговање према пословном субјекту 3 у износу од 2000 евра и
• Пословни субјект 3 има дуговање према пословном субјекту 1 у износу од 1000 евра.
Дакле, максимално је могуће компензовати износ од 1000 евра. Након компензације, стање је следеће
(слика 1, десно):
• Пословни субјект 1 има дуговање према пословном субјекту 2 у износу од 1500 евра,
• Пословни субјект 2 има дуговање према пословном субјекту 3 у износу од 1000 евра и
• Пословни субјект 3 више нема дуговања према пословном субјекту 1.

Имплементирати решење које ће омогућити:
• [10 поена] Учитавање графа који представља дуговања компанија из улазне текстуалне датотеке
• [20 поена] Проналажење и исписивање циклуса у графу
• [10 поена] Одређивање највећег могућег новчаног износа који је могуће компензовати за сваки
пронађени циклус
• [20 поена] Спровођење компензације и формирање новог графа дуговања.
 
У зависности од редног броја j добијеног коришћењем формуле назначене у напоменама, потребно је
користити једну од следећих техника за проналажење циклуса у усмереном графу:
1. Алгоритам за обилазак графа по дубини
2. Warshall-ов алгоритам
Сматрати да се у оквиру улазне текстуалне датотеке граф репрезентује матрицом суседности по
следећем формату: у првом реду датотеке се налази број n који представља број чворова у графу, а затим
следи n редова који садрже саму матрицу суседности. Студентима су на располагању графови за
тестирање у датотекама graf2.txt и graf3.txt, а сваки студент треба да осмисли један додатан пример
графа за тестирање са минимално 10 чворова и 3 циклуса. На слици 2 је приказан граф дуговања који се
налази у датотеци graf2.txt. 
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct GRANA{
    int cvor;
    int tezina;
}grana;


int * napraviCvorove(int n)
{
    int *m = calloc(n, sizeof(int));
    for(int i = 0; i < n; i++)
        m[i] = 1;
    return m;
}

int * napraviGrane()
{
    int *m = calloc(1, sizeof(int));
    return m;
}

int * dodajCvor(int *n, int *m)
{
    m = realloc(m, (*n + 1) * sizeof(int));
    m[*n] = m[*n - 1];
    *n += 1;
    return m;
}

int * dodajGranu(int from, int to, int n, int *cvorovi, int *grane)
{
    int gr = cvorovi[n - 1];
    grane = realloc(grane, (gr + 1) * sizeof(int));

    cvorovi[from] = (cvorovi[from] == cvorovi[from + 1]) ? cvorovi[from + 1] : cvorovi[from];

    for(int i = from + 1; i < n; i++)
        cvorovi[i]++;

    for(int i = cvorovi[n - 1]; i > cvorovi[from] ; i--)
        grane[i] = grane[i - 1];

    grane[cvorovi[from]] = to;
    return grane;
}

grana * dodajTezinskuGranu(int from, int to, int tezina, int n, int *cvorovi, grana *grane)
{
    int gr = cvorovi[n - 1];
    grane = realloc(grane, (gr + 1) * sizeof(grana));

    cvorovi[from] = (cvorovi[from] == cvorovi[from + 1]) ? cvorovi[from + 1] : cvorovi[from];

    for(int i = from + 1; i < n; i++)
        cvorovi[i]++;

    for(int i = cvorovi[n - 1]; i > cvorovi[from] ; i--)
    {
        grane[i].cvor = grane[i - 1].cvor;
        grane[i].tezina = grane[i - 1].tezina;
    }

    grane[cvorovi[from]].cvor = to;
    grane[cvorovi[from]].tezina = tezina;
    return grane;
}

int brojGrana(int from, int n, int *cvorovi)
{
    return cvorovi[from + 1] - cvorovi[from];
}

int granaPostoji(int from, int to, int n, int *cvorovi, const int *grane)
{
    int br = brojGrana(from, n, cvorovi);
    for(int i = 0; i < br; i++)
        if(grane[i + cvorovi[from]] == to)
            return 1;
    return 0;
}

int * obrisiGranu(int from, int to, int n, int *cvorovi, int *grane)
{
    int j = cvorovi[from];
    while(grane[j++] != to);

    for(int i = j; i < cvorovi[n - 1]; i++)
        grane[i - 1] = grane[i];

    cvorovi[from] = ((cvorovi[from + 1] == cvorovi[from] + 1) ? cvorovi[from + 1] - 1 : cvorovi[from]);

    for(int i = from + 1; i < n; i++)
        cvorovi[i]--;

    grane = realloc(grane, cvorovi[n - 1] * sizeof(int));
    return grane;
}

int * obrisiCvor(int id, int *n, int *cvorovi, int *grane)
{
    int br = brojGrana(id, *n, cvorovi);

    for(int i = 0; i < br; i++)
        grane = obrisiGranu(id, grane[cvorovi[id]], *n, cvorovi, grane);

    for(int i = id + 1; i < *n; i++)
        cvorovi[i - 1] = cvorovi[i];

    *n -= 1;
    cvorovi = realloc(cvorovi, *n * sizeof(int));
    return cvorovi;
}

void ispisiGraf(int n, int *cvorovi, int *grane)
{
    printf("niz cvorova\n");
    for(int i = 1; i < n; i++)
        printf("%d ", cvorovi[i]);

    printf("\nniz grana\n");

    for(int i = 1; i < cvorovi[n - 1]; i++)
        printf("%d ", grane[i]);

    printf("\n");
}

void ispisiTezinskiGraf(int n, int *cvorovi, grana *grane)
{
    printf("niz cvorova:\n");
    for(int i = 1; i < n; i++)
        printf("%d%c", cvorovi[i], (i == n - 1) ? '\n' : ' ');

    printf("niz grana\n");

    for(int i = 1; i < cvorovi[n - 1]; i++)
        printf("%d%c", grane[i].cvor, (i == cvorovi[n - 1] - 1) ? '\n' : ' ');
    for(int i = 1; i < cvorovi[n - 1]; i++)
        printf("%d%c", grane[i].tezina, (i == cvorovi[n - 1] - 1) ? '\n' : ' ');
    printf("\n");
}

void meni()
{
    printf("\nZAD 1\nIzaberite jednu od sledecih stavki:\n\t(1) dodavanje cvora\n\t(2) brisanje cvora\n\t(3) dodavanje grane\n\t(4) brisanje grane\n\t(5) ispis grafa\n\t(6) povratak na glavni meni\n\t(0) prekid programa\n\n");
}

void freeGraf(int *cvorovi, int *grane)
{
    free(cvorovi);
    free(grane);
}

void zad1()
{
    int n, t, x, y, *cvorovi, *grane;

    printf("Unesite broj cvorova:\n");
    scanf_s("%d", &n);
    n+= 2;
    cvorovi = napraviCvorove(n);
    grane = napraviGrane();

    while(1)
    {
        meni();
        scanf_s("%d", &t);
        switch (t)
        {
            case 1:
                printf("\nuspesno ste dodali cvor sa indeksom %d\n\n", n-1);
                dodajCvor(&n, cvorovi);
                break;

            case 2:
                printf("\nunesite indeks cvora koji zelite da obrisete\n");
                scanf_s("%d", &x);
                if(x < n - 1 && x > 0 && n != 2)
                {
                    cvorovi = obrisiCvor(x, &n, cvorovi, grane);
                    printf("uspesno ste obrisali cvor\n");
                }
                else
                    printf("cvor ne postoji\n");
                break;

            case 3:
                printf("\nunesite indekse cvorova koje povezuje grana koju zelite da dodate\n");
                scanf_s("%d%d", &x, &y);
                if(x > n - 2 || x < 1)
                {
                    printf("cvor %d ne postoji\n", x);
                    break;
                }
                if(y > n - 2 || y < 1)
                {
                    printf("cvor %d ne postoji\n", y);
                    break;
                }
                if(granaPostoji(x, y, n, cvorovi, grane))
                {
                    printf("grana vec postoji\n");
                    break;
                }
                grane = dodajGranu(x, y, n, cvorovi, grane);
                printf("uspesno ste dodali granu\n");
                break;

            case 4:
                printf("\nunesite indekse cvorova koje povezuje grana koju zelite da obrisete\n");
                scanf_s("%d%d", &x, &y);
                if(x > n - 2 || x < 1)
                {
                    printf("cvor %d ne postoji\n", x);
                    break;
                }
                if(y > n - 2 || y < 1)
                {
                    printf("cvor %d ne postoji\n", y);
                    break;
                }
                if(granaPostoji(x, y, n, cvorovi, grane) == 0)
                {
                    printf("grana ne postoji\n");
                    break;
                }
                grane = obrisiGranu(x, y, n, cvorovi, grane);
                printf("uspesno ste obrisali granu\n");
                break;

            case 5:
                printf("graf predstavljen linearizovanom listom susednosti izgleda kao:\n");
                ispisiGraf(n, cvorovi, grane);
                break;

            case 6:
                freeGraf(cvorovi, grane);
                return;

            case 7:
                printf("asp je moj omiljeni predmet <3\n");
                break;

            case 0:
                printf("Prijatan dan :)");
                exit(0);

            default:
                printf("\nizabrali ste nepostojecu opciju iz menija, pokusajte ponovo\n");
        }
    }
}



int ** ucitajFajl(char *s, int *n)
{
    FILE *file = fopen(s, "r");
    if(!file)
    {
        printf("\nFAJL NE POSTOJI\n");
        return NULL;
    }

    fscanf_s(file, "%d", n);

    int ** m = malloc(*n * sizeof(int *));
    for(int i = 0; i < *n; i++)
    {
        m[i] = malloc(*n * sizeof(int));
        for(int j = 0; j < *n; j++)
            fscanf_s(file, "%d", &m[i][j]);
    }

    fclose(file);
    return m;
}

void dodelaVrednosti(int n, int **p, int **a, int **m)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            p[i][j] = (m[i][j]) ? 1 : 99999;
            a[i][j] = (m[i][j] && i != j) ? j : -1;
        }
}

void matricaToLinearizovana(int n, int **m)
{
    int *cvorovi = napraviCvorove(n + 2);
    grana *grane = malloc(sizeof(grana));

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            if(m[i][j])
                grane = dodajTezinskuGranu(i + 1, j + 1, m[i][j], n + 2, cvorovi, grane);

    ispisiTezinskiGraf(n + 2, cvorovi, grane);
    free(cvorovi);
    free(grane);
}

void dodeliMinVrendost(int i, int j, int *a, int *b, int ** m)
{
    if(m[i][j] < m[*a][*b])
    {
        *a = i; *b = j;
    }
}

void ispisiCiklus(int i, int j, int *imin, int *jmin, int **next, int **m)
{
    printf("%d ", i + 1);
    while(i != j)
    {
        printf("%d ", j + 1);
        dodeliMinVrendost(j, next[j][j], imin, jmin, m);
        j = next[j][j];
    }
}

void izvrsiKompenzaciju(int i, int j, int min, int **next, int **m)
{
    m[i][j] -= min;
    while(i != j)
    {
        m[j][next[j][j]] -= min;
        j = next[j][j];
    }
}

void floydWarshall(int n, int **t, int **next)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                if (t[j][k] > t[j][i] + t[i][k])
                {
                    next[j][k] = next[j][i];
                    t[j][k] = t[j][i] + t[i][k];
                }
}

void zad2()
{
    int n, x, y, count = 0, **m, **p, **a;
    char s[10];
    printf("\nZAD 2\nunesite ime datoteke:\n");
    scanf_s("%s", s);

    m = ucitajFajl(s, &n);
    if(!m)
        return;
    printf("(1) POCETNA MATRICA:\n");
    matricaToLinearizovana(n, m);

    p = malloc(n * sizeof(int *));
    a = malloc(n * sizeof(int *));
    for(int i = 0; i < n; i++)
    {
        p[i] = calloc(n, sizeof(int));
        a[i] = calloc(n, sizeof(int));
    }

    dodelaVrednosti(n, p, a, m);
    floydWarshall(n, p, a);

    printf("\n\n(2) CIKLUSI I KOMPENZACIJA\n");
    for(int k = 0; k < n; k++)
    {
        x = k, y = a[k][k];
        if(a[k][k] != -1)
        {
            ispisiCiklus(k, a[k][k], &x, &y, a, m);
            printf("\npronadjen je ciklus!\nmaksimalna komenzacija je %d\n\n", m[x][y]);
            izvrsiKompenzaciju(k, a[k][k], m[x][y], a, m);

            count++;

            dodelaVrednosti(n, p, a, m);
            floydWarshall(n, p, a);
        }
    }

    if(!count)
        printf("u grafu ne postoje ciklusi\n");

    printf("\n\n(3) KRAJNJA MATRICA\n");
    matricaToLinearizovana(n, m);
}



int main() {
    int n;
    printf("Dobar dan! : )\n");
    while(1)
    {
        printf("\nGLAVNI MENI\nIzaberite zadatak koj zelite da testirate:\n\t(1) implementacija osnovnih algoritama za rad nad grafom\n\t(2) multilateralna komenzacija\n\t(0) prekid programa\n\n");
        scanf_s("%d", &n);
        switch(n)
        {
            case 1:
                zad1();
                break;

            case 2:
                zad2();
                printf("\nIzaberite jednu od sledecih stavki:\n\t(1) povratak na glavni meni\n\t(0) prekid programa\n\n");
                scanf_s("%d", &n);
                if(n)
                    break;

            case 0:
                printf("Prijatan dan! :)");
                return 0;

            default:
                printf("izabrali ste nepostojecu opciju, pokusajte ponovo\n");
        }
    }
}
