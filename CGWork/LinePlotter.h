#pragma once


void plotLineLow(int x0, int y0, int x1, int y1, COLORREF* bitArr, CRect rect, int windowWidth, COLORREF clr);
void plotLineHigh(int x0, int y0, int x1, int y1, COLORREF* bitArr, CRect rect, int windowWidth, COLORREF clr);
void plotLine(int x0, int y0, int x1, int y1, COLORREF* bitArr, CRect rect, int widowWidth, COLORREF clr);
void plotLineLowWithDepth(int x0, int y0, int x1, int y1, float* zBuffer, float* depth, COLORREF* bitArr, CRect rect, int windowWidth, COLORREF clr);
void plotLineHighWithDepth(int x0, int y0, int x1, int y1, float* zBuffer, float* depth, COLORREF* bitArr, CRect rect, int windowWidth, COLORREF clr);
void plotLineWithDepth(int x0, int y0, int x1, int y1, float* zBuffer, float* depth, COLORREF* bitArr, CRect rect, int windowWidth, COLORREF clr);