#pragma once
#include <functional>
#include <typeindex>

namespace tap
{

struct EventPipeBase
{
    virtual ~EventPipeBase() = default;

    JUCE_LEAK_DETECTOR(EventPipeBase)
};

struct EventSystem;

template <typename EventType>
struct Handler
{
    Handler(EventSystem* theSystem, std::function<void(EventType)> theFn)
        : system(theSystem)
        , fn(std::move(theFn))
    {
    }

    ~Handler();

    void run(EventType event) { fn(event); }

    bool operator==(const Handler<EventType>& other) { return this == &other; }

    EventSystem* system = nullptr;
    std::function<void(EventType)> fn;

    //JUCE_LEAK_DETECTOR(Handler)
};

template <typename EventType>
class EventPipe : public EventPipeBase
{
public:
    void add(Handler<EventType>* handler) { handlers.emplace_back(handler); }

    void remove(Handler<EventType>* handler)
    {
        handlers.erase(std::remove_if(handlers.begin(),
                                      handlers.end(),
                                      [handler](auto& item) { return item == handler; }));
    }

    void send(EventType event)
    {
        for (auto& handler: handlers)
        {
            handler->run(event);
        }
    }

    std::vector<Handler<EventType>*> handlers;
};

struct EventSystem
{
    template <typename EventType>
    std::unique_ptr<Handler<EventType>> add(std::function<void(EventType)> handler)
    {
        auto handlerWrapper = std::make_unique<Handler<EventType>>(this, handler);
        getPipe<EventType>().add(handlerWrapper.get());
        return handlerWrapper;
    }

    template <typename EventType>
    void send(EventType event)
    {
        getPipe<EventType>().send(event);
    }

    template <typename EventType>
    void removeHandler(Handler<EventType>* handler)
    {
        getPipe<EventType>().remove(handler);
    }

    template <typename EventType>
    auto& getPipe()
    {
        const auto typeIndex = std::type_index(typeid(EventType));
        if (pipes[typeIndex] == nullptr)
        {
            pipes[typeIndex] = std::make_unique<EventPipe<EventType>>();
        }

        return *static_cast<EventPipe<EventType>*>(pipes[typeIndex].get());
    }

    std::unordered_map<std::type_index, std::unique_ptr<EventPipeBase>> pipes;

    JUCE_LEAK_DETECTOR(EventSystem)
};

template <typename EventType>
Handler<EventType>::~Handler()
{
    system->template removeHandler<EventType>(this);
}

} // namespace tap
