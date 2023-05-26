#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct stacknode {
	int *next;
	int number;
} StackNode;

typedef struct stack {
	StackNode* top;
} Stack;

void* push(int num, Stack* s) {
	StackNode* tmp = malloc(sizeof(StackNode));
	tmp->number = num;
	tmp->next = s->top;
	s->top = tmp;

}

int pop(Stack* s) {
	if (s->top) {
		int tmp = s->top->number;
		StackNode* old = s->top;
		s->top = s->top->next;
		free(old);
		return tmp;
	}
	else return -1;
}

bool empty(Stack* s) {
	return !s->top;
}

int** createGraph(int numOfNodes) {
	int** graph = calloc(numOfNodes, sizeof(int*));
	for (size_t i = 0; i < numOfNodes; i++) graph[i] = calloc(numOfNodes, sizeof(int));
	return graph;
}

void printGraph(int numOfNodes, int** graph) {
	for (int i = 0; i < numOfNodes; i++){
		for (int j = 0; j < numOfNodes; j++) printf("%d ", graph[i][j]);
		putchar('\n');
	}
	putchar('\n');
}

int** insertNode(int** graph, int* numOfNodes) {
	graph = realloc(graph, (*numOfNodes + 1) * sizeof(int*));
	for (int i = 0; i < *numOfNodes; i++) {
		graph[i] = realloc(graph[i], (*numOfNodes + 1) * sizeof(int));
		graph[i][*numOfNodes] = 0;
	}
	graph[*numOfNodes] = calloc(*numOfNodes + 1, sizeof(int));
	(*numOfNodes)++;
	return graph;
}

int** deleteNode(int** graph, int* numOfNodes, int node) {
	for (size_t i = 0; i < *numOfNodes; i++)
	{
		for (size_t j = node; j < *numOfNodes-1; j++) graph[i][j] = graph[i][j + 1];
	}

	for (size_t i = node; i < *numOfNodes - 1; i++) graph[i] = graph[i + 1];

	graph = realloc(graph, (*numOfNodes-1) * sizeof(int*));
	for (int i = 0; i < *numOfNodes-1; i++) {
		graph[i] = realloc(graph[i], (*numOfNodes-1) * sizeof(int));
	}
	(*numOfNodes)--;
	return graph;
}

void insertBranch(int** graph, int num1, int num2) {
	if (graph[num1][num2]) printf("Grana vec postoji.\n");
	else graph[num1][num2] = 1;
}

void deleteBranch(int** graph, int num1, int num2) {
	if (!graph[num1][num2]) printf("Grana vec ne postoji.\n");
	else graph[num1][num2] = 0;
}

void deleteGraph(int** graph, int numOfNodes) {
	for (size_t i = 0; i < numOfNodes; i++) free(graph[i]);
	free(graph);
}

double** readFile(FILE* file, int* numOfNodes) {
	fscanf(file, "%d", numOfNodes);
	double** info = calloc(*numOfNodes, sizeof(double*));
	for (size_t j = 0; j < *numOfNodes; j++)
	{
		info[j] = calloc(3, sizeof(double));
		fscanf(file, "%lf %lf %lf", &info[j][0], &info[j][1], &info[j][2]);
	}
	return info;
}

int** makeGraphFromFile(int numOfNodes, double** info) {
	int** graph = createGraph(numOfNodes);
	for (size_t i = 0; i < numOfNodes; i++)
	{
		for (size_t j = 0; j < numOfNodes; j++)
		{
			if (i != j) {
				double dist = sqrt(pow(info[i][0] - info[j][0], 2) + pow(info[i][1] - info[j][1], 2));
				if (info[i][2] >= dist) insertBranch(graph, i, j);
			}
		}
	}
	return graph;
}

int dfs(int** graph, int start, int numOfNodes, int** detonated) {
	int br = 0;
	int* tmp = calloc(1, sizeof(int));
	Stack* s = malloc(sizeof(Stack));
	s->top = NULL;
	push(start, s);
	bool* visited = calloc(numOfNodes, sizeof(bool));
	while (!empty(s)) {
		int curr = pop(s);
		if (!visited[curr]) {
			tmp = realloc(tmp, ++br * sizeof(int));
			tmp[br-1] = curr;
			visited[curr] = 1;
		}
		else continue;

		for (size_t i = 0; i < numOfNodes; i++)
		{
			if (graph[curr][i]) push(i, s);
		}
	}

	*detonated = tmp;
	return br;
}

int findIndex(double x, double y, double r, double** info, int numOfNodes) {
	for (size_t i = 0; i < numOfNodes; i++)
	{
		if (info[i][0] == x && info[i][1] == y && info[i][2] == r) return i;
	}
	return -1;
}

int findMaxEfficiency(int** graph, int numOfNodes) {
	int* detonated;
	int max = INT_MIN;
	int index = -1;
	for (size_t i = 0; i < numOfNodes; i++)
	{
		if (dfs(graph, i, numOfNodes, &detonated) > max) {
			max = dfs(graph, i, numOfNodes, &detonated);
			index = i;
		}
	}
	return index;
}

void explodeRocket(int*** graph, double** info, int x, int y, int r, int numOfNodes) {
	*graph = insertNode(*graph, &numOfNodes);
	for (size_t j = 0; j < numOfNodes - 1; j++)
	{
		double dist = sqrt(pow(x - info[j][0], 2) + pow(y - info[j][1], 2));
		if (r >= dist) insertBranch(*graph, numOfNodes-1, j);
	}
	int* detonated;
	int br = dfs(*graph, numOfNodes-1, numOfNodes, &detonated);
	printf("Mine koje je raketa detonirala:\n");
	for (size_t i = 1; i < br; i++)
	{
		printf("%d(%.2lf, %.2lf, %.2lf)  ", detonated[i], info[detonated[i]][0], info[detonated[i]][1], info[detonated[i]][2]);
	}
	putchar('\n');
	*graph = deleteNode(*graph, &numOfNodes, numOfNodes - 1);
}

double findMaxArea(int** graph, int numOfNodes, double** info) {
	double area = 0;
	int mostEff = findMaxEfficiency(graph, numOfNodes);
	int* detonated;
	int br = dfs(graph, mostEff, numOfNodes, &detonated);
	for (size_t i = 0; i < br; i++)
	{
		area += M_PI * pow(info[detonated[i]][2], 2);
		for (size_t j = i+1; j < br; j++)
		{
			int k = detonated[i], l = detonated[j];
			double pr1 = pow(info[k][2], 2), pr2 = pow(info[l][2], 2);
			double dist = sqrt(pow(info[k][0] - info[l][0], 2) + pow(info[k][1] - info[l][1], 2));

			if (dist <= fabs(info[k][2] - info[l][2]) && info[k][2] >= info[l][2]) area -= M_PI * pr2;
			else if (dist <= fabs(info[k][2] - info[l][2]) && info[l][2] >= info[k][2]) area -= M_PI * pr1;
			else {
				double a = acos((pr1 - pr2 + pow(dist, 2)) / (2 * info[k][2] * dist)) * 2;
				double b = acos((pr2 - pr1 + pow(dist, 2)) / (2 * info[l][2] * dist)) * 2;
				area -= pr2 * (b - sin(b)) / 2 + pr1 * (a - sin(a)) / 2;
			}
		}
	}
	return area;
}



void main() {
	Stack* s = malloc(sizeof(Stack));
	s->top = NULL;
	int numOfNodes, menu, option, **graph = NULL, node, num1, num2, tmp, *detonated, br;
	double x, y, r;
	char fajl[20];
	double** info = NULL;
	while (true) {
		printf("Izaberi Minsko Polje ili Osnovni Graf Meni: ");
		printf("\n-1) Prekini Program");
		printf("\n0) Osnovni graf meni");
		printf("\n1) Minsko polje\n");
		scanf("%d", &menu);
		switch (menu) {
		case 0:
			graph = NULL;
			while (true) {
				printf("\n\nIzaberi opciju:\n");
				printf("0) Povratak na top-level meni\n");
				printf("1) Kreiraj matricu susednosti\n");
				printf("2) Dodaj cvor\n");
				printf("3) Ukloni cvor\n");
				printf("4) Dodaj granu\n");
				printf("5) Ukloni granu\n");
				printf("6) Ispisi matricu susednosti\n");
				printf("7) Obrisi graf iz memorije\n");
				scanf("%d", &option);
				switch (option) {
				case 0: break;
				case 1:
					printf("Unesi broj cvorova grafa: ");
					scanf("%d", &numOfNodes);
					graph = createGraph(numOfNodes);
					printf("\nMatrica je napravljena!\n");
					continue;
				case 2:
					if (graph) {
						graph = insertNode(graph, &numOfNodes);
						printf("Cvor dodat.\n");
					}
					else printf("Graf jos uvek ne postoji, prvo ga napravi.\n");
					continue;
				case 3:
					if (!graph) {
						printf("Graf ne postoji, nema sta da se ukloni.\n");
						continue;
					}
					printf("Koji cvor zelis da obrises: ");
					scanf("%d", &node);
					if (node<0 || node > numOfNodes) {
						printf("Taj cvor ne postoji, pokusaj ponovo!");
					}
					else graph = deleteNode(graph, &numOfNodes, node);
					printf("Cvor uklonjen.\n");
					continue;
				case 4:
					if (!graph) {
						printf("Graf ne postoji, ne moze da se doda grana.\n");
						continue;
					}
					printf("Unesi brojeve cvorova koje zelis da spojis: ");
					scanf("%d %d", &num1, &num2);
					insertBranch(graph, num1, num2);
					printf("Grana dodata.\n");
					continue;
				case 5:
					if (!graph) {
						printf("Graf ne postoji, ne moze da se ukloni grana.\n");
						continue;
					}
					printf("Unesi brojeve cvorova ciju vezu zelis da ukines (u jednom smeru): ");
					scanf("%d %d", &num1, &num2);
					deleteBranch(graph, num1, num2);
					printf("Grana uklonjena.\n");
					continue;
				case 6:
					if (!graph) {
						printf("Graf ne postoji, ne moze da se ispise.\n");
						continue;
					}
					printGraph(numOfNodes, graph);
					continue;
				case 7:
					if (!graph) {
						printf("Graf ne postoji, nema sta da se obrise.\n");
						continue;
					}
					deleteGraph(graph, numOfNodes);
					printf("Graf obrisan!\n");
					graph = NULL;
					continue;
				default:
					printf("Odabrna opcija ne postoji.\n");
					continue;
				}
				break;
			}
			break;
		case 1:
			graph = NULL;
			while (true)
			{
				printf("\n\nIzaberi opciju:\n");
				printf("0) Povratak na top-level meni\n");
				printf("1) Ucitaj podatke i kreiraj graf\n");
				printf("2) Pronadji efikasnost zadate mine\n");
				printf("3) Pronadji najefikasniju minu\n");
				printf("4) Eksplodiraj raketu\n");
				printf("5) Povrsina koju pokriva najefikasnija mina\n");
				scanf("%d", &option);
				switch (option) {
				case 0: break;
				case 1:
					printf("Unesi ime fajla iz kog se ucitava: ");
					scanf("%s", &fajl);
					FILE* file = fopen(fajl, "r");
					if (!file) {
						printf("Fajl ne postoji, pokusaj opet!\n");
						continue;
					}
					info = readFile(file, &numOfNodes);
					graph = makeGraphFromFile(numOfNodes, info);
					printf("Graf kreiran!\n");
					fclose(file);
					continue;
				case 2:
					if (!graph) {
						printf("Graf ne postoji, prvo ga ucitaj.\n");
						continue;
					}
					printf("Unesi broj mine za koju zelis da vidis efikasnost: ");
					scanf("%d", &tmp);
					if (tmp < 0 || tmp >= numOfNodes) {
						printf("Ne postoji zadata mina.\n");
						continue;
					}
					br = dfs(graph, tmp, numOfNodes, &detonated);
					printf("Broj mina koje su detonirane: %d\n", br);
					printf("Mine koje su detonirane: ");
					for (size_t i = 0; i < br; i++)
					{
						printf(" %d(%.2lf, %.2lf, %.2lf)  ", detonated[i], info[detonated[i]][0], info[detonated[i]][1], info[detonated[i]][2]);
					}
					putchar('\n');
					continue;
				case 3:
					if (!graph) {
						printf("Graf ne postoji, prvo ga ucitaj.\n");
						continue;
					}
					tmp = findMaxEfficiency(graph, numOfNodes);
					printf("Najefikasnija mina je mina broj %d\n", tmp);
					br = dfs(graph, tmp, numOfNodes, &detonated);
					printf("Broj mina koje ona detonira: %d\n", br);
					printf("Mine koje ona detonira: ");
					for (size_t i = 0; i < br; i++)
					{
						printf(" %d(%.2lf, %.2lf, %.2lf)  ", detonated[i], info[detonated[i]][0], info[detonated[i]][1], info[detonated[i]][2]);
					}
					putchar('\n');
					continue;
				case 4:
					if (!graph) {
						printf("Graf ne postoji, prvo ga ucitaj.\n");
						continue;
					}
					printf("Unesi x i y koordinate rakete kao i radius eksplozije: ");
					scanf("%lf %lf %lf", &x, &y, &r);
					explodeRocket(&graph, info, x, y, r, numOfNodes);
					continue;
				case 5:
					if (!graph) {
						printf("Graf ne postoji, prvo ga ucitaj.\n");
						continue;
					}
					printf("Povrsina koju pokriva najefikasnija mina je: %lf\n", findMaxArea(graph, numOfNodes, info));
					continue;
				default:
					printf("Odabrna opcija ne postoji.\n");
					continue;
				}
				break;
			}
			break;
		case -1:
			return 0;
		}
	}
}