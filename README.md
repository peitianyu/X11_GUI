# 简单可视化工具, 使用x11实现, 在ubuntu20上运行

## 特点
```
JUST_RUN_TEST(x11_window, test)
TEST(x11_window, test)
{
    X11_Window win("window", 500, 500, X11_GRAY);

    win.push_back(std::make_shared<X11_Line>(0, 0, 500, 500, X11_RED));
    win.push_back(std::make_shared<X11_Point>(250, 250, 10, X11_BLUE));
    win.push_back(std::make_shared<X11_Cell>(0, 0, 20, X11_GREEN));
    win.push_back(std::make_shared<X11_Pose>(220, 200, M_PI/2, 5, X11_YELLOW));

    while(true) {}
}

// JUST_RUN_TEST(x11_window, test_2)
TEST(x11_window, test_2)
{
    X11_Window win("window", 500, 500, X11_GRAY);

    // 点云
    for(int i = 0; i < 100; i++)
        win.push_back(std::make_shared<X11_Point>(rand() % 500, rand() % 500, 3, X11_BLUE));
    
    // 栅格, 使用cell
    uint cell_size = 5;
    for(uint i = 0; i < 500; i+=cell_size)
        for(uint j = 0; j < 500; j+=cell_size)
            win.push_back(std::make_shared<X11_Cell>(i, j, 5, X11_COLOR(uint16_t(i/2))));

    while(true) {}
}
```