#include <queue>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include "GameField.h"
#include "Bot.h"
using namespace std;

Bot::Bot()
{
}

Bot::Bot(GameField& Field, vertex position)
{
	field = &Field;
	pos = position;
}

void Bot::djk(const vector<vector<int>>& cost, vector<vector<int>>& dp, vector<vector<vertex>>& p)
{
	dp = vector<vector<int>>(field->h, vector<int>(field->w, (int)1e9));
	p = vector<vector<vertex>>(field->h, vector<vertex>(field->w, vertex(-1, -1)));
	dp[pos.vi][pos.vj] = 0;

	auto __compare = [&dp](const vertex& a, const vertex& b)
	{
		return dp[a.vi][a.vj] > dp[b.vi][b.vj];
	};

	auto ok_pos = [&](vertex v)
	{
		return
			v.vi >= field->border_width &&
			v.vj >= field->border_width &&
			v.vi < field->h - field->border_width &&
			v.vj < field->w - field->border_width;
	};

	vector<int> di({ -1, 1,  0, 0 });
	vector<int> dj({ 0, 0, -1, 1 });

	priority_queue<vertex, vector<vertex>, decltype(__compare)> pq(__compare);
	pq.push(pos);

	while (!pq.empty())
	{
		vertex v = pq.top();
		pq.pop();

		for (int k = 0; k < di.size(); k++)
		{
			int toi = v.vi + di[k];
			int toj = v.vj + dj[k];

			if (!ok_pos(vertex(toi, toj)) || 
				dp[v.vi][v.vj] + cost[toi][toj] >= dp[toi][toj] || 
				field->get(toi, toj) == '@')
				continue;

			dp[toi][toj] = dp[v.vi][v.vj] + cost[toi][toj];
			p[toi][toj] = v;
			pq.push(vertex(toi, toj));
		}
	}
}

void Bot::find_path()
{
	vector<vector<int>> cost(field->h, vector<int>(field->w, 1));
	vector<vector<vertex>> p;
	vector<vector<int>> dp;

	for (int iter = 0; iter < FIND_PATH_ITER; iter++)
	{
		djk(cost, dp, p);

		vector<vector<char>> best_way_cnt(field->h, vector<char>(field->w, 0));
		int best_cost = (int)1e9;

		for (int i = 0; i < field->h; i++)
			for (int j = 0; j < field->w; j++)
				if (field->get(i, j) == '*')
					best_cost = min(best_cost, dp[i][j]);

		assert(best_cost != (int)1e9);

		queue<vertex> vtx;
		vector<vector<char>> calced(field->h, vector<char>(field->w, 0));

		for (int i = 0; i < field->h; i++)
			for (int j = 0; j < field->w; j++)
				if (field->get(i, j) == '*' && dp[i][j] == best_cost)
				{
					vtx.push(vertex(i, j));
					calced[i][j] = 1;
				}

		auto ok_pos = [&](vertex v)
		{
			return
				v.vi >= field->border_width &&
				v.vj >= field->border_width &&
				v.vi < field->h - field->border_width &&
				v.vj < field->w - field->border_width;
		};

		vector<int> di({ -1, 1, 0, 0 });
		vector<int> dj({ 0, 0, -1, 1 });

		while (!vtx.empty())
		{
			vertex v = vtx.front();
			vtx.pop();

			for (int k = 0; k < 4; k++)
			{
				int toi = v.vi + di[k];
				int toj = v.vj + dj[k];

				if (ok_pos(vertex(toi, toj)) && dp[v.vi][v.vj] == dp[toi][toj] + cost[v.vi][v.vj])
				{
					best_way_cnt[v.vi][v.vj]++;

					if (!calced[toi][toj])
					{
						vtx.push(vertex(toi, toj));
						calced[toi][toj] = 1;
					}
				}
			}
		}
	
		mt19937 gen(chrono::high_resolution_clock::now().time_since_epoch().count());
		int cost_mult = uniform_int_distribution<int>(10, 20)(gen);

		for (int i = 0; i < field->h; i++)
			for (int j = 0; j < field->w; j++)
				cost[i][j] += cost_mult * best_way_cnt[i][j] * best_way_cnt[i][j];
	}
	
	djk(cost, dp, p);
	int min_cost = (int)1e9;
	vertex start;

	for (int i = 0; i < field->h; i++)
		for (int j = 0; j < field->w; j++)
			if (dp[i][j] < min_cost && field->get(i, j) == '*')
			{
				min_cost = dp[i][j];
				start = vertex(i, j);
			}

	path = {};

	for (vertex cur = start; cur != pos; cur = p[cur.vi][cur.vj])
		path.push_back(cur);
}

int Bot::move()
{
	if (path.empty())
	{
		find_path();

		if (path.empty())
			return 0;
		else
			return move();
	}
	else if (field->field[path[0].vi][path[0].vj] != '*')
	{
		path = {};
		find_path();
		return move();
	}

	pos = path.back();
	path.pop_back();
	return 1;
}

void Bot::move(vertex to)
{
	pos = to;
}
