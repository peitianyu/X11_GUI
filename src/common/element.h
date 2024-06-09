#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "x11_gui.h"
#include <cmath>

struct X11_Element
{
    float x, y;
    XColor color;

    X11_Element(float x, float y, XColor color) : x(x), y(y), color(color) {}

    virtual void update(int mouse_x, int mouse_y, MouseType mouse_type)
    {
        constexpr float scale_factor = 1.1; 
        float relative_x = x - mouse_x;     
        float relative_y = y - mouse_y;      

        float zoom = (mouse_type == MouseType::MOUSE_UP) ? 1 / scale_factor : scale_factor;

        x = mouse_x + relative_x * zoom;
        y = mouse_y + relative_y * zoom;
    }
    virtual void move(int dx, int dy) { x += dx; y += dy; }
    virtual void draw(X11Gui &gui, const float& scale_factor) { gui.set_color(color); gui.pixel(x, y); }
};

struct X11_Point : public X11_Element {    
    int size;
    X11_Point(float x, float y, int size, XColor color) : X11_Element(x, y, color), size(size) {}
    void draw(X11Gui &gui, const float& scale_factor) override { gui.set_color(color); gui.point(x, y, size*scale_factor+0.5); }
};

struct X11_Pose : public X11_Element {
    int size;
    float theta;
    X11_Pose(float x, float y, float theta, int size, XColor color) : X11_Element(x, y, color), size(size), theta(theta) {}
    void draw(X11Gui &gui, const float& scale_factor) override {
        gui.set_color(color);
        float draw_size = size*scale_factor;
        gui.line(x, y, x + 2*draw_size*cos(theta), y+2*draw_size*sin(theta));
        gui.point(x, y, draw_size+0.5);
    }
};

struct X11_Line : public X11_Element {
    float x2, y2;
    X11_Line(float x, float y, float x2, float y2, XColor color) : X11_Element(x, y, color), x2(x2), y2(y2) {}

    virtual void move(int dx, int dy) override { x += dx; y += dy; x2 += dx; y2 += dy; }
    void update(int mouse_x, int mouse_y, MouseType mouse_type) override 
    {
        constexpr float scale_factor = 1.1; 
        float relative_x = x - mouse_x;      
        float relative_y = y - mouse_y;    
        float relative_x2 = x2 - mouse_x;   
        float relative_y2 = y2 - mouse_y;   

        float zoom = (mouse_type == MouseType::MOUSE_UP) ? 1 / scale_factor : scale_factor;

        x = mouse_x + relative_x * zoom;
        y = mouse_y + relative_y * zoom;
        x2 = mouse_x + relative_x2 * zoom;
        y2 = mouse_y + relative_y2 * zoom;
    }
    void draw(X11Gui &gui, const float& scale_factor) override { gui.set_color(color); gui.line(x, y, x2, y2); }
};

struct X11_Cell : public X11_Element {
    int size;
    X11_Cell(float x, float y, int size, XColor color) : X11_Element(x, y, color), size(size) {}
    void draw(X11Gui &gui, const float& scale_factor) override { gui.set_color(color); gui.cell(x, y, size*scale_factor+0.5); }
};

#endif // __ELEMENT_H__