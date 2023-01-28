# Simple Type Pipe

This is an event / listener system where the channels are type based. 

For each TypePipe instance any Event Type will be sent and received on the same channel.

Adding a listener for an event returns a Subscription to control the listener lifetime. RAII style.

## What makes it simple?

Single header - just #include "TypePipe.h"

No threads. No message loop. No listener base class to inherit from.

Event sending causes synchronous dispatch. Don't use this if you need asynchronous events.

Event messages are always sent as copies.

## Usage examples

The examples folder has a couple of demos you can build and run, including a JUCE demo.

```cpp
    norm::TypePipe typePipe;

    auto subscription = eventSystem.add<std::string>([](std::string message) {
        std::cout << "Main Handler : " << message << "\n";
    });

    typePipe.send(std::string { "Event One" });

    /**
     * Use any copyable type as an event message.
     */
    struct ComplexEvent {
        int id = 0;
        std::string name{};
    };
    
    auto complexSubscription = typePipe.add<ComplexEvent>([](auto event) {
        std::cout << "Complex Event " << event.id << " -> " << event.name << "\n";
    });

    typePipe.send(ComplexEvent{ 23, "Normalised" });
```