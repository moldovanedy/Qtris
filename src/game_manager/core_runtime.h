#include "main_loop.h"
#include "play_field.h"

namespace GameManager {
    class CoreRuntime {
    public:
        CoreRuntime(CoreRuntime &other) = delete;
        ~CoreRuntime() {};
        static CoreRuntime *getInstance();

    private:
        CoreRuntime();
        static CoreRuntime *_instance;

        std::function<void()> _updateCallback;
        void onUpdate();
    };
}