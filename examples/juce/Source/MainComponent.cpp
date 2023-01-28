#include "MainComponent.h"

MainComponent::MainComponent() {
    setSize(800, 600);
    addAndMakeVisible(label);
    label.setBounds(10,10,300,100);

    /**
     * This shows how you can control the lifetime of a subscription
     * The mainSubscription is alive for the whole method whilst
     * scopedSubscription only receives messages during the temporary scope.
     * Main Subscription receives all 3 events.
     * Scoped Subscription only receives one event.
     */
    auto mainSubscription = pipe.add<SimpleEvent>(
            [this](SimpleEvent event) {
                DBG("Main Sub : " + event.data);
                label.setText(event.data, juce::dontSendNotification);
            });

    pipe.send(SimpleEvent{"Event One"});

    {
        auto scopedSubscription = pipe.add<SimpleEvent>(
                [](SimpleEvent event) {
                    DBG("Scoped Sub : " + event.data);
                });

        pipe.send(SimpleEvent{"Event Two : during local scope"});

        DBG("End Local Scope");
    }

    pipe.send(SimpleEvent{"Event Three : after local scope"});

    /**
     * Event types can be as complex as you like.
     */
    auto complexHandler = pipe.add<ComplexEvent>(
            [](auto event) {
                DBG("Complex Event " + juce::String(event.id) + " -> " + event.name);
            });

    pipe.send(ComplexEvent{23, "Normalised"});

    /**
     * Its easy to
     */
    clickHandler = pipe.add<juce::Point<int>>(
            [](juce::Point<int> position) {
                DBG("Clicked at " + juce::String(position.x) + "," + juce::String(position.y));
            });
}

void MainComponent::mouseDown(const juce::MouseEvent &event) {
    pipe.send(event.getMouseDownPosition());
}
