#pragma once
#ifndef BOT_H
#define BOT_H
#include <queue>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include "GameField.h"
using namespace std;

const int FIND_PATH_ITER = 3;

struct Bot
{
	struct vertex
	{
		int vi, vj;

		vertex() : vi(0), vj(0) {}

		vertex(int i, int j)
		{
			vi = i;
			vj = j;
		}

		bool operator!=(const vertex& r)
		{
			return vi != r.vi || vj != r.vj;
		}
	};

	GameField* field;
	vector<vertex> path;
	vertex pos;

	Bot();
	Bot(GameField& Field, vertex position);
	void djk(const vector<vector<int>>& cost, vector<vector<int>>& dp, vector<vector<vertex>>& p);
	void find_path();
	int move();
	void move(vertex to);
};
#endif