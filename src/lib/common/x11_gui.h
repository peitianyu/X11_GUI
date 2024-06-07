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

class X11Gui
{
public:
    X11Gui(const std::string& name, const uint& width, const uint& height, XColor c = X11_BLACK) 
    {
        display_ = XOpenDisplay(0);
        if(!display_) {std::cout << "Failed to open display" << std::endl; return;}

        // Detect if display can do fast color mode.
        Visual *visual = DefaultVisual(display_, 0);
        fast_mode_ = (visual && visual -> c_class == TrueColor);

        // Create a simple X graphical window
        window_ = XCreateSimpleWindow(display_, DefaultRootWindow(display_), 0, 0, width, height, 0, 0, 0);

        // Set backing store
        XSetWindowAttributes attr;
        attr.backing_store = Always;
        
        // Change the title and other window attributes
        XChangeWindowAttributes(display_, window_, CWBackingStore, &attr);
        XStoreName(display_, window_, name.c_str());

        set_background_color(c);

        // Setup input
        XSelectInput(display_, window_, StructureNotifyMask|KeyPressMask|ButtonPressMask);

        // Map the window
        XMapWindow(display_, window_);

        // Create graphics context
        canvas_ = XCreateGC(display_, window_, 0, NULL);

        // Setup color
        colors_ = DefaultColormap(display_, 0);
        XSetForeground(display_, canvas_, WhitePixel(display_, DefaultScreen(display_)));

        // Wait for the MapNotify event
        while (1) {
            XEvent e;
            XNextEvent(display_, &e);
            if (e.type == MapNotify) break; 
        }
    }

    void set_background_color(XColor c) {
        XColor color = c;
        XSetWindowAttributes attr;
        if(fast_mode_) {
            attr.background_pixel = ((color.blue & 0xff) | ((color.green & 0xff) << 8) | (color.red & 0xff) << 16);
        } else {
            XAllocColor(display_, colors_, &color);
            attr.background_pixel = color.pixel;
        }
        XChangeWindowAttributes(display_, window_, CWBackPixel, &attr);
    }

    void set_color(XColor c) {
        XColor color = c;
        if(fast_mode_)  color.pixel = ((color.blue & 0xff) | ((color.green & 0xff) << 8) | (color.red & 0xff) << 16);
        else            XAllocColor(display_, colors_, &color);

        XSetForeground(display_, canvas_, color.pixel);
    }

    void pixel(int x, int y) {
        XDrawPoint(display_, window_, canvas_, x, y);
    }

    void line(int x1, int y1, int x2, int y2) {
        XDrawLine(display_, window_, canvas_, x1, y1, x2, y2);
    }

    void polygon(int points[][2], int n) {
        XPoint* xpoints = new XPoint[n];
        for (int i = 0; i < n; i++) {
            xpoints[i].x = points[i][0];
            xpoints[i].y = points[i][1];
        }
        XDrawLines(display_, window_, canvas_, xpoints, n, CoordModeOrigin);
        delete[] xpoints;
    }

    void circle(int x, int y, int r) {
        XDrawArc(display_, window_, canvas_, x - r, y - r, 2 * r, 2 * r, 0, 360 * 64);
    }

    void grid(int x, int y, int w, int h, int r) {
        for (int i = x; i <= w; i += r) 
            XDrawLine(display_, window_, canvas_, i, y, i, h);
        for (int i = y; i <= h; i += r) 
            XDrawLine(display_, window_, canvas_, x, i, w, i);
    }

    void cell(int x, int y, int w, int h) {
        XFillRectangle(display_, window_, canvas_, x, y, w, h);
    }

    void flush() {
        XFlush(display_);
    }

    void clear() {
        XClearWindow(display_, window_);
    }

    void close() {
        XFreeGC(display_, canvas_);
        XCloseDisplay(display_);
    }
private:
    Display* display_;
    bool fast_mode_;
    Window window_;
    GC canvas_;
    Colormap colors_;
};

#endif // X11_GUI_H__