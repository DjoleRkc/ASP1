#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

int n, m, sizeOfRules, numOfSolutions;

//rules - pravila igranja
//matrix - svi pojmovi

typedef struct node
{
	int level, id, numOfChildren;
	struct node** children, * parent;
	int isValid;
	char*** currentState;
	bool** used;
} Node;

typedef struct queueNode {
	Node* info;
	struct queueNode* next;
} QueueNode;

typedef struct queue {
	QueueNode* front, * rear;
} Queue;


void insert(Node* info, Queue* q) {
	QueueNode* new = malloc(sizeof(QueueNode));
	new->info = info;
	new->next = NULL;
	if (!q->rear) q->front = new;
	else q->rear->next = new;
	q->rear = new;

}

Node* dequeue(Queue* q) {
	if (q->front) {
		QueueNode* tmp = q->front;
		q->front = q->front->next;
		if (!q->front) q->rear = NULL;
		return tmp->info;
	}
	return NULL;
}

bool isEmpty(Queue* q) {
	return !q->front;
}

Node* createNode(Node* p, char*** currentState, int level, bool** used) {
	Node* new = malloc(sizeof(Node));
	new->children = calloc(n * n * (m), sizeof(Node));
	new->parent = p;
	new->level = level;
	new->currentState = currentState;
	new->isValid = new->id = new->numOfChildren = 0;
	new->used = used;
	return new;
}

//fja za kopiranje trenutnog stanja igre
char*** copyState(char*** currentState) {
	char*** new = malloc(m * sizeof(char**));
	for (size_t i = 0; i < m; i++) {
		new[i] = malloc(n * sizeof(char*));
		for (size_t j = 0; j < n; j++) {
			if (currentState[i][j]) {
				new[i][j] = malloc((strlen(currentState[i][j]) + 1) * sizeof(char));
				strcpy(new[i][j], currentState[i][j]);
			}
			else new[i][j] = NULL;
		}
	}
	return new;
}


//fja za kopiranje matrice koji pojmovi su vec iskorisceni
bool** copyUsed(bool** used) {
	bool** new = malloc(m * sizeof(bool*));
	for (int i = 0; i < m; i++) {
		new[i] = malloc(n * sizeof(bool));
		for (int j = 0; j < n; j++) new[i][j] = used[i][j];
	}
	return new;
}

void printMatrix(char*** currentState) {
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++) printf("%10s ", currentState[i][j] ? currentState[i][j] : "-");
		putchar('\n');
	}
	putchar('\n');
}

void printTree(Node* root) {
	int oldLevel = -1;
	Queue* q = malloc(sizeof(Queue));
	q->front = q->rear = NULL;
	insert(root, q);
	while (!isEmpty(q)) {
		Node* curr = dequeue(q);
		if (curr->level > oldLevel) printf("\n\n\n\n\n\n\n\t\tNivo: %d\n\n\n\n\n\n", curr->level);
		oldLevel = curr->level;
		printf("\nID: %d PAR_ID: %d NUM_OF_CHILDREN: %d IS_VALID: %s LEADS_TO_SOLUTION: %s\n", curr->id, curr->parent ? curr->parent->id: -1, curr->numOfChildren, curr->isValid != -1 ? "YES" : "NO", curr->isValid == 1 ? "YES" : "NO");
		printMatrix(curr->currentState);
		for (size_t i = 0; i < n * n * m; i++)
		{
			if (curr->children[i]) insert(curr->children[i], q);
		}
	}

}

char*** readFile(FILE* file, char*** matrix) {
	char** tmp = malloc(300 * sizeof(char));
	fscanf(file, "%d\n%d\n", &n, &m);
	for (int i = 0; i < m; i++) {
		matrix[i] = malloc(n * sizeof(char*));
		fscanf(file, "%s\n", tmp);
		char** token = strtok(tmp, ",");
		for (int j = 0; j < n; j++)
		{
			matrix[i][j] = malloc(100 * sizeof(char));
			strcpy(matrix[i][j], token);
			matrix[i][j] = realloc(matrix[i][j], strlen(matrix[i][j]) + 1);
			token = strtok(NULL, ",");
		}
	}
	char*** rules = malloc(50 * sizeof(char**));
	sizeOfRules = 0;
	rules[0] = malloc(3 * sizeof(char*));
	rules[0][0] = malloc(100 * sizeof(char));
	rules[0][1] = malloc(100 * sizeof(char));
	rules[0][2] = malloc(2 * sizeof(char));
	while (fscanf(file, "%[^ +-]%c%[^\n]\n", rules[sizeOfRules][0], rules[sizeOfRules][2], rules[sizeOfRules][1]) == 3)
	{
		rules[sizeOfRules][0] = realloc(rules[sizeOfRules][0], strlen(rules[sizeOfRules][0]) + 1);
		rules[sizeOfRules][1] = realloc(rules[sizeOfRules][1], strlen(rules[sizeOfRules][1]) + 1);
		rules[sizeOfRules][2][1] = '\0';
		rules[++sizeOfRules] = malloc(3 * sizeof(char*));
		rules[sizeOfRules][0] = malloc(100 * sizeof(char));
		rules[sizeOfRules][1] = malloc(100 * sizeof(char));
		rules[sizeOfRules][2] = malloc(2 * sizeof(char));
	}
	return rules;
}

char*** readConsole(char*** matrix) {
	char** tmp = malloc(300 * sizeof(char));
	printf("Unesi n i m:\n");
	scanf("%d\n%d", &n, &m);
	printf("\nUnesi sve pojmove odvojene razmacima:\n");
	for (int i = 0; i < m; i++) {
		matrix[i] = malloc(n * sizeof(char*));
		for (int j = 0; j < n; j++)
		{
			matrix[i][j] = malloc(100 * sizeof(char));
			scanf("%s", matrix[i][j]);
			matrix[i][j] = realloc(matrix[i][j], strlen(matrix[i][j]) + 1);
		}
	}
	char*** rules = malloc(50 * sizeof(char**));
	char** tmp2 = malloc(100 * sizeof(char));
	sizeOfRules = 0;
	printf("Unesi sva pravila:\n");
	getchar();
	while (scanf("%[^ \n]", tmp2) == 1)
	{
		getchar();
		rules[sizeOfRules] = malloc(3 * sizeof(char*));
		rules[sizeOfRules][0] = malloc(100 * sizeof(char));
		rules[sizeOfRules][1] = malloc(100 * sizeof(char));
		rules[sizeOfRules][2] = malloc(2 * sizeof(char));
		sscanf(tmp2, "%[^ +-]%c%s", rules[sizeOfRules][0], rules[sizeOfRules][2], rules[sizeOfRules][1]);
		rules[sizeOfRules][0] = realloc(rules[sizeOfRules][0], strlen(rules[sizeOfRules][0]) + 1);
		rules[sizeOfRules][1] = realloc(rules[sizeOfRules][1], strlen(rules[sizeOfRules][1]) + 1);
		rules[sizeOfRules++][2][1] = '\0';

	}
	return rules;
}

//fja koja pronalazi vrstu u kojoj se nalazi pojam u matrici svih pojmova
int findRow(char* term, char*** matrix) {
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < n; j++) if (strcmp(matrix[i][j], term) == 0) return i;
	}
	return -1;
}

//fja koja pronalazi kolonu u kojoj se nalazi pojam u matrici svih pojmova
int findColumn(char* term, char*** matrix) {
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < n; j++) if (strcmp(matrix[i][j], term) == 0) return j;
	}

	return -1;
}

//fja koja proverava da li je trenutni cvor validno stanje ili ne - tj da li je u tr. cvoru prekrseno pravilo igre
int isValid(char*** currentState, int row, int column, char*** rules, char* term, char*** matrix) {
	for (size_t i = 0; i < m; i++)
	{
		if (currentState[i][column]) {
			for (size_t j = 0; j < sizeOfRules; j++)
			{
				if (strcmp(rules[j][0], term) == 0 && strcmp(rules[j][1], currentState[i][column]) == 0 && strcmp(rules[j][2], "-") == 0) return -1;
				if (strcmp(rules[j][1], term) == 0 && strcmp(rules[j][0], currentState[i][column]) == 0 && strcmp(rules[j][2], "-") == 0) return -1;
				if (strcmp(rules[j][0], term) == 0 && strcmp(rules[j][1], currentState[i][column]) != 0 && i == findRow(rules[j][1], matrix) && strcmp(rules[j][2], "+") == 0) return -1;
				if (strcmp(rules[j][1], term) == 0 && strcmp(rules[j][0], currentState[i][column]) != 0 && i == findRow(rules[j][0], matrix) && strcmp(rules[j][2], "+") == 0) return -1;
				if (strcmp(rules[j][0], term) != 0 && strcmp(rules[j][1], currentState[i][column]) == 0 && row == findRow(rules[j][0], matrix) && strcmp(rules[j][2], "+") == 0) return -1;
				if (strcmp(rules[j][1], term) != 0 && strcmp(rules[j][0], currentState[i][column]) == 0 && row == findRow(rules[j][1], matrix) && strcmp(rules[j][2], "+") == 0) return -1;
			}
		}

	}
	return 0;

}

void createTree(Node* root, char*** rules, char*** matrix) {
	int id = 1;
	Queue* q = malloc(sizeof(Queue));
	q->front = q->rear = NULL;
	insert(root, q);

	while (!isEmpty(q)) {

		Node* curr = dequeue(q);
		if (curr->isValid == -1) continue;
		int childrenCounter = 0;

		for (size_t i = 1; i < m; i++)
		{
			for (size_t j = 0; j < n; j++)
			{
				if (!curr->currentState[i][j]) {
					for (size_t l = 0; l < n; l++)
					{
						if (!curr->used[i][l]) {
							Node* child = createNode(curr, copyState(curr->currentState), curr->level + 1, copyUsed(curr->used));
							child->id = id++;
							curr->numOfChildren++;
							child->isValid = isValid(child->currentState, i, j, rules, matrix[i][l], matrix);
							child->currentState[i][j] = malloc(strlen(matrix[i][l]) * sizeof(char));
							strcpy(child->currentState[i][j], matrix[i][l]);
							child->used[i][l] = true;
							curr->children[childrenCounter++] = child;
						}
					}
				}
			}
		}

		if (!childrenCounter && curr->isValid == 0) {
			Node* temp = curr;
			while (temp) {
				temp->isValid = 1;
				temp = temp->parent;
			}
		}
		for (size_t i = 0; i < childrenCounter; i++)
		{
			if (curr->children[i]) insert(curr->children[i], q);
		}


	}
}

//fja koja proverava da li su dve matrice stanja jednake
bool equalStates(char*** state1, char*** state2) {
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			if ((state1[i][j] && state2[i][j] && strcmp(state1[i][j], state2[i][j]) != 0) || (!state1[i][j] && state2[i][j]) || (state1[i][j] && !state2[i][j])) return false;
		}
	}

	return true;
}

void printAllSolutions(Node* root, char**** solutions) {
	Queue* q = malloc(sizeof(Queue));
	q->front = q->rear = NULL;
	insert(root, q);
	while (!isEmpty(q)) {
		Node* curr = dequeue(q);
		int childrenCounter = 0;
		for (size_t i = 0; i < n * n * m; i++)
		{
			if (curr->children[i]) {
				insert(curr->children[i], q);
				childrenCounter++;
			}
		}

		if (!childrenCounter && curr->isValid == 1) {
			bool found = true;
			for (size_t i = 0; i < numOfSolutions; i++)
			{
				if (equalStates(curr->currentState, solutions[i])) { found = false; break; }
			}
			if (found) {
				solutions[numOfSolutions++] = copyState(curr->currentState);
				int* path = malloc(curr->level * sizeof(int));
				Node* parent = curr;
				for (int i = curr->level - 1; i >= 0; i--)
				{
					for (size_t j = 0; j < n * n * m; j++)
					{
						if (equalStates(parent->currentState, parent->parent->children[j]->currentState)) { path[i] = j; break; }
					}
					parent = parent->parent;
				}
				printf("Put do %d. resenja je:\n", numOfSolutions);
				int i = 0;
				while (parent != curr) {
					printMatrix(parent->currentState);
					printf("\t\t   |\n");
					printf("\t\t   |\n");
					printf("\t\t   |\n");
					printf("\t\t  \\/\n");
					parent = parent->children[path[i++]];
				}
				printf("Konacno resenje je:\n");
				printMatrix(curr->currentState);
			}

		}
	}
}

//levelOrder helper
//void levelOrder(Node* root) {
//	Queue* q = malloc(sizeof(Queue));
//	q->front = q->rear = NULL;
//	insert(root, q);
//	while (!isEmpty(q)) {
//		Node* curr = dequeue(q);
//		for (size_t i = 0; i < n * n * m; i++)
//		{
//			if (curr->children[i]) insert(curr->children[i], q);
//		}
//	}
//}

//Formatted tree
void printFormattedTree(Node* root) {
	int br = 0;
	char*** matrix = malloc(m * sizeof(char**));
	for (size_t k = 0; k < m; k++)
	{
		br = 0;
		matrix[k] = malloc(n * n * (m - 1) * n * sizeof(char*));
		for (size_t j = 0; j < n; j++)
		{
			for (size_t i = 0; i < 5; i++)
			{
				if (i < 4 && root->children[i]->currentState[k][j]) {
					matrix[k][br] = malloc((strlen(root->children[i]->currentState[k][j]) + 1) * sizeof(char));
					strcpy(matrix[k][br++], root->children[i]->currentState[k][j]);
				}

				else if (i < 4) matrix[k][br++] = NULL;
				else if(i==4 && root->children[n * n * (m - 1) - 1]->currentState[k][j]) {
					matrix[k][br] = malloc(6 * sizeof(char));
					strcpy(matrix[k][br++], "\t...\t");

					matrix[k][br] = malloc((strlen(root->children[n*n*(m-1)-1]->currentState[k][j]) + 1) * sizeof(char));
					strcpy(matrix[k][br++], root->children[n * n * (m - 1) - 1]->currentState[k][j]);
				}

				else matrix[k][br++] = NULL;
			}
		}
	}
	printMatrix(root->currentState);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < br; j++) printf("%10s ", matrix[i][j] ? matrix[i][j] : "-");
		putchar('\n');
	}
	putchar('\n');
}


int main() {
	//isValid - 0 ne vodi resenju, 1 - vodi res. -1 prekrseno pravilo
	int value = 1;
	bool leftToPair = true;
	char**** solutions = malloc(20 * sizeof(char***));

	char*** rules, *** matrix = malloc((m) * sizeof(char**));
	int option;
	char* fileName = malloc(100 * sizeof(char)), * column = malloc(50 * sizeof(char)), * term = malloc(50 * sizeof(char));
	while (true) {
		printf("Unesi 0 za unos iz fajla ili 1 za unos sa konzole: ");
		scanf("%d", &option);
		if (option == 0) {
			printf("Unesi ime fajla koji zelis da ucitas: ");
			scanf("%s", fileName);
			FILE* f = fopen(fileName, "r");
			if (!f) {
				printf("Fajl ne postoji, pokusaj ponovo\n");
				continue;
			}
			rules = readFile(f, matrix);
			break;
		}

		else if (option == 1) {
			rules = readConsole(matrix);
			break;
		}

		else {
			printf("Nisi uneo dobru opciju, pokusaj ponovo.\n");
		}
	}

	char*** currentState = malloc(m * sizeof(char**));
	bool** used = malloc(m * sizeof(bool*));

	for (int i = 0; i < m; i++) {
		currentState[i] = malloc(n * sizeof(char*));
		for (int j = 0; j < n; j++)
		{
			if (i == 0) {
				currentState[i][j] = malloc(strlen((matrix[i][j]) + 1) * sizeof(char));
				strcpy(currentState[i][j], matrix[i][j]);

			}
			else currentState[i][j] = NULL;
		}
	}

	for (int i = 0; i < m; i++) used[i] = calloc(n, sizeof(bool));

	Node* root = createNode(NULL, currentState, 0, used);
	Node* curr = root;
	createTree(root, rules, matrix);

	printf("Stablo je kreirano!\n");

	while (true) {
		printf("\n\n0) Prekini program\n1) Ispisi stablo\n2) Ispisi sva resenja\n3) Odigraj sledeci potez\n4) Da li si na dobrom putu?\n5) Pomoc prijatelja\n\n");
		scanf("%d", &option);
		switch (option) {
		case 0:
			printf("Kraj igre.\n");
			exit(0);
		case 1:
			printTree(root);
			break;
		case 2:
			numOfSolutions = 0;
			if (root->isValid == 0) printf("Nema resenja.\n");
			printAllSolutions(root, solutions);
			break;
		case 3:
			printMatrix(curr->currentState);
			printf("\nKoji potez zelis da odigras? Ako mislis da ova igra nema resenja, unesi 'nema resenja'\n");
			scanf("%s %s", column, term);
			putchar('\n');
			if (strcmp(column, "nema") == 0 && strcmp(term, "resenja") == 0 && root->isValid == 0) {
				printf("Bravo, resio si igru, trenutna igra nema nijedno resenje!\n");
				curr = root;
				break;
			}

			else if (strcmp(column, "nema") == 0 && strcmp(term, "resenja") == 0 && root->isValid != 0) {
				printf("Nisi u pravu, igra ima resenje, nastavi dalje!\n");
				printf("\nKoji potez zelis da odigras?\n");
				scanf("%s %s", column, term);
			}
			if (findRow(term, matrix) == -1 || findColumn(column, matrix) == -1) {
				printf("Ne postoji neki od pojmova koje si uneo!\n");
				break;
			}
			char*** tmp = copyState(curr->currentState);
			if (tmp[findRow(term, matrix)][findColumn(column, matrix)]) {
				printf("Pojmovi iz ove dve grupe su vec upareni, pokusaj ponovo\n");
				break;
			}
			tmp[findRow(term, matrix)][findColumn(column, matrix)] = term;

			for (size_t i = 0; i < n * n * m; i++)
			{
				if (!curr->children[i]) break;
				if (equalStates(curr->children[i]->currentState, tmp)) {
					curr = curr->children[i];
					if (curr->isValid == 1 && curr->level == (m - 1) * n) printf("Cestitamo, resio si igru!\n");
					else if (curr->level != (m - 1) * n) printf("\nStanje nakon odigranog poteza:\n");
					else printf("Stigao si do kraja igre ali nisi uspeo da nadjes resenje :(\n");
					printMatrix(curr->currentState);
					break;
				}
			}

			if (curr->level == (m - 1) * n) curr = root;


			if (curr->isValid == -1) {
				printf("Prekrsio si pravila uparivanja, igra je gotova.\n");
				curr = root;
				break;
			}
			break;
		case 4:
			if (curr->isValid == 1) printf("Trenutno stanje vodi ka resenju :)\n");
			else printf("Trenutno stanje ne vodi ka resenju :(\n");
			break;

		case 5:
			//if (curr->isValid == 0) { printf("Ne mogu ti pomoci jer trenutno stanje ne vodi ka resenju\n"); break; }

			printf("Trenutno stanje je:\n");
			printMatrix(curr->currentState);

			if (curr->isValid == 0) value = 0;

			leftToPair = true;

			for (size_t i = 0; i < n * n * m; i++)
			{
				if (!curr->children[i]) break;
				if (curr->children[i]->isValid == value) {
					curr = curr->children[i];
					leftToPair = false;
					if (curr->isValid == 1 && curr->level == (m - 1) * n) printf("Stigao si do kraja, resenje igre je:\n");
					else printf("\nStanje nakon pomocnog poteza:\n");
					printMatrix(curr->currentState);
					break;
				}
			}

			if (leftToPair) printf("Nemam vise pojmova da uparim po pravilima, ne mogu ti pomoci dalje");

			if (curr->level == (m - 1) * n) curr = root;
			break;

		default:
			printf("Nisi uneo validnu opciju, pokusaj opet\n");
		}

	}
}