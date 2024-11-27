#ifndef EVENT_H
#define EVENT_H

#include <vector>

namespace Utils {
    typedef void(*eventListener(void));

    class Event {
    public:
        void addListener(eventListener callback) {
            _callbacks.push_back(callback);
        }

        bool removeListener(eventListener callback) {
            bool result = false;
            for (int i = 0; i < _callbacks.size(); i++) {
                if (_callbacks[i] == callback) {
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
        std::vector<eventListener *> _callbacks;
    };
}
#endif