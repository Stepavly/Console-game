#include "GameField.h"
using namespace std;

dsu::dsu() {}

dsu::dsu(int size)
{
	p = vector<int>(size);
	sz = vector<int>(size, 1);
	components = size;

	for (int i = 0; i < size; i++)
		p[i] = i;
}

int dsu::find_set(int v)
{
	if (p[v] == v)
		return v;

	return p[v] = find_set(p[v]);
}

int dsu::merge_sets(int a, int b)
{
	a = find_set(a);
	b = find_set(b);

	if (a == b)
		return 0;

	if (sz[a] < sz[b])
		swap(a, b);

	sz[a] += sz[b];
	p[b] = a;
	components--;
	return 1;
}

void GameField::fill(char c)
{
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			field[i][j].text = c;
}

//[si; ei) [sj; ej)
void GameField::fill(int si, int sj, int ei, int ej, char c)
{
	for (int i = si; i < ei; i++)
		for (int j = sj; j < ej; j++)
			field[i][j].text = c;
}

GameField::GameField() : w(0), h(0)
{
}

GameField::GameField(int W, int H)
{
	w = W, h = H;
	field = vector<vector<Cell>>(h, vector<Cell>(w, ' '));
}

GameField::GameField(int W, int H, char c)
{
	w = W, h = H;
	field = vector<vector<Cell>>(h, vector<Cell>(w, c));
}

Cell GameField::get(int i, int j) const
{
	return field[i][j];
}

void GameField::set(int i, int j, Cell c)
{
	field[i][j] = c;
}

void GameField::swap(int i1, int j1, int i2, int j2)
{
	std::swap(field[i1][j1], field[i2][j2]);
}

void GameField::operator=(const GameField& r)
{
	h = r.h;
	w = r.w;
	field = r.field;
}