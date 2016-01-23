#pragma once

// This hex library was made based on the hexagonal grid reference at http://www.redblobgames.com/grids/hexagons/
// it was then extended a bit in the main.cpp file by me

#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

#define PI 3.14159265359f

struct Point
{
    float x;
    float y;

    Point(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
	Point()
	{

	}
};

struct Hex
{
    int q;
    int r;
    int s;

    Hex(int q, int r, int s)
    {
        this->q = q;
        this->r = r;
        this->s = s;
    }
	Hex(int q, int r)
	{
		this->q = q;
		this->r = r;
		s = -q - r;
	}
	Hex()
	{

	}
};

struct FractionalHex
{
    float q;
    float r;
    float s;

    FractionalHex(float q, float r, float s)
    {
        this->q = q;
        this->r = r;
        this->s = s;
    }
};

struct OffsetCoord
{
	int col;
	int row;
	int EVEN = 1;
	int ODD = -1;

	OffsetCoord(int col, int row)
	{
		this->col = col;
		this->row = row;
	}
};

struct Orientation
{
	float f0;
	float f1;
	float f2;
	float f3;
	float b0;
	float b1;
	float b2;
	float b3;
	float start_angle;

	Orientation(float f0, float f1, float f2, float f3, float b0, float b1, float b2, float b3, float start_angle)
	{
		this->f0 = f0;
		this->f1 = f1;
		this->f2 = f2;
		this->f3 = f3;
		this->b0 = b0;
		this->b1 = b1;
		this->b2 = b2;
		this->b3 = b3;
		this->start_angle = start_angle;
	}
	Orientation()
	{

	}
};

struct Layout
{
	Orientation orientation;
	Point size;
	Point origin;
	Orientation pointy = Orientation(sqrt(3.0), sqrt(3.0) / 2.0f, 0.0f, 3.0f / 2.0f, sqrt(3.0) / 3.0f, -1.0f / 3.0f, 0.0f, 2.0f / 3.0f, 0.5f);
	Orientation flat = Orientation(3.0f / 2.0f, 0.0f, sqrt(3.0) / 2.0f, sqrt(3.0), 2.0f / 3.0f, 0.0f, -1.0f / 3.0f, sqrt(3.0) / 3.0f, 0.0f);
};



// Functions



Hex Add(Hex a, Hex b)
{
	return (Hex(a.q + b.q, a.r + b.r, a.s + b.s));
}

Hex Subtract(Hex a, Hex b)
{
	return (Hex(a.q - b.q, a.r - b.r, a.s - b.s));
}

Hex Scale(Hex a, int k)
{
	return (Hex(a.q * k, a.r * k, a.s * k));
}

std::vector<Hex> hex_directions = {Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1),
Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)};

Hex Direction(int direction)
{
	return (hex_directions[direction]);
}

Hex Neighbor(Hex hex, int direction)
{
	return (Add(hex, hex_directions[direction]));
}

std::vector<Hex> hex_diagonals = {Hex(2, -1, -1), Hex(1, -2, 1), Hex(-1, -1, 2),
Hex(-2, 1, 1), Hex(-1, 2, -1), Hex(1, 1, -2)};

int Length(Hex hex)
{
	return (int)((abs(hex.q) + abs(hex.r) + abs(hex.s)) / 2);
}

int Distance(Hex a, Hex b)
{
	return (Length(Subtract(a, b)));
}

bool Compare(Hex a, Hex b)
{
	if (a.q == b.q && a.r == b.r/* && a.s == b.s*/)
		return true;
	return false;
}

Hex HexRound(FractionalHex h)
{
	int q = (int)(round(h.q));
	int r = (int)(round(h.r));
	int s = (int)(round(h.s));
	float q_diff = abs(q - h.q);
	float r_diff = abs(r - h.r);
	float s_diff = abs(s - h.s);
	if (q_diff > r_diff && q_diff > s_diff)
	{
		q = -r - s;
	}
	else if (r_diff > s_diff)
	{
		r = -q - s;
	}
	else
	{
		s = -q - r;
	}

	return Hex(q, r, s);
}

FractionalHex HexLerp(Hex a, Hex b, float t)
{
	return (FractionalHex(a.q + (b.q - a.q) * t, a.r + (b.r - a.r) * t, a.s + (b.s - a.s) * t));
}

std::vector<Hex> HexLineDraw(Hex a, Hex b)
{
	int N = Distance(a, b);
	std::vector<Hex> Result;
	float step = 1.0f / std::max(N, 1);
	for (int i = 0; i <= N; ++i)
	{
		Result.push_back(HexRound(HexLerp(a, b, step * i)));
	}

	return Result;
}

OffsetCoord QOffsetFromCube(int offset, Hex h)
{
	int col = h.q;
	int row = h.r + (int)((h.q + offset * (h.q & 1)) / 2);

	return OffsetCoord(col, row);
}

Hex QOffsetToCube(int offset, OffsetCoord h)
{
	int q = h.col;
	int r = h.row - (int)((h.col + offset * (h.col & 1)) / 2);
	int s = -q - r;

	return Hex(q, r, s);
}

OffsetCoord ROffsetFromCube(int offset, Hex h)
{
	int col = h.q + (int)((h.r + offset * (h.r & 1)) / 2);
	int row = h.r;

	return OffsetCoord(col, row);
}

Hex ROffsetToCube(int offset, OffsetCoord h)
{
	int q = h.col - (int)((h.row + offset * (h.row & 1)) / 2);
	int r = h.row;
	int s = -q - r;

	return Hex(q, r, s);
}

Point HexToPixel(Layout layout, Hex h)
{
	Orientation M = layout.orientation;
	Point size = layout.size;
	Point origin = layout.origin;
	float x = (M.f0 * h.q + M.f1 * h.r) * (size.x / 2.0f);
	float y = (M.f2 * h.q + M.f3 * h.r) * (size.y / 2.0f);

	return Point(x + origin.x, y + origin.y);
}

FractionalHex PixelToHex(Layout layout, Point p)
{
	Orientation M = layout.orientation;
	Point size = layout.size;
	Point origin = layout.origin;
	Point pt((p.x - origin.x) / size.x, (p.y - origin.y) / size.y);
	float q = M.b0 * pt.x + M.b1 * pt.y;
	float r = M.b2 * pt.x + M.b3 * pt.y;

	return FractionalHex(q, r, -q - r);
}

Point HexCornerOffset(Layout layout, int corner)
{
	Orientation M = layout.orientation;
	Point size = layout.size;
	float angle = 2.0f * PI * (corner + M.start_angle) / 6.0f;

	return Point(size.x * cos(angle), size.y * sin(angle));
}

std::vector<Point> PolygonCorners(Layout layout, Hex h)
{
	std::vector<Point> corners;
	Point center = HexToPixel(layout, h);
	for (int i = 0; i < 6; ++i)
	{
		Point offset = HexCornerOffset(layout, i);
		corners.push_back(Point(center.x + offset.x, center.y + offset.y));
	}

	return corners;
}