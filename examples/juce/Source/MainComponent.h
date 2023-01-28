#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../../TypePipe.h"

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

    norm::TypePipe pipe;
    norm::Subscription<juce::Point<int>> clickHandler;

    juce::Label label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
