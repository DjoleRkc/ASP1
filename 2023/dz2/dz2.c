#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

int n, m;

typedef struct node
{
	int level;
	struct node** children, * parent;
	bool isValid, isLeaf;
	char*** currentState;
	bool** used;
} Node;

typedef struct queueNode {
	Node* info;
	struct queueNode* next;
} QueueNode;

typedef struct queue {
	QueueNode* front, *rear;
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

Node* createNode(Node* p, char*** matrix, int l, bool** u) {
	Node* new = malloc(sizeof(Node));
	new->children = malloc(n * n * m * sizeof(Node));
	new->parent = p;
	new->level = l;
	new->currentState = matrix;
	new->isLeaf = new->isValid = false;
	new->used = u;
	return new;
}

char*** copyState(char*** currentState) {
	char*** new = malloc((m + 1) * sizeof(char**));
	for (size_t i = 0; i < m + 1; i++) {
		new[i] = malloc(n * sizeof(char*));
		for (size_t j = 0; j < n; j++) {
			new[i][j] = malloc((strlen(currentState[i][j]) + 1 )* sizeof(char));
			strcpy(new[i][j], currentState[i][j]);
		}
	}
	return new;
}

bool** copyUsed(bool** used) {
	bool** new = malloc(m * sizeof(bool*));
	for (int i = 0; i < m; i++) {
		new[i] = malloc(n * sizeof(bool));
		for (int j = 0; j < n; j++) new[i][j] = used[i][j];
	}
	return new;
}

void levelOrder(Node* root) {
	Queue* q = malloc(sizeof(Queue));
	q->front = q->rear = NULL;
	insert(root, q);
	while (!isEmpty(q)) {
		Node* curr = dequeue(q);
		printf("%d ", curr->level);
		for (size_t i = 0; i < n*n*m; i++)
		{
			if (curr->children[i]) insert(curr->children[i], q);
		}
	}

}

void printMatrix(char*** matrix) {
	for (int i = 0; i < m+1; i++)
	{
		for (int j = 0; j < n; j++) printf("%s ", matrix[i][j]);
		putchar('\n');
	}
}

int main() {
	n = 3, m = 2;
	char*** matrix = malloc((m + 1) * sizeof(char**));
	for (int i = 0; i < m + 1; i++) {
		matrix[i] = malloc(n * sizeof(char*));
		for (int j = 0; j < n; j++)
		{
			matrix[i][j] = malloc(100 * sizeof(char));
			scanf("%s", matrix[i][j]);
			matrix[i][j] = realloc(matrix[i][j], strlen(matrix[i][j]) + 1);
		}
	}
	printMatrix(matrix);
	/*char*** new = copyState(matrix);
	new[2][0] = "la";
	printMatrix(new);
	printMatrix(matrix);*/

	/*char* s = malloc(20*sizeof(char));
	scanf("%s", s);
	s = realloc(s, strlen(s) + 1);
	printf("%s %d", s, strlen(s));
	free(s);*/
}