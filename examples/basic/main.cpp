#include <iostream>
#include "../../EventSystem.h"

int main() {
    norm::EventSystem eventSystem;

    auto subscription = eventSystem.add<std::string>([](std::string message) {
        std::cout << "Main Handler : " << message << "\n";
    });

    eventSystem.send(std::string {"Event One"});

    /**
     * Use any copyable type as an event message.
     */
    struct ComplexEvent {
        int id = 0;
        std::string name{};
    };

    auto complexSubscription = eventSystem.add<ComplexEvent>(
            [](auto event) {
                std::cout << "Complex Event " << event.id << " -> " << event.name << "\n";
            });

    eventSystem.send(ComplexEvent{23, "Normalised"});
}