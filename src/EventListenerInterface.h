#pragma once

class EventListenerInterface {
public:
    virtual ~EventListenerInterface() = default;

    // Callback for key press event
    virtual void onKeyEvent(int key, int scancode, int action, int mods) {}

    // Callback for mouse button event
    virtual void onMouseEvent(int button, int action, int mods) {}

    // Callback for window resize event
    virtual void onWindowResize(int width, int height) {}
};
