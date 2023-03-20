#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define _CRT_SECURE_NO_WARNINGS

int bbs(int* seed) {
    int m = 2351 * 811, parityBit = 0, broj = 0, tmp = 0;
    for (size_t i = 0; i < 3; i++)
    {
        *seed = (*seed * *seed) % m;
        tmp = *seed;
        while (tmp) {
            tmp = tmp & (tmp - 1);
            parityBit = !parityBit;
        }
        broj <<= 1;
        broj += parityBit;
    }
    return broj;
}

int generateDice(int* seed) {
    int broj = bbs(seed);
    while (broj < 1 || broj>6) broj = bbs(seed);
    return broj;
}

int numExists(int kockice[5], int broj) {
    for (size_t i = 0; i < 5; i++)
        if (broj == kockice[i]) return i;
    return -1;
}

int pointsInTurn(int kockice[5], int whichTurn, int option) {
    int points = 0;
    int sums[7] = { 0 };
    for (size_t i = 0; i < 5; i++) sums[kockice[i]]++;

    if (option > 0 && option < 7) {
        for (size_t i = 0; i < 5; i++)
            if (kockice[i] == option) points += kockice[i];
        if (!points) return -1;
        return points;
    }

    if (option == 7) {
        if (sums[2] && sums[3] && sums[4] && sums[5] && (sums[1] || sums[6])) return 66 - (whichTurn) * 10;
        return -1;
    }

    if (option == 8) {
        bool two = false, three = false;
        for (size_t i = 0; i < 7; i++)
        {
            if (sums[i] == 2) { two = true; points += 2 * i; }
            if (sums[i] == 3) { three = true; points += 3 * i; }
        }
        if (two && three) return 30 + points;
        return -1;
    }

    if (option == 9) {
        for (size_t i = 0; i < 7; i++)
        {
            if (sums[i] == 4) return 40 + 4 * i;
        }
        return -1;
    }

    if (option == 10) {
        for (size_t i = 0; i < 7; i++)
        {
            if (sums[i] == 5) return 50 + 5 * i;
        }
        return -1;
    }
}

int** insert(int** matrix, int* numOfElems, int row, int column, int value) {
    if (*(numOfElems) >= 11) {
        if ((*numOfElems) == 11) {
            int** copy = malloc(11 * sizeof(int*));
            for (size_t i = 0; i < 11; i++)
            {
                copy[i] = calloc(3, sizeof(int));
                for (size_t j = 0; j < 3; j++)
                {
                    copy[i][j] = matrix[i][j];
                    matrix[i][j] = 0;
                }
            }
            for (size_t j = 0; j < 11; j++)
            {
                matrix[copy[j][0]][copy[j][1]] = copy[j][2];
            }
            (*numOfElems)++;
        }
        matrix[row][column] = value;
    }

    else {
        int i;
        matrix = realloc(matrix, (*numOfElems + 1) * sizeof(int*));
        matrix[*numOfElems] = calloc(3, sizeof(int));
        for (i = (*numOfElems); i > 0; i--)
        {
            matrix[i][0] = matrix[i - 1][0];
            matrix[i][1] = matrix[i - 1][1];
            matrix[i][2] = matrix[i - 1][2];
            if (matrix[i - 1][0] < row || (matrix[i - 1][0] == row && matrix[i - 1][1] <= column)) break;
        }
        matrix[i][0] = row;
        matrix[i][1] = column;
        matrix[i][2] = value;
        (*numOfElems)++;
    }
    return matrix;
}

double probability(int kockice[5], int *seed, int row, int broj1, int broj2, int broj3) {
    int counter = 0, array[5] = { 0 };
    for (size_t i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 5; j++) array[j] = kockice[j];
        for (size_t j = 0;  j < 2;  j++)
        {
            for (int k = 0; k < 5; k++) {
                if ((row == 8 || row == 9) && array[k] != broj1) array[k] = generateDice(seed);
                if (row == 7 && (array[k] != broj1 && array[k] != broj2)) array[k] = generateDice(seed);
                if (row == 6 && array[k] == broj3) { array[k] = generateDice(seed); break; }
            }
        }
        if (pointsInTurn(array, 0, row + 1) != -1) counter++;
    }
    if (row == 6) counter <<= 1;
    return (double)(counter) / 10000;
}

void printMatrix(int** matrix, int numOfElems) {
    int sums[3] = { 0 };
    printf("\n     NA DOLE NA GORE  RUCNA\n");
    if (numOfElems > 11) {
        for (size_t i = 0; i < 11; i++)
        {
            if (i < 6) printf("%d\t", i + 1);
            else if (i == 6) { printf("SUMA \t%d\t%d\t%d\n\n", sums[0], sums[1], sums[2]); printf("KENTA\t"); }
            else if (i == 7) printf("FUL  \t");
            else if (i == 8) printf("POKER\t");
            else if (i == 9) printf("JAMB \t");
            else if (i == 10) { printf("SUMA \t%d\t%d\t%d\n", sums[0], sums[1], sums[2]); break; }
            for (size_t j = 0; j < 3; j++)
            {
                if (matrix[i][j] == -1) printf("/ \t");
                else {
                    printf("%d\t", matrix[i][j]);
                    sums[j] += matrix[i][j];
                }
            }
            putchar('\n');
        }
        putchar('\n');
    }
    else {
        int br2 = 0;
        for (size_t i = 0; i < 11; i++)
        {
            if (i < 6) printf("%d\t", i + 1);
            else if (i == 6) { printf("SUMA \t%d\t%d\t%d\n\n", sums[0], sums[1], sums[2]); printf("KENTA\t"); }
            else if (i == 7) printf("FUL  \t");
            else if (i == 8) printf("POKER\t");
            else if (i == 9) printf("JAMB \t");
            else if (i == 10) { printf("SUMA \t%d\t%d\t%d\n", sums[0], sums[1], sums[2]); break; }
            for (size_t j = 0; j < 3; j++)
            {
                if (br2 < numOfElems && matrix[br2][0] == i && matrix[br2][1] == j) {
                    if (matrix[br2][2] == -1) printf("/ \t");
                    else {
                        printf("%d\t", matrix[br2][2]);
                        sums[j] += matrix[br2][2];
                    }
                    br2++;
                }
                else printf("0\t");
            }
            putchar('\n');
        }
        putchar('\n');
    }

}

int findRowToFill(int** matrix, int column, int numOfElems) {
    if (numOfElems > 11) {
        if (column == 0) {
            for (size_t i = 0; i < 10; i++)
                if (matrix[i][column] == 0) return i;
        }

        else if (column == 1) {
            for (int i = 9; i >= 0; i--)
                if (matrix[i][column] == 0) return i;
        }

        else {
            for (size_t i = 0; i < 10; i++)
                if (matrix[i][column] == 0) return 1;
        }
    }
    else {
        if (column == 0) {
            int br = 0;
            for (size_t i = 0; i < numOfElems; i++)
                if (matrix[i][1] == 0) br++;
            if (br < 11) return br;
            return -1;
        }
        if (column == 1) {
            int br = 9;
            for (size_t i = 0; i < numOfElems; i++)
                if (matrix[i][1] == 1) br--;
            if (br >= 0) return br;
            return -1;
        }
        else {
            int br = 0;
            for (size_t i = 0; i < numOfElems; i++)
                if (matrix[i][1] == 2) br++;
            if (br < 10) return 1;
            return -1;
        }
    }
}

void rucnaMenu(int** matrix, int numOfElems) {
    putchar('\n');
    if (numOfElems > 11) {
        for (size_t i = 0; i < 10; i++)
        {
            if (!matrix[i][2]) {
                printf("%d - ", i + 1);
                switch (i + 1)
                {
                case 7:
                    printf("KENTA\n");
                    break;
                case 8:
                    printf("FUL\n");
                    break;
                case 9:
                    printf("POKER\n");
                    break;
                case 10:
                    printf("JAMB\n");
                    break;
                default: printf("%d\n", i + 1);
                }
            }
        }
    }
    else {
        int array[11] = { 0 };
        for (size_t i = 0; i < numOfElems; i++)
        {
            if (matrix[i][1] == 2) array[matrix[i][0]] = 1;
        }
        for (size_t i = 0; i < 10; i++)
        {
            if (!array[i]) {
                printf("%d - ", i + 1);
                switch (i + 1)
                {
                case 7:
                    printf("KENTA\n");
                    break;
                case 8:
                    printf("FUL\n");
                    break;
                case 9:
                    printf("POKER\n");
                    break;
                case 10:
                    printf("JAMB\n");
                    break;
                default: printf("%d\n", i + 1);
                }
            }
        }
    }
}

int** friendAssistUp(int** matrix, int* numOfElems, int kockice[5], int row, bool column, int* seed) {
    printf("Drugo Bacanje: ");
    for (size_t i = 0; i < 5; i++)
    {
        if (kockice[i] != row + 1) kockice[i] = generateDice(seed);
        printf("%d ", kockice[i]);
    }
    printf("\nTrece Bacanje: ");
    for (size_t i = 0; i < 5; i++)
    {
        if (kockice[i] != row + 1) kockice[i] = generateDice(seed);
        printf("%d ", kockice[i]);
    }
    matrix = insert(matrix, numOfElems, row, column, pointsInTurn(kockice, 2, row + 1));
    return matrix;
}

int** friendAssistDown(int** matrix, int* numOfElems, int kockice[5], int row, bool column, int* seed, int broj1, int broj2, int broj3) {
    if (row == 8 || row == 9) {
        if (row == 8) printf("POKER ");
        else printf("JAMB ");
        if (column) printf("U KOLONI NA GORE\n");
        else printf("U KOLONI NA DOLE\n");
        printf("Drugo Bacanje: ");
        if (pointsInTurn(kockice, 1, row + 1) == -1) {
            for (size_t i = 0; i < 5; i++)
            {
                if (kockice[i] != broj1) kockice[i] = generateDice(seed);
                printf("%d ", kockice[i]);
            }
        }
        if (pointsInTurn(kockice, 1, row + 1) == -1) {
            printf("\nTrece Bacanje: ");
            for (size_t i = 0; i < 5; i++)
            {
                if (kockice[i] != broj1) kockice[i] = generateDice(seed);
                printf("%d ", kockice[i]);
            }
        }
        matrix = insert(matrix, numOfElems, row, column, pointsInTurn(kockice, 2, row + 1));
    }
    if (row == 7) {
        printf("FUL ");
        if (column) printf("U KOLONI NA GORE\n");
        else printf("U KOLONI NA DOLE\n");
        printf("Drugo Bacanje: ");
        if (pointsInTurn(kockice, 1, row + 1) == -1) {
            for (size_t i = 0; i < 5; i++)
            {
                if (kockice[i] != broj1 && kockice[i] != broj2) kockice[i] = generateDice(seed);
                printf("%d ", kockice[i]);
            }
        }
        if (pointsInTurn(kockice, 1, row + 1) == -1) {
            printf("\nTrece Bacanje: ");
            for (size_t i = 0; i < 5; i++)
            {
                if (kockice[i] != broj1 && kockice[i] != broj2) kockice[i] = generateDice(seed);
                printf("%d ", kockice[i]);
            }
        }
        matrix = insert(matrix, numOfElems, row, column, pointsInTurn(kockice, 2, row + 1));
    }
    if (row == 6)
    {
        printf("KENTA ");
        if (column) printf("U KOLONI NA GORE\n");
        else printf("U KOLONI NA DOLE\n");
        printf("Drugo Bacanje: ");
        if (pointsInTurn(kockice, 1, row + 1) == -1) {
            for (size_t i = 0; i < 5; i++)
            {
                if (kockice[i] == broj3) { kockice[i] = generateDice(seed); break; }
            }
            for(int i = 0; i < 5; i++)
                printf("%d ", kockice[i]);
        }
        if (pointsInTurn(kockice, 1, row + 1) == -1) {
            printf("\nTrece Bacanje: ");
            for (size_t i = 0; i < 5; i++)
            {
                if (kockice[i] == broj3) { kockice[i] = generateDice(seed); break; }
            }
            for (int i = 0; i < 5; i++)
                printf("%d ", kockice[i]);
        }
        matrix = insert(matrix, numOfElems, row, column, pointsInTurn(kockice, 2, row + 1));
    }
    return matrix;
}

int** friendAssist(int** matrix, int* numOfElems, int kockice[5], int whichTurn, int rowToFillDown, int rowToFillUp, int* seed) {
    bool free = false;
    int br = 0, broj3 = 0;
    if (*numOfElems > 11) {
        for (size_t i = 0; i < 6; i++)
        {
            if (!matrix[i][2] && numExists(kockice, i + 1) != -1) {
                free = true;
                break;
            }
        }
    }
    else {
        int niz[11] = { 0 };
        for (size_t i = 0; i < *numOfElems; i++)
            if (matrix[i][1] == 2) niz[matrix[i][0]] = 1;
        for (int i = 0; i < 6; i++)
            if (!niz[i] && numExists(kockice, i + 1) != -1) free = true;
    }

    int sums[7] = { 0 }, max = 0, broj = 0, broj2 = 0;
    for (size_t i = 0; i < 5; i++) {
        sums[kockice[i]]++;
        if (sums[kockice[i]] > max) {
            max = sums[kockice[i]];
            broj = kockice[i];
        }
    }
    max = 0;
    for (size_t i = 0; i < 5; i++) {
        if (sums[kockice[i]] > max && kockice[i] != broj) {
            max = sums[kockice[i]];
            broj2 = kockice[i];
        }
        if (sums[kockice[i]] > 1) broj3 = kockice[i];
    }
    if (!broj3 && numExists(kockice, 1) != -1) broj3 = 1;
    if (!broj3 && numExists(kockice, 6) != -1) broj3 = 6;

    if (!whichTurn && pointsInTurn(kockice, whichTurn, 10) != -1) {
        printf("IGRA SE JAMB U KOLONI RUCNA\n");
        matrix = insert(matrix, numOfElems, 9, 2, pointsInTurn(kockice, whichTurn, 10));
    }
    else if (!whichTurn && pointsInTurn(kockice, whichTurn, 9) != -1) {
        printf("IGRA SE POKER U KOLONI RUCNA\n");
        matrix = insert(matrix, numOfElems, 8, 2, pointsInTurn(kockice, whichTurn, 9));
    }
    else if (!whichTurn && pointsInTurn(kockice, whichTurn, 8) != -1) {
        printf("IGRA SE FUL U KOLONI RUCNA\n");
        matrix = insert(matrix, numOfElems, 7, 2, pointsInTurn(kockice, whichTurn, 8));
    }
    else if (!whichTurn && pointsInTurn(kockice, whichTurn, 7) != -1) {
        printf("IGRA SE KENTA U KOLONI RUCNA\n");
        matrix = insert(matrix, numOfElems, 6, 2, pointsInTurn(kockice, whichTurn, 7));
    }
    else if (!whichTurn && *numOfElems > 11 && free) {
        for (size_t i = 0; i < 6; i++)
        {
            if (matrix[i][2] == 0 && sums[i]) { printf("IGRA SE %d U KOLONI RUCNA\n", i + 1); matrix = insert(matrix, numOfElems, i, 2, pointsInTurn(kockice, whichTurn, i + 1)); break; }
        }
    }
    else if (!whichTurn && *numOfElems <= 11 && free) {
        int array[11] = { 0 };
        for (size_t i = 0; i < *numOfElems; i++)
        {
            if (matrix[i][1] == 2) array[matrix[i][0]] = 1;
        }
        for (size_t i = 0; i < 6; i++)
        {
            if (array[i] == 0 && sums[i + 1]) { printf("IGRA SE %d U KOLONI RUCNA\n", i + 1); matrix = insert(matrix, numOfElems, i, 2, pointsInTurn(kockice, whichTurn, i + 1)); break; }
        }
    }
    else if (rowToFillDown < 6 && rowToFillUp < 6) {
        if (sums[rowToFillDown + 1] >= sums[rowToFillUp + 1]) { printf("IGRA SE %d U KOLONI NA DOLE\n", rowToFillDown + 1); matrix = friendAssistUp(matrix, numOfElems, kockice, rowToFillDown, 0, seed); }
        else { printf("IGRA SE %d U KOLONI NA GORE\n", rowToFillUp + 1); matrix = friendAssistUp(matrix, numOfElems, kockice, rowToFillUp, 1, seed); }
    }
    else if (rowToFillDown < 6 && rowToFillUp >= 6) {
        if (probability(kockice, seed, rowToFillUp, broj, broj2, broj3) > 0.2) { printf("IGRA SE "); matrix = friendAssistDown(matrix, numOfElems, kockice, rowToFillUp, 1, seed, broj, broj2, broj3); }
        else { printf("IGRA SE %d U KOLONI NA DOLE\n", rowToFillDown + 1); matrix = friendAssistUp(matrix, numOfElems, kockice, rowToFillDown, 0, seed); }
    }
    else if (rowToFillDown >= 6 && rowToFillUp < 6) {
        if (probability(kockice, seed, rowToFillDown, broj, broj2, broj3) > 0.2) { printf("IGRA SE "); matrix = friendAssistDown(matrix, numOfElems, kockice, rowToFillDown, 0, seed, broj, broj2, broj3); }
        else { printf("IGRA SE %d U KOLONI NA GORE\n", rowToFillUp + 1); matrix = friendAssistUp(matrix, numOfElems, kockice, rowToFillUp, 1, seed); }
    }
    else {
        if (probability(kockice, seed, rowToFillDown, broj, broj2, broj3) > probability(kockice, seed, rowToFillUp, broj, broj2, broj3)) { printf("IGRA SE "); matrix = friendAssistDown(matrix, numOfElems, kockice, rowToFillDown, 0, seed, broj, broj2, broj3); }
        else { printf("IGRA SE "); matrix = friendAssistDown(matrix, numOfElems, kockice, rowToFillUp, 1, seed, broj, broj2, broj3); }
    }
    return matrix;
}

int main() {
    srand(time(0));
    int seed = time(0) % 10000 + 1, ** board = malloc(sizeof(int*)), numOfElems = 0, i, kockice[5], option, column, row;
    board[0] = calloc(3, sizeof(int));
    for (int k = 0; k < 30; k++) {
        for (i = 0; i < 5; i++) kockice[i] = generateDice(&seed);

        for (i = 0; i < 3; i++)
        {
            printf("Bacene su sledece kockice: ");
            for (size_t j = 0; j < 5; j++) printf("%d ", kockice[j]);
            putchar('\n');
            if (i == 2) break;
            printf("Da li zelis ponovo da bacas?\n1-DA\n0-NE\n");
            int nextThrow;
            scanf("%d", &nextThrow);
            if (nextThrow != 1) break;

            printf("\nKoliko kockica zelis ponovo da bacis? (1-5)\n");
            int nextThrowNumOfDice;
            scanf("%d", &nextThrowNumOfDice);
            while (nextThrowNumOfDice < 1 || nextThrowNumOfDice>5) {
                printf("\nKoliko kockica zelis ponovo da bacis? (1-5)\n");
                scanf("%d", &nextThrowNumOfDice);
            }

            for (size_t j = 0; j < nextThrowNumOfDice; j++)
            {
                printf("\nKoju kockicu zelis ponovo da bacis?\n");
                int diceAgain;
                scanf("%d", &diceAgain);
                if (numExists(kockice, diceAgain) == -1) {
                    printf("Odabrana kockica ne postoji, pokusaj ponovo.\n");
                    j--;
                }
                else {
                    kockice[numExists(kockice, diceAgain)] = generateDice(&seed);
                }
            }
        }

        printf("Da li zelis da uneses:\n0 - KRAJ IGRE");
        if (findRowToFill(board, 0, numOfElems) != -1) printf("\n1 - NA DOLE");
        if (findRowToFill(board, 1, numOfElems) != -1) printf("\n2 - NA GORE");
        if (findRowToFill(board, 2, numOfElems) != -1 && i == 0) printf("\n3 - RUCNA");
        printf("\n4 - POMOC PRIJATELJA\n");
        scanf("%d", &column);
        if (!column) {
            printf("\n\nGOTOVA IGRA! :)\n\n    KONACAN REZULTAT\n");
            printMatrix(board, numOfElems);
            exit(0);
        }
        if (column == 4)
        {
            board = friendAssist(board, &numOfElems, kockice, i, findRowToFill(board, 0, numOfElems), findRowToFill(board, 1, numOfElems), &seed);
            printMatrix(board, numOfElems);
            continue;
        }
        if (column != 3) {
            row = findRowToFill(board, column - 1, numOfElems);
        }
        else {
            printf("Unesi opciju koju zelis: ");
            rucnaMenu(board, numOfElems);
            scanf("%d", &row);
            row--;
        }
        board = insert(board, &numOfElems, row, column - 1, pointsInTurn(kockice, i - 1, row + 1));
        printMatrix(board, numOfElems);
    }

    printf("GOTOVA IGRA! :)\n\n    KONACAN REZULTAT\n\n");
    printMatrix(board, numOfElems);
    exit(0);
}