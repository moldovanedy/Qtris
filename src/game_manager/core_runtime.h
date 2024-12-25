#include "main_loop.h"
#include "play_field.h"
#include "current_piece.h"
#include "../ui/play/piece_manager.h"

namespace GameManager {
    class CoreRuntime {
    public:
        CoreRuntime(CoreRuntime &other) = delete;
        ~CoreRuntime();
        static CoreRuntime *getInstance();

    private:
        CoreRuntime();
        static CoreRuntime *_instance;

        void onUpdate();
        void checkForLineClears();

        bool _isInLineClearAnimation = false;
        int _lineClearAnimationStepsLeft = 0;
        int _rowsToClear[4]{ 0 };
    };
}