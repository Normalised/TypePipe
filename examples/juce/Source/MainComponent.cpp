#include "MainComponent.h"

MainComponent::MainComponent() {
    setSize(800, 600);

    /**
     * This shows how you can control the lifetime of a subscription
     * The mainSubscription is alive for the whole method whilst
     * scopedSubscription only receives messages during the temporary scope.
     * Main Subscription receives all 3 events.
     * Scoped Subscription only receives one event.
     */
    auto mainSubscription = eventSystem.add<SimpleEvent>(
            [](SimpleEvent event) {
                DBG("Main Sub : " + event.data);
            });

    eventSystem.send(SimpleEvent{"Event One"});

    {
        auto scopedSubscription = eventSystem.add<SimpleEvent>(
                [](SimpleEvent event) {
                    DBG("Scoped Sub : " + event.data);
                });

        eventSystem.send(SimpleEvent{"Event Two : during local scope"});

        DBG("End Local Scope");
    }

    eventSystem.send(SimpleEvent{"Event Three : after local scope"});

    /**
     * Event types can be as complex as you like.
     */
    auto complexHandler = eventSystem.add<ComplexEvent>(
            [](auto event) {
                DBG("Complex Event " + juce::String(event.id) + " -> " + event.name);
            });

    eventSystem.send(ComplexEvent{23, "Normalised"});

    /**
     * Its easy to
     */
    clickHandler = eventSystem.add<juce::Point<int>>(
            [](juce::Point<int> position) {
                DBG("Clicked at " + juce::String(position.x) + "," + juce::String(position.y));
            });
}

void MainComponent::mouseDown(const juce::MouseEvent &event) {
    eventSystem.send(event.getMouseDownPosition());
}
