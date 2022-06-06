#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "EventSystem.h"
#include <raw_keyboard_input/raw_keyboard_input.h>

struct SimpleEvent
{
    juce::String data {};
};

struct ComplexEvent
{
    int id = 0;
    juce::String name {};
};

struct KeyEvent
{
    int keyCode = 0;
    bool isDown = false;
};

class MainComponent : public juce::Component
{
public:
    MainComponent();

private:
    tap::EventSystem eventSystem;

    std::unique_ptr<tap::Handler<KeyEvent>> keyHandler;

    Keyboard* keyboard;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
