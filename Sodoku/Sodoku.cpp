// Sodoku.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>
#include <stack>

using namespace std;

const int domain[9] = { 1,2,3,4,5,6,7,8,9 };

const int N = 9;
const int SUB_N = 3;
const int COORDS = 2;
const int EMPTY = 0;

struct SODOKU_POS {
	int x;
	int y;
};

SODOKU_POS getCandidat(int sodoku[N][N]) {

	int i, j;
	SODOKU_POS sol;

	i = 0;
	while (i < N) {
		j = 0;
		while (j < N) {
			if (sodoku[i][j] == EMPTY) {
				sol.y = i;
				sol.x = j;
				return sol;
			}
			j++;
		}
		i++;
	}

}

bool checkSodokuComplete(int sodoku[N][N]) {

	int i, j;

	i = 0;
	while (i < N) {
		j = 0;
		while (j < N) {
			if (sodoku[i][j] == EMPTY) {
				return false;
			}
			j++;
		}
		i++;
	}

	return true;
}

void printSodoku(int sodoku[N][N]) {

	int i, j;

	cout << "###########################" << endl;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			cout << sodoku[i][j] << " ";
		}
		cout << endl;
	}
	cout << "###########################" << endl;
}

bool checkNumberCol(int sodoku[N][N], int num, int col) {

	int i = 0;

	while(i < N){
		if (sodoku[i][col] == num) {
			return false;
		}

		i++;
	}

	return true;
}

bool checkNumberRow(int sodoku[N][N], int num, int row) {

	int i = 0;

	while (i < N) {
		if (sodoku[row][i] == num) {
			return false;
		}

		i++;
	}

	return true;
}

bool checkSub3(int sodoku[N][N], int num, SODOKU_POS pos) {

	int subCol, subRow, i, j;

	subRow = (pos.y / SUB_N) * SUB_N;
	subCol = (pos.x / SUB_N) * SUB_N;

	i = subRow;
	j = subCol;
	
	while (i < subRow+SUB_N) {
		while (j < subCol+SUB_N) {
			if (sodoku[i][j] == num) {
				return false;
			}
			j++;
		}
		i++;
	}

	return true;
}

bool checkRestrictions(int sodoku[N][N], int num, SODOKU_POS pos) {
	
	if (!checkNumberCol(sodoku, num, pos.x))
		return false;

	if (!checkNumberRow(sodoku, num, pos.y))
		return false;

	if (!checkSub3(sodoku, num, pos))
		return false;

	return true;
}

bool resolve(int sodoku[N][N]) {

	if (checkSodokuComplete(sodoku))
		return true;

	SODOKU_POS pos = getCandidat(sodoku);

	for(int i : domain){
		if (checkRestrictions(sodoku, i, pos)) {
			sodoku[pos.y][pos.x] = i;

			if (resolve(sodoku)) {
				return true;
			}
		}
	}

	sodoku[pos.y][pos.x] = 0;

	return false;
}


int main()
{
	int sodoku[9][9] = {
		{ 0,0,0,2,4,0,0,3,5 },
		{ 0,0,0,0,5,7,0,0,0 },
		{ 2,0,0,6,0,8,0,0,0 },
		{ 0,0,0,0,8,0,0,0,6 },
		{ 0,7,4,0,0,0,3,0,8 },
		{ 0,0,8,7,9,1,4,0,0 },
		{ 0,5,0,0,6,0,0,8,0 },
		{ 0,0,0,4,0,0,6,0,0 },
		{ 0,2,9,8,7,3,0,0,1 },
	};
	//stack<SODOKU_POS> soluciones;

	bool resuelto = resolve(sodoku);
	
	printSodoku(sodoku);
	cin.get();

    return 0;
}

