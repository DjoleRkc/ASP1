
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct cvor{
    int id, nivo, br, ispravnost;
    int **stanje;
    int potezi[18][2];
    struct cvor *roditelj;
    struct cvor **dete;
}Cvor;

typedef struct queue{
    struct queue *prvi, *posl, *sl;
    Cvor *info;
}Queue;


void push(Queue *q, Cvor *info)
{
    Queue *pom = malloc(sizeof(Queue));
    pom -> info = info;
    pom -> sl = NULL;

    if (q -> posl == NULL)
        q -> prvi = q -> posl = pom;
    else
    {
        q -> posl -> sl = pom;
        q -> posl = pom;
    }
}

Cvor* pop(Queue *q)
{
    if (q -> prvi == NULL)
        return NULL;

    Queue *pom = q -> prvi;
    q -> prvi = q -> prvi -> sl;

    if (q -> prvi == NULL)
        q -> posl = NULL;

    return pom -> info;
}



int fibonacijev_generator(int* used, int* fib, int br, int k)
{
    while(1)
    {
        fib[k] = (fib[k - 7] + fib[k - 10]) % br;
        if (used[fib[k++]])
        {
            used[fib[k - 1]]--;
            return fib[k - 1] + 1;
        }
    }
}

void pocetno_stanje(int** m, int n, int k)
{
    int br = 10, used[n-k], fib[1000];
    srand(time(0));
    for(int i = 0; i < n-k; i++)
        used[i] = 4;
    for(int i = 0; i < 10; i++)
        fib[i] = rand() % 20;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < n; j++)
        {
            if(j >= n-k)
            {
                m[i][j] = 0;
                m[4][j] = 4;
            }
            else
            {
                m[i][j] = fibonacijev_generator(used, fib, n-k, br++);
                m[4][j] = 0;
            }
        }
}



int** prekopiraj(int** m, int n)
{
    int **t;
    t = malloc(5 * sizeof(int*));
    for(int i = 0; i < 5; i++)
    {
        t[i] = malloc(n * sizeof(int));
        for(int j = 0; j < n; j++)
            t[i][j] = m[i][j];
    }
    return t;
}

int** prebaci(int **m, int n, int x, int y, int br)
{
    int** p = prekopiraj(m, n);
    for(int i = 0; i < br; i++)
    {
        p[(p[4][y]--) - 1][y] = p[p[4][x]][x];
        p[(p[4][x]++)][x] = 0;
    }
    return p;
}

int broj_uzastopnih(int** m, int x)
{
    int t = 0, i = m[4][x];
    while(m[m[4][x]][x] == m[i][x] && i++ < 4)
        t++;
    return t;
}

int da_li_moze(int** m, int x, int y)
{
    if(m[4][x] == 4 || m[4][x] == 0 && broj_uzastopnih(m, x) == 4)
        return 0;
    if(m[4][y] == 4 || (m[m[4][x]][x] == m[m[4][y]][y] && broj_uzastopnih(m, x) <= m[4][y]))
        return 1;
    return 0;
}

int da_li_su_iste(int **m, int **p, int n)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < n; j++)
            if(m[i][j] != p[i][j])
                return 0;
    return 1;
}

int krajnje_resenje(int** m, int n)
{
    for(int i = 0; i < n; i++)
        if(m[m[4][i]][i] != 4 && broj_uzastopnih(m, i) != 4)
            return 0;
    return 1;
}

void ispisi_mat(int** m, int n)
{
    for (int i = 0; i < 4; i++)
        for(int j = 0; j < n; j++)
            printf("|%d|%c", m[i][j], (j == n-1)?'\n':' ');
    printf("\n");
}



void ispisi_hint(Cvor *tr, int** m, int n)
{
    int a, b;
    for(int j = 0; j < tr -> br; j++)
        if(tr -> dete[j] -> ispravnost == 1)
        {
            for (int i = 0; i < n; i++)
            {
                if(m[4][i] < tr -> dete[j] -> stanje[4][i])
                    a = i;
                if(m[4][i] > tr -> dete[j] -> stanje[4][i])
                    b = i;
            }
            printf("HINT: (%d->%d)\n\n", a, b);
            break;
        }
}

void hint(int** m, int** t, int *a, int *b, int n)
{

    for (int i = 0; i < n; i++)
    {
        if(m[4][i] < t[4][i])
            *a = i;
        if(m[4][i] > t[4][i])
            *b = i;
    }
}

int ubaci_u_stablo(Queue *queue, Cvor *root, int n, int p)
{
    int id = 1, resenje = 0;
    while(1) {
        Cvor *rod = pop(queue);
        if(rod == NULL)
            break;
        if(rod -> nivo == p)
        {
            rod -> br = 0;
            continue;
        }
        int k = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (i != j && da_li_moze(rod -> stanje, i, j))
                {
                    //if(rod != root && da_li_su_iste(rod -> roditelj -> stanje, prebaci(rod-> stanje, n, i, j, broj_uzastopnih(rod -> stanje, n)), n))
                    //continue;
                    rod -> potezi[k][0] = i;
                    rod -> potezi[k++][1] = j;
                }
        rod -> br = k;
        rod -> dete = malloc(k * sizeof(Cvor));

        for (int i = 0; i < rod -> br; i++)
        {
            Cvor *x = malloc(sizeof(Cvor)), *tr;
            x -> id = id++;
            x -> nivo = rod -> nivo + 1;
            x -> roditelj = rod;
            x -> stanje = prebaci(rod -> stanje, n, rod -> potezi[i][0], rod -> potezi[i][1], broj_uzastopnih(rod -> stanje, rod -> potezi[i][0]));
            x -> ispravnost = krajnje_resenje(x -> stanje, n);
            rod -> dete[i] = x;

            if (x -> ispravnost == 1)
            {
                tr = x;
                x -> br = 0;
                while(tr -> roditelj != NULL)
                {
                    tr -> ispravnost = 1;
                    tr = tr -> roditelj;
                }
                resenje = 1;
                continue;
            }

            push(queue, x);
        }
    }
    return resenje;
}

void ispisi_stablo(Queue *queue, Cvor *root, int n, int p)
{
    int pret = -1;
    while(1) {
        Cvor *rod = pop(queue);
        if(rod == NULL)
            break;

        if (rod -> nivo != pret)
            printf("\n\nnivo broj %d\n", pret = rod -> nivo);

        printf("%d:%d:%d ", (rod -> roditelj == NULL) ? 0 : rod -> roditelj -> id, rod -> id, rod -> ispravnost);

        for (int i = 0; i < rod -> br; i++)
            push(queue, rod -> dete[i]);
    }
    printf("\n");
}

void ispisi_resenje(Cvor *root, int n)
{
    int a, b;
    Cvor *tr = root;
    while(tr -> br != 0)
    {
        for(int j = 0; j < tr -> br; j++)
            if(tr -> dete[j] -> ispravnost == 1)
            {
                tr = tr -> dete[j];
                hint(tr -> roditelj -> stanje, tr -> stanje, &a, &b, n);
                printf("%d->%d\n", a, b);
                ispisi_mat(tr -> stanje, n);
                break;
            }
    }
}

int meni()
{
    int t;
    printf("izaberite jednu od sledecih stavki\n\t(1) unos sledeceg poteza\n\t(2) pomoc pri odabiru sledeceg poteza\n\t(3) ispis puta do resenja\n\t(4) ispis izgleda stabla\n");
    while(1)
    {
        scanf("%d", &t);
        if(t < 5 && t > 0)
            break;
        printf("\nizabrali ste nepostojecu opciju, pokusajte ponovo\n");
    }
    return t;
}



int main() {
    int n, k, p, x, y, t, resenje;
    int **m;
    printf("\nunesi ukupan broj bocica:\n");
    scanf("%d", &n);
    printf("unesi broj praznih bocica:\n");
    scanf("%d", &k);
    printf("unesi maksimalan broj pokusaja:\n");
    scanf("%d", &p);

    m = malloc(5 * sizeof(int*));
    for(int i = 0; i < 5; i++)
        m[i] = malloc(n * sizeof(int));

    printf("\n");
    pocetno_stanje(m, n, k);
    ispisi_mat(m, n);


    Cvor *root, *tr;
    root = malloc(sizeof(Cvor));
    root -> id = 0;
    root -> nivo = 0;
    root -> ispravnost = 1;
    root -> roditelj = NULL;
    root -> stanje = prekopiraj(m, n);
    tr = root;

    Queue *queue = malloc(sizeof(Queue));
    queue -> prvi = NULL;
    queue -> posl = NULL;
    push(queue, root);

    resenje = ubaci_u_stablo(queue, root, n, p);

    if(resenje == 0)
    {
        printf("nema resenja:(");
        return 0;
    }

    printf("\nS  T  A  R  T      G  A  M  E\n");
    int i = 0;
    while(i < p)
    {
        t = meni();
        switch (t)
        {
            case 1: {
                printf("\n");
                ispisi_mat(tr -> stanje, n);
                printf("preostalo je jos %d poteza\n", p-i);
                printf("unesite zeljeni potez u formatu (x->y)\n\n");
                while(1)
                {
                    scanf("%d->%d", &x, &y);
                    if(x >= n || x < 0 || y >= n || y < 0 || da_li_moze(tr -> stanje, x, y) == 0)
                        printf("nedozvoljeno prebacivanje, pokusajte ponovo\n");
                    else
                        break;
                }
                for(int j = 0; j < tr -> br; j++)
                    if(tr -> potezi[j][0] == x && tr -> potezi[j][1] == y)
                    {
                        tr = tr -> dete[j];
                        break;
                    }

                ispisi_mat(tr -> stanje, n);

                if(tr -> ispravnost == 0)
                {
                    printf("nemoguce je pobediti iz ove pozicije\nniste uspeli da resite igricu:(\n");
                    printf("\n\nG  A  M  E      O  V  E  R\n\n");
                    return 0;
                }

                if(krajnje_resenje(tr -> stanje, n))
                {
                    printf("uspesno ste resili igricu:)\n");
                    printf("\nG  A  M  E      O  V  E  R\n\n");
                    return 0;
                }
                i++;
                continue;
            }

            case 2: {
                ispisi_hint(tr, tr -> stanje, n);
                continue;
            }

            case 3: {
                ispisi_resenje(tr, n);
                continue;
            }

            case 4: {
                push(queue, tr);
                ispisi_stablo(queue, tr, n, p);
                continue;
            }

            default:
            {
                continue;
            }
        }
    }

    printf("niste uspeli da resite igricu:(\n");
    printf("\n\nG  A  M  E      O  V  E  R\n\n");

    return 0;
}
