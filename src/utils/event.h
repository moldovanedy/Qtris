#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <functional>

namespace Utils {
    typedef void(*eventListener(void));

    class Event {
    public:
        void clearAllListeners() {
            _callbacks.clear();
        }

        void addListener(std::function<void()> callback) {
            _callbacks.push_back(callback);
        }

        bool removeListener(std::function<void()> callback) {
            bool result = false;
            for (int i = 0; i < _callbacks.size(); i++) {
                if (_callbacks[i].target_type() == callback.target_type()) {
                    _callbacks.erase(_callbacks.begin() + i);
                    result = true;
                    break;
                }
            }

            return result;
        }

        void invoke() {
            for (int i = 0; i < _callbacks.size(); i++) {
                _callbacks[i]();
            }
        }

    private:
        std::vector<std::function<void()>> _callbacks;
    };
}
#endif