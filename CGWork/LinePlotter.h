#pragma once


void plotLineLow(int x0, int y0, int x1, int y1, COLORREF* bitArr/*CDC* pdc*/, CRect rect, COLORREF clr);
void plotLineHigh(int x0, int y0, int x1, int y1, COLORREF* bitArr/*CDC* pdc*/, CRect rect, COLORREF clr);
void plotLine(int x0, int y0, int x1, int y1, COLORREF* bitArr/*CDC* pdc*/, CRect rect, COLORREF clr);