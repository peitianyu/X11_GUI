#include "core/tt_test.h"
#include "core/tt_log.h"
#include "common/x11_gui.h"

#include <unistd.h>


JUST_RUN_TEST(x11_gui, test)
TEST(x11_gui, test)
{
    X11Gui x11_gui("x11_gui", 500, 500, X11_COLOR(128));

    x11_gui.set_color(X11_RED);
    x11_gui.circle(100, 100, 50);
    x11_gui.flush();
    sleep(2);

    x11_gui.clear();
    x11_gui.set_color(X11_GREEN);
    int polygon[][2] = {
        {100, 100},
        {200, 100},
        {200, 200},
        {100, 200},
        {100, 100},
    };
    x11_gui.polygon(polygon, 5);
    x11_gui.flush();
    sleep(2);

    x11_gui.clear();
    x11_gui.set_color(X11_BLUE);
    x11_gui.line(100, 100, 200, 200);
    x11_gui.flush();
    sleep(2);

    x11_gui.clear();
    x11_gui.set_color(X11_RED);
    x11_gui.pixel(100, 100);
    x11_gui.flush();
    sleep(2);

    x11_gui.clear();
    x11_gui.set_color(X11_WHITE);
    x11_gui.grid(100, 100, 200, 200, 10);
    x11_gui.set_color(X11_RED);
    x11_gui.cell(0, 0, 10, 10);
    x11_gui.flush();
    sleep(2);

    x11_gui.close();
}