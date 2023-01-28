#pragma once

#include <functional>
#include <typeindex>
#include <vector>
#include <memory>
#include <unordered_map>

/**
 * Simple type-based event system.
 *
 * Make an EventSystem, add a listener, send messages.
 *
 * norm::EventSystem eventSystem;
 * auto subscription = eventSystem.add<std::string>([](std::string message) {
 *      std::cout << message << "\n";
 * });
 * eventSystem.send("Testing Testing 1 2 3");
 *
 * ---
 *
 * Subscription type is available as norm::Subscription<EventType>
 * for places you cant or dont want to use auto.
 * e.g.
 * norm::Subscription<std::string> mySubscription;
 */
namespace norm {

    struct EventPipeBase {
        virtual ~EventPipeBase() = default;
    };

    struct EventSystem;

    /**
     * Subscription lifetime control, RAII style.
     * @tparam EventType
     */
    template<typename EventType>
    struct Handler {
        Handler(EventSystem *theSystem, std::function<void(EventType)> theFn)
                : system(theSystem), fn(std::move(theFn)) {
        }

        ~Handler();

        void run(EventType event) {
            fn(event);
        }

        bool operator==(const Handler<EventType> &other) {
            return this == &other;
        }

        EventSystem *system = nullptr;
        std::function<void(EventType)> fn;
    };

    /**
     * A message pipe for a single EventType
     * @tparam EventType
     */
    template<typename EventType>
    class EventPipe : public EventPipeBase {
    public:
        void add(Handler<EventType> *handler) {
            handlers.emplace_back(handler);
        }

        void remove(Handler<EventType> *handler) {
            handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
                                          [handler](auto &item) {
                                              return item == handler;
                                          }));
        }

        void send(EventType event) {
            for (auto &handler: handlers) {
                handler->run(event);
            }
        }

        std::vector<Handler<EventType> *> handlers;
    };

    template<typename EventType> using Subscription = std::unique_ptr<Handler<EventType>>;

    struct EventSystem {
        /**
         * Add a listener for EventType messages.
         * The listener will be removed when the returned handle is destructed.
         * @tparam EventType
         * @param handler
         * @return Handle which controls the listener lifetime.
         */
        template<typename EventType>
        Subscription<EventType> add(std::function<void(EventType)> handler) {
            auto handlerWrapper = std::make_unique<Handler<EventType>>(this, handler);
            getPipe<EventType>().add(handlerWrapper.get());
            return handlerWrapper;
        }

        /**
         * Send a message of EventType.
         * This will be synchronously dispatched to all listeners for EventType messages
         * @tparam EventType
         * @param event
         */
        template<typename EventType>
        void send(EventType event) {
            getPipe<EventType>().send(event);
        }

        template<typename EventType>
        void removeHandler(Handler<EventType> *handler) {
            getPipe<EventType>().remove(handler);
        }

        template<typename EventType>
        auto &getPipe() {
            const auto typeIndex = std::type_index(typeid(EventType));
            if (pipes[typeIndex] == nullptr) {
                pipes[typeIndex] = std::make_unique<EventPipe<EventType>>();
            }

            return *static_cast<EventPipe<EventType> *>(pipes[typeIndex].get());
        }
    private:

        std::unordered_map<std::type_index, std::unique_ptr<EventPipeBase>> pipes;
    };

    /**
     * RAII style removal of handler from EventSystem when the Handler dies.
     * @tparam EventType
     */
    template<typename EventType>
    Handler<EventType>::~Handler() {
        system->template removeHandler<EventType>(this);
    }

} // namespace normalised
