#pragma once
#ifndef GAMEFIELD_H
#define GAMEFIELD_H
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <string>
#include <cstring>
#include <functional>
#include <vector>
#include "Graphics.h"
using namespace std;

struct dsu
{
	vector<int> p;
	vector<int> sz;
	int components;

	dsu();
	dsu(int size);
	int find_set(int v);
	int merge_sets(int a, int b);
};

struct Cell
{
	char text;
	Color back_color;
	Color font_color;

	Cell()
	{
		text = 0;
		back_color = BLACK;
		font_color = WHITE;
	}

	Cell(char symbol)
	{
		text = symbol;
	}

	Cell(char symbol, Color back, Color font)
	{
		text = symbol;
		back_color = back;
		font_color = font;
	}

	void operator=(const Cell& r)
	{
		text = r.text;
		back_color = r.back_color;
		font_color = r.font_color;
	}
};

struct GameField
{
	vector<vector<Cell>> field;
	int w, h;

	void fill(char c);
	//[si; ei) [sj; ej)
	void fill(int i1, int j1, int i2, int j2, char c);
	GameField();
	GameField(int W, int H);
	GameField(int W, int H, char c);
	void operator=(const GameField&);
	Cell get(int i, int j) const;
	void set(int i, int j, Cell c);
	void swap(int i1, int j1, int i2, int j2);
};
#endif
