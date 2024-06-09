#include "core/tt_test.h"
#include "core/tt_log.h"
#include "common/x11_gui.h"

#include <iostream>
#include <cmath> // 用于使用 sqrt 函数计算距离

// JUST_RUN_TEST(x11_gui, test)
TEST(x11_gui, test)
{
    X11Gui x11_gui("x11_gui", 500, 500, X11_COLOR(128));

    // 初始点的位置和尺寸
    int point_x = 250, point_y = 250, point_size = 10;
    // 绘制初始点
    x11_gui.clear();
    x11_gui.set_color(X11_RED);
    x11_gui.point(point_x, point_y, point_size);
    x11_gui.flush();

    auto update_point = [](int& x, int& y, int& size, int mouse_x, int mouse_y, int value = MouseType::MOUSE_UP) {
        constexpr float scale_factor = 0.05;
        int dx = mouse_x - x;
        int dy = mouse_y - y;
        x = (value == MouseType::MOUSE_UP) ? x-dx*scale_factor : x+dx*scale_factor;
        y = (value == MouseType::MOUSE_UP) ? y-dy*scale_factor : y+dy*scale_factor;

        size = (value == MouseType::MOUSE_UP) ? int(size/(1-scale_factor)+0.5) : int(size*(1-scale_factor)+0.5);

        if(value == MouseType::MOUSE_UP)  std::cout << "[Wheel Up]: " << x << " " << y << " " << size << std::endl;
        else                              std::cout << "[Wheel Up]: " << x << " " << y << " " << size << std::endl;
    };

    while(true) {
        Interaction ia = x11_gui.get_event();
        if(ia.type == MOUSE) {
            update_point(point_x, point_y, point_size, ia.coord[0], ia.coord[1], ia.value);
            // 重绘点
            x11_gui.clear();
            x11_gui.set_color(X11_RED);
            x11_gui.point(point_x, point_y, point_size);
            x11_gui.flush();
        }
    }

    x11_gui.close();
}