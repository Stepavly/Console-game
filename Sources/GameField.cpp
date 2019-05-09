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
			field[i][j] = c;
}

//[si; ei) [sj; ej)
void GameField::fill(int si, int sj, int ei, int ej, char c)
{
	for (int i = si; i < ei; i++)
		for (int j = sj; j < ej; j++)
			field[i][j] = c;
}

GameField::GameField() : w(0), h(0), border_width(0)
{
	gen.seed((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());
}

GameField::GameField(int W, int H)
{
	w = W, h = H;
	border_width = 0;
	field = vector<string>(h, string(w, ' '));
	gen.seed((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());
}

GameField::GameField(int W, int H, char c)
{
	w = W, h = H;
	border_width = 0;
	field = vector<string>(h, string(w, c));
	gen.seed((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());
}

void GameField::make_border(int width, char c)
{
	border_width = width;

	for (int i = 0; i < width; i++)
		for (int j = 0; j < w; j++)
			field[i][j] = c;

	for (int i = h - width; i < h; i++)
		for (int j = 0; j < w; j++)
			field[i][j] = c;

	for (int j = 0; j < width; j++)
		for (int i = 0; i < h; i++)
			field[i][j] = c;

	for (int j = w - width; j < w; j++)
		for (int i = 0; i < h; i++)
			field[i][j] = c;
}

void GameField::print()
{
	for (string& line : field)
		cout << line << "\n";
}

//[si; ei) [sj; ej)
void GameField::print(int si, int sj, int ei, int ej)
{
	for (int i = si; i < ej; i++)
	{
		for (int j = sj; j < ej; j++)
			cout << field[i][j];

		cout << "\n";
	}
}

void GameField::generate(int min_locked_prc, char locked, char free)
{
	fill(border_width, border_width, h - border_width, w - border_width, locked);

	dsu cells(w * h);
	uniform_int_distribution<int> column(border_width, h - 1 - border_width);
	uniform_int_distribution<int> row(border_width, w - 1 - border_width);
	vector<int> di({ -1, 1,  0, 0 });
	vector<int> dj({ 0, 0, -1, 1 });
	int can_delete = (100 - min_locked_prc) * (w - border_width) * (h - border_width) / 100;
	int free_cells = (can_delete + 1) / 2;
	can_delete -= free_cells;
	cells.components = free_cells;

	auto ok_place = [&](int vi, int vj)
	{
		return vi >= border_width && vj >= border_width && vi < h - border_width && vj < w - border_width;
	};

	while (free_cells > 0)
	{
		int vi = column(gen);
		int vj = row(gen);
		int can_place = 1;

		for (int k = 0; k < di.size(); k++)
		{
			int toi = vi + di[k];
			int toj = vj + dj[k];

			if (ok_place(toi, toj) && field[toi][toj] == free)
				can_place = 0;
		}

		if (can_place)
		{
			field[vi][vj] = free;
			free_cells--;
		}
	}

	while (cells.components > 1 && can_delete > 0)
	{
		int vi = column(gen);
		int vj = row(gen);

		if (field[vi][vj] == free)
			continue;

		for (int k = 0; k < di.size(); k++)
		{
			int toi = vi + di[k];
			int toj = vj + dj[k];

			if (ok_place(toi, toj) && field[toi][toj] == free && can_delete > 0)
			{
				can_delete--;
				cells.merge_sets(toi * w + toj, vi * w + vj);
				field[vi][vj] = free;
				break;
			}
		}
	}
}

char GameField::get(int i, int j)
{
	return field[i][j];
}

void GameField::set(int i, int j, char c)
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
	border_width = r.border_width;
	field = r.field;
	gen = r.gen;
}

string GameField::operator[](int i) const
{
	return field[i];
}

string& GameField::operator[](int i)
{
	return field[i];
}