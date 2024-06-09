#ifndef __X11_WINDOW_H__
#define __X11_WINDOW_H__

#include "x11_gui.h"
#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include "element.h"

class X11_Window {
public:
    X11_Window(const std::string& name, const uint& width, const uint& height, XColor c = X11_BLACK) : should_quit_(false), button_down_(false), scale_factor_(1.0) {
        gui_ = std::make_unique<X11Gui>(name, width, height, c);

        // NOTE: 用于获取变化尺度
        elements_.push_back(std::make_shared<X11_Line>(0, 0, 10000, 0, c));

        interval_ms_ = 5;
        event_thread_ = std::thread([this]() {
            std::unique_lock<std::mutex> lock(mutex_);
            auto next_wake_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval_ms_);
            while (true) {
                cv_.wait_until(lock, next_wake_time, [this]{ return should_quit_; });
                if (should_quit_) { gui_->close(); break; }
                thread_tick();
                next_wake_time += std::chrono::milliseconds(interval_ms_);
            }
        });
    }

    ~X11_Window() {
        should_quit_ = true;
        event_thread_.join();
    }
    
    void push_back(std::shared_ptr<X11_Element> element) {
        std::unique_lock<std::mutex> lock(mutex_);
        elements_.push_back(element);
    }

    void clear() {
        std::unique_lock<std::mutex> lock(mutex_);
        elements_.erase(elements_.begin()+1, elements_.end());
    }
private:
    void thread_tick() {
        XEvent e = gui_->event();
        if(e.type == ButtonPress) process_button_press(e);
        else if(e.type == KeyPress) process_key_press(e);
        else if(e.type == ButtonRelease) button_down_ = false;
        else if(e.type == MotionNotify) process_motion_notify(e);

        gui_->clear();
        for(auto& element : elements_) element->draw(*gui_, scale_factor_);
    }

    void process_button_press(XEvent& e)
    {
        if(e.xbutton.button == 2) {
            // NOTE: 滚轮按下
            button_down_ = true;
            button_x_ = e.xkey.x;
            button_y_ = e.xkey.y;
            // std::cout << "e.xbutton.button: " << e.xbutton.button << " " << e.xkey.x << " " << e.xkey.y << std::endl;
        } else if(e.xbutton.button == 4) {
            // NOTE: 滚轮上滑
            for(auto& element : elements_) element->update(e.xbutton.x, e.xbutton.y, MOUSE_DOWN); 
            // std::cout << "e.xbutton.button: " << e.xbutton.button << " " << e.xkey.x << " " << e.xkey.y << std::endl;
            update_scale_factor();
        } else if(e.xbutton.button == 5) {
            // NOTE: 滚轮下滑
            for(auto& element : elements_) element->update(e.xbutton.x, e.xbutton.y, MOUSE_UP); 
            // std::cout << "e.xbutton.button: " << e.xbutton.button << " " << e.xkey.x << " " << e.xkey.y << std::endl;
            update_scale_factor();
        } 
    }

    void process_key_press(XEvent& e)
    {
        int key_value = XLookupKeysym(&e.xkey, 0);
        if(key_value == 'q') should_quit_ = true;
        // std::cout << "KeyPress" << " " << (char)key_value << std::endl;
    }

    void process_motion_notify(XEvent& e) {
        if(button_down_) {
            for(auto& element : elements_) element->move(e.xmotion.x - button_x_, e.xmotion.y - button_y_);
            // std::cout << "MotionNotify" << " " << e.xmotion.x << " " << e.xmotion.y << std::endl;
            button_x_ = e.xmotion.x;
            button_y_ = e.xmotion.y;
        }
    }

    void update_scale_factor()
    {
        X11_Line* line = (X11_Line*)elements_[0].get();
        int dx = line->x2 - line->x;
        scale_factor_ = (float)dx / (float)10000;
        // std::cout << "scale_factor_ " << scale_factor_ << std::endl;
    }
private:
    bool should_quit_;
    int interval_ms_;
    std::condition_variable cv_;
    std::thread event_thread_;
    std::mutex mutex_;

    bool button_down_;
    int button_x_;
    int button_y_;
    float scale_factor_;
    std::unique_ptr<X11Gui> gui_;
    std::vector<std::shared_ptr<X11_Element>> elements_;
};


#endif // __X11_WINDOW_H__