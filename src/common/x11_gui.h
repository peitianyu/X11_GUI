#ifndef __X11_GUI_H__
#define __X11_GUI_H__

#include "X11/Xlib.h"
#include <memory>
#include <iostream>

#define X11_RED (XColor){.red = 255, .green = 0, .blue = 0}
#define X11_GREEN (XColor){.red = 0, .green = 255, .blue = 0}
#define X11_BLUE (XColor){.red = 0, .green = 0, .blue = 255}
#define X11_WHITE (XColor){.red = 255, .green = 255, .blue = 255}
#define X11_BLACK (XColor){.red= 0, .green = 0, .blue = 0}
#define X11_GRAY (XColor){.red = 128, .green = 128, .blue = 128}
#define X11_YELLOW (XColor){.red = 255, .green = 255, .blue = 0}
#define X11_COLOR(x) (XColor){.red = x, .green = x, .blue = x}

enum InteractionType {
	MOUSE, KEY
};

enum MouseType {
    MOUSE_UP, MOUSE_DOWN
};

struct Interaction {
	int type;
	uint value;
	int coord[2];
};

class X11Gui
{
public:
    X11Gui(const std::string& name, const uint& width, const uint& height, XColor c = X11_BLACK);

    void set_color(XColor c);

    void pixel(int x, int y);

    void point(int x, int y, int size);

    void line(int x1, int y1, int x2, int y2);

    void polygon(int points[][2], int n);

    void circle(int x, int y, int r);

    void grid(int x, int y, int w, int h, int r);

    void cell(int x, int y, int size);

    void flush();

    void clear();

    void close();

    Interaction get_event();

    XEvent event();
private:
    void set_background_color(XColor c);
private:
    Display* display_;
    bool fast_mode_;
    Window window_;
    GC canvas_;
    Colormap colors_;
};

#endif // X11_GUI_H__