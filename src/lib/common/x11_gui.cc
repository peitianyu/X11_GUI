#include "x11_gui.h"


X11Gui::X11Gui(const std::string& name, const uint& width, const uint& height, XColor c) 
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

void X11Gui::set_background_color(XColor c) {
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

void X11Gui::set_color(XColor c) {
    XColor color = c;
    if(fast_mode_)  color.pixel = ((color.blue & 0xff) | ((color.green & 0xff) << 8) | (color.red & 0xff) << 16);
    else            XAllocColor(display_, colors_, &color);

    XSetForeground(display_, canvas_, color.pixel);
}

void X11Gui::pixel(int x, int y) {
    XDrawPoint(display_, window_, canvas_, x, y);
}

void X11Gui::line(int x1, int y1, int x2, int y2) {
    XDrawLine(display_, window_, canvas_, x1, y1, x2, y2);
}

void X11Gui::polygon(int points[][2], int n) {
    XPoint* xpoints = new XPoint[n];
    for (int i = 0; i < n; i++) {
        xpoints[i].x = points[i][0];
        xpoints[i].y = points[i][1];
    }
    XDrawLines(display_, window_, canvas_, xpoints, n, CoordModeOrigin);
    delete[] xpoints;
}

void X11Gui::circle(int x, int y, int r) {
    XDrawArc(display_, window_, canvas_, x - r, y - r, 2 * r, 2 * r, 0, 360 * 64);
}

void X11Gui::grid(int x, int y, int w, int h, int r) {
    for (int i = x; i <= w; i += r) 
        XDrawLine(display_, window_, canvas_, i, y, i, h);
    for (int i = y; i <= h; i += r) 
        XDrawLine(display_, window_, canvas_, x, i, w, i);
}

void X11Gui::cell(int x, int y, int w, int h) {
    XFillRectangle(display_, window_, canvas_, x, y, w, h);
}

void X11Gui::flush() {
    XFlush(display_);
}

void X11Gui::clear() {
    XClearWindow(display_, window_);
}

void X11Gui::close() {
    XFreeGC(display_, canvas_);
    XCloseDisplay(display_);
}
