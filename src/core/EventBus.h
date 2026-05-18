#ifndef RR99_CORE_EVENTBUS_H
#define RR99_CORE_EVENTBUS_H

#include <any>
#include <functional>
#include <map>
#include <typeindex>
#include <vector>

namespace rr99 {

class EventBus {
public:
    template<typename T>
    using Callback = std::function<void(const T&)>;

    template<typename T>
    void subscribe(Callback<T> callback) {
        subscribers[typeid(T)].push_back([callback](const std::any& event) { callback(std::any_cast<const T&>(event)); });
    }

    template<typename T>
    void publish(const T& event) {
        auto it = subscribers.find(typeid(T));
        if (it != subscribers.end()) {
            for (auto& callback : it->second) {
                callback(event);
            }
        }
    }

private:
    std::map<std::type_index, std::vector<std::function<void(const std::any&)>>> subscribers;
};

} // namespace rr99

#endif // RR99_CORE_EVENTBUS_H
