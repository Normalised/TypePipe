#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../../EventSystem.h"

struct SimpleEvent
{
    juce::String data {};
};

struct ComplexEvent
{
    int id = 0;
    juce::String name {};
};

class MainComponent : public juce::Component
{
public:
    MainComponent();

private:
    void mouseDown(const juce::MouseEvent &event) override;

    norm::EventSystem eventSystem;
    norm::Subscription<juce::Point<int>> clickHandler;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
