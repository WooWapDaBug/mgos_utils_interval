#include <mgos.h>

#include <mgos_utils_interval.h>

#include <functional>
#include <memory>
#include <mgos_timers.h>

#define MGOS_TIMER_DO_ONCE false

namespace mgos_utils {

    using interval_function_t = std::function<void(void)>;

    interval::interval(int millis, interval_function_t f) : 
        repeat_millis(millis), function(f)
    {
        start();
    }

    void interval::start() {
        if (!running) {
            running = true;
            id = mgos_set_timer(repeat_millis, MGOS_TIMER_DO_ONCE, [](void* this_interval) {
                auto interval = reinterpret_cast<mgos_utils::interval*>(this_interval);
                if (interval->running) interval->function(); 
                // Check again as the called function might stop the interval
                if (interval->running) interval->start();
            }, this);
        } else {
            stop();
            start();
        }
    }

    void interval::stop() {
        if (running) {
            running = false;
            mgos_clear_timer(id);
        }
    }

    interval& interval::operator=(interval&& other) {
        other.stop();
        function = other.function;
        repeat_millis = other.repeat_millis;
        start();
        return *this;
    }

    interval::~interval() {
        stop();
    }
}
