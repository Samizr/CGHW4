#include "stdafx.h"
#include "LinePlotter.h"


static int LP_Abs(int x) {
	return x > 0 ? x : -x;
}

void plotLineLow(int x0, int y0, int x1, int y1, COLORREF* bitArr, CRect rect, int windowWidth, COLORREF clr) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = y0;
	for (int x = x0; x < x1; x++) {
		if (x > rect.left && y > rect.top && x < rect.right && y < rect.bottom) {
			bitArr[x + windowWidth * y] = clr;
		}
		if (D > 0) {
			y = y + yi;
			D = D - 2 * dx;
		}
		D = D + 2 * dy;

	}
}

void plotLineHigh(int x0, int y0, int x1, int y1, COLORREF* bitArr, CRect rect, int windowWidth, COLORREF clr) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}
	int D = 2 * dx - dy;
	int x = x0;
	for (int y = y0; y < y1; y++) {
			if (x > rect.left && y > rect.top && x < rect.right && y < rect.bottom) {
			bitArr[x + windowWidth * y] = clr;
		}
		if (D > 0) {
			x = x + xi;
			D = D - 2 * dy;
		}
		D = D + 2 * dx;
	}
}

void plotLine(int x0, int y0, int x1, int y1, COLORREF* bitArr, CRect rect, int windowWidth, COLORREF clr) {
	if (LP_Abs(y1 - y0) < LP_Abs(x1 - x0)) {
		if (x0 > x1) {
			plotLineLow(x1, y1, x0, y0, bitArr, rect, windowWidth, clr);
		}
		else {
			plotLineLow(x0, y0, x1, y1, bitArr, rect, windowWidth ,clr);
		}
	}
	else {
		if (y0 > y1) {
			plotLineHigh(x1, y1, x0, y0, bitArr, rect, windowWidth, clr);
		}
		else {
			plotLineHigh(x0, y0, x1, y1, bitArr, rect, windowWidth, clr);
		}
	}
}

void plotLineLowWithDepth(int x0, int y0, int x1, int y1,float* zBuffer, float* depth, COLORREF* bitArr, CRect rect, int windowWidth, COLORREF clr) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = y0;
	for (int x = x0; x < x1; x++) {
		if (x > rect.left && y > rect.top && x < rect.right && y < rect.bottom) {
			if (zBuffer[y * rect.Width() + x] == depth[y * rect.Width() + x]) {
				bitArr[x + windowWidth * y] = clr;
			}
		}
		if (D > 0) {
			y = y + yi;
			D = D - 2 * dx;
		}
		D = D + 2 * dy;

	}
}

void plotLineHighWithDepth(int x0, int y0, int x1, int y1, float* zBuffer, float* depth, COLORREF* bitArr, CRect rect, int windowWidth, COLORREF clr) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}
	int D = 2 * dx - dy;
	int x = x0;
	for (int y = y0; y < y1; y++) {
		if (x > rect.left && y > rect.top && x < rect.right && y < rect.bottom) {
			if (zBuffer[y * rect.Width() + x] == depth[y * rect.Width() + x]) {
				bitArr[x + windowWidth * y] = clr;
			}
			bitArr[x + windowWidth * y] = clr;
		}
		if (D > 0) {
			x = x + xi;
			D = D - 2 * dy;
		}
		D = D + 2 * dx;
	}
}

void plotLineWithDepth(int x0, int y0, int x1, int y1, float* zBuffer, float* depth, COLORREF* bitArr, CRect rect, int windowWidth, COLORREF clr) {
	if (LP_Abs(y1 - y0) < LP_Abs(x1 - x0)) {
		if (x0 > x1) {
			plotLineLowWithDepth(x1, y1, x0, y0, zBuffer, depth, bitArr, rect, windowWidth, clr);
		}
		else {
			plotLineLowWithDepth(x1, y1, x0, y0, zBuffer, depth, bitArr, rect, windowWidth, clr);
		}
	}
	else {
		if (y0 > y1) {
			plotLineHighWithDepth(x1, y1, x0, y0, zBuffer, depth, bitArr, rect, windowWidth, clr);
		}
		else {
			plotLineHighWithDepth(x1, y1, x0, y0, zBuffer, depth, bitArr, rect, windowWidth, clr);
		}
	}
}