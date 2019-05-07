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

struct GameField
{
	vector<string> field;
	mt19937 gen;
	int w, h;
	int border_width;

	void fill(char);
	//[si; ei) [sj; ej)
	void fill(int, int, int, int, char);
	GameField();
	GameField(int, int);
	GameField(int, int, char);
	void make_border(int, char);
	void print();
	//[si; ei) [sj; ej)
	void print(int, int, int, int);
	void generate(int, char, char);
	GameField operator=(const GameField&);
	char get(int, int);
	void set(int, int, char);
};
#endif
