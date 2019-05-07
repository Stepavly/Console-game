#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

struct Point
{
	unsigned x, y;

	Point() : x(0), y(0) {}

	Point(unsigned X, unsigned Y)
	{
		x = X;
		y = Y;
	}
};

enum Color
{
	BLACK = 0x00,
	BLUE = 0x01,
	GREEN = 0x02,
	CYAN = 0x03,
	RED = 0x04,
	MAGENTA = 0x05,
	BROWN = 0x06,
	LGRAY = 0x07,
	DGRAY = 0x08,
	LBLUE = 0x09,
	LGREEN = 0x0A,
	LCYAN = 0x0B,
	LRED = 0x0C,
	LMAGENTA = 0x0D,
	YELLOW = 0x0E,
	WHITE = 0x0F,
};

struct ConsoleGUI
{
	Color current_bg_color = BLACK;
	Color current_text_color = WHITE;

	ConsoleGUI();
	HANDLE get_console_handle();
	void set_pos(Point p);
	void set_text_color(Color cl);
	void set_bg_color(Color cl);
	void clear(Point p, Color bg_color);
	void clear(Point start, Point end, Color bg_color);
	void clear_all();
	void print(char c);
	void print(int n);
	void print(string text);
	void print(char c, Color font, Color back);
	void print(int n, Color font, Color back);
	void print(string text, Color font, Color back);
};
#endif
