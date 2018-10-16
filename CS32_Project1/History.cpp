#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include "Player.h"
#include "City.h"
#include "Game.h"
#include "Flatulan.h"
#include "History.h"
#include "globals.h"
using namespace std;

History::History(int nRows, int nCols) {
	m_rows = nRows;
	m_cols = nCols;
	markedGrid = new char*[m_rows];
	for (int i = 0; i < m_rows; ++i)
		markedGrid[i] = new char[m_cols];
	for (int r = 0; r < m_rows; r++) {
		for (int c = 0; c < m_cols; c++) {
			markedGrid[r][c] = '.';
		}
	}
}

bool History::record(int r, int c) {
	if (markedGrid[r - 1][c - 1] == '.')
		markedGrid[r - 1][c - 1] = 'A';
	else if (markedGrid[r - 1][c - 1] == 'Z')
		markedGrid[r - 1][c - 1] = 'Z';
	else {
		markedGrid[r - 1][c - 1]++;
	}
	return false;
}

void History::display() const {

	// Draw the grid
	clearScreen();
	for (int r = 0; r < m_rows; r++)
	{
		for (int c = 0; c < m_cols; c++)
			cout << markedGrid[r][c];
		cout << endl;
	}
	cout << endl;
}