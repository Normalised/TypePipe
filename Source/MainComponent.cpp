#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(800, 600);

    auto mainHandler = eventSystem.add<SimpleEvent>([](SimpleEvent event)
    {
        DBG("Main Handler : " + event.data);
    });

    eventSystem.send(SimpleEvent{"Event One"});
    {
        auto scopedHandler = eventSystem.add<SimpleEvent>([](SimpleEvent event)
        {
            DBG("Scoped handler : " + event.data);
        });
        eventSystem.send(SimpleEvent{"Event Two : during local scope"});

        DBG("End Local Scope");
    }

    eventSystem.send(SimpleEvent{"Event Three : after local scope"});

    auto complexHandler = eventSystem.add<ComplexEvent>(
        [](auto event)
        {
            DBG("Complex Event " + juce::String(event.id) + " -> "
                + event.name);
        });

    eventSystem.send(ComplexEvent{23, "Normalised"});
	
	keyboard = KeyboardFactory::instance(this);

	keyboard->onKeyDownFn = [&](int keyCode){
		eventSystem.send(KeyEvent{keyCode, true});
	};
	
	keyboard->onKeyUpFn = [&](int keyCode){
		eventSystem.send(KeyEvent{keyCode, false});
	};
	
	keyHandler = eventSystem.add<KeyEvent>([](KeyEvent event) {
		if(event.isDown)
		{
			DBG("Key Down " + juce::String(event.keyCode));
		} else {
			DBG("Key Up " + juce::String(event.keyCode));
		}
	});
}
