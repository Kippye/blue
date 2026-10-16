#pragma once

#include "glad_glfw.h"
#include "program_info.h"

#include <functional>

#include "event.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

enum CURSOR_TYPE {
    NORMAL,
    DRAW
};

class Window {
    private:
        // window events
        std::function<void(Window*, int, int)> framebuffer_size_callback;
        std::function<void(Window*, int)> window_focus_callback;
        std::function<void(Window*)> window_close_callback;
        // input events
        std::function<void(Window*, int, int, int, int)> key_callback;
        std::function<void(Window*, int, int, int)> mouse_button_callback;
        std::function<void(Window*, double, double)> scroll_callback;
        std::function<void(Window*, double, double)> cursor_pos_callback;

    public:
        GLFWwindow* window;
        std::string_view titleBase = program_info::PROGRAM_NAME;
        std::string title = "";
        CURSOR_TYPE cursor = NORMAL;
        GLFWcursor* cursors[2] = {};
        int SCREEN_WIDTH = 800;
        int SCREEN_HEIGHT = 600;
        bool firstMouseMovement = true;
        bool hasFocus = true;
        bool shouldClose = false;
        // window functions
        void setCursor(CURSOR_TYPE _cursor);
        void setTitle(const char* titleFile, bool dirty);
        // lifecycle functions
        void initialize();
        void terminate();

        Event<int, int, int, int> keyEvent;
        Event<int, int, int> mouseButtonEvent;
        Event<double, double> scrollEvent;
        Event<double, double> cursorPosEvent;
};
