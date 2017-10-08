// Sodoku.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>

// =============================================================================
// ALUMNOS =====================================================================
// =============================================================================

char* NombreAlumno1 = "nombre del alumno 1";
char* ApellidosAlumno1 = "apellidos del alumno 1";
char* NIAAlumno1 = "1423234"; // NIA alumno1

								// No rellenar en caso de grupo de un alumno
char* NombreAlumno2 = "nombre del alumno 2";
char* ApellidosAlumno2 = "apellidos del alumno 2";
char* NIAAlumno2 = ""; // NIA alumno2

char* NIAS[] = {
	"1335167","1428136","1397750","1390855","1424739","1424670","1423739",
	"1423177","1427371","1423725","1448752","1423210","1391968","1424454",
	"1338429","1425988","1424818","1421980","1368533","1365501","1423802",
	"1175542","1264719","1424801","1423710","1390452","1424116","1391795",
	"1395085","1391630","1424310","1423234","1425968","1307828","1395062",
	"1426532","1391627","1366098","1425082","1325835","1425067","1424350",
	"1424035","1391544","1391634","1426771","1424698","1358835","1362219",
	"1425785","1424114","1424408","1362389","1428260","1391808","1425084",
	"1366706","1391632","1425109","1424032","1430970","1430896","1428176",
	"1390536","1429086","1423588","1195650","1424671","1306347"
};

bool CheckNIA(char* nia)
{
	for (const char *pNIA : NIAS) {
		if (strcmp(nia,pNIA) == 0) return true;
	}
	return false;
}

// =============================================================================
// PROBLEMA ====================================================================
// =============================================================================

// Clock =======================================================================

double Clock()
{
	LARGE_INTEGER cnt;
	LARGE_INTEGER fre;

	if (QueryPerformanceFrequency(&fre) && QueryPerformanceCounter(&cnt)) {
		return (double)cnt.QuadPart / (double)fre.QuadPart;
	}
	else return (double)clock() / (double)CLOCKS_PER_SEC;
}

typedef int Sodoku[9][9];

// LeerSodoku ==================================================================

void LeerSodoku(Sodoku &s, const char *filename)
{
	FILE *fp;
	fopen_s(&fp,filename, "r");
	if (fp == NULL) {
		printf("No se ha podido abrir el fichero %s", filename);
		exit(1);
	}
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			fscanf_s(fp,"%d", &s[i][j]);
		}
	}
	fclose(fp);
}

// Print =======================================================================

void Print(Sodoku &s) {
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			printf("%d ", s[i][j]);
		}
		puts("");
	}
}

// Functions
const int domain[9] = { 1,2,3,4,5,6,7,8,9 };

const int N = 9;
const int SUB_N = 3;
const int COORDS = 2;
const int EMPTY = 0;


//Global Structures

struct SodokuPos {
	int x;
	int y;
};

Sodoku colNumbers;
Sodoku rowNumbers;
Sodoku subNumbers;
Sodoku defNumbers;
SodokuPos lastPos;

SodokuPos getCandidat(Sodoku sodoku) {

	int i, j;
	SodokuPos sol;

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

bool checkSodokuComplete(Sodoku sodoku, SodokuPos pos) {
	return (pos.x == lastPos.x && pos.y == lastPos.y);
}

bool checkNumberCol(Sodoku sodoku, int num, int col) {
	return !colNumbers[col][num-1];
}

bool checkNumberRow(Sodoku sodoku, int num, int row) {
	return !rowNumbers[row][num-1];
}

bool checkSub3(Sodoku sodoku, int num, SodokuPos pos) {
	return !subNumbers[(pos.y - (pos.y % 3)) + ((pos.x - (pos.x % 3)) / 3)][num - 1];
}

bool checkRestrictions(Sodoku sodoku, int num, SodokuPos pos) {

	if (!checkNumberCol(sodoku, num, pos.x))
		return false;

	if (!checkNumberRow(sodoku, num, pos.y))
		return false;

	if (!checkSub3(sodoku, num, pos))
		return false;

	return true;
}

bool resolve(Sodoku sodoku, SodokuPos lastPos) {

	if (checkSodokuComplete(sodoku, lastPos))
		return true;

	SodokuPos pos = getCandidat(sodoku);

	for (int i : domain) {
		if (checkRestrictions(sodoku, i, pos)) {
			sodoku[pos.y][pos.x] = i;
			rowNumbers[pos.y][i-1] = 1;
			colNumbers[pos.x][i-1] = 1;
			subNumbers[(pos.y - (pos.y % 3)) + ((pos.x - (pos.x % 3)) / 3)][i - 1] = 1;

			if (resolve(sodoku, pos)) {
				return true;
			}

			sodoku[pos.y][pos.x] = 0;
			rowNumbers[pos.y][i-1] = 0;
			colNumbers[pos.x][i-1] = 0;
			subNumbers[(pos.y - (pos.y % 3)) + ((pos.x - (pos.x % 3)) / 3)][i - 1] = 0;
		}
	}

	sodoku[pos.y][pos.x] = 0;

	return false;
}

void loadBooleanStructures(Sodoku sodoku) {

	int i, j;
	int subRow, subCol;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (sodoku[i][j] != 0) {
				//Init Row
				rowNumbers[i][sodoku[i][j]-1] = true;
				//Init Col
				colNumbers[j][sodoku[i][j]-1] = true;
				//Init sub3x3 
				subRow = i - (i % 3);
				subCol = j - (j % 3);
				subNumbers[subRow + (subCol / 3)][sodoku[i][j] - 1] = true;
			}
			else {
				lastPos.x = j;
				lastPos.y = i;
			}
		}
	}

}

void initMatrix(Sodoku matrix) {
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			matrix[i][j] = 0;
		}
	}
}

// main ========================================================================

Sodoku taulell; // Variable per guadar el taulell del Sodoku

int main(int argc, char *argv[])
{
	if (!CheckNIA(NIAAlumno1) || !(*NIAAlumno2 == '\0' || CheckNIA(NIAAlumno2))) {
		puts("Indentificacio dels alumnes incorrecte");
		return 1;
	}
	printf("NIA1: %s\n",NIAAlumno1 );
	printf("NOM1: %s\n", NombreAlumno1);
	printf("COGNOMS1: %s\n", ApellidosAlumno1);
	printf("NIA2: %s\n", NIAAlumno2);
	printf("NOM2: %s\n", NombreAlumno2);
	printf("COGNOMS2: %s\n", ApellidosAlumno2);
	if (argc != 2) {
		puts("Uso: sodoku FicheroDeSodoku\n");
		return 1;
	}
	LeerSodoku(taulell, argv[1]);
	puts("\nSODOKU:");
	Print(taulell);
	bool HiHaSolucio=false;
	double t0 = Clock();

	SodokuPos pos;
	pos.x = 0;
	pos.y = 0;

	initMatrix(colNumbers);
	initMatrix(rowNumbers);
	initMatrix(subNumbers);
	loadBooleanStructures(taulell);
	HiHaSolucio = resolve(taulell, pos);

	double t1 = Clock();
	if (HiHaSolucio) {
		puts("\nSOLUCIO:");
		Print(taulell);
	}
	else {
		puts("\nNO TE SOLUCIO");
	}
	printf("TEMPS: %lf\n", t1 - t0);
    return 0;
}

