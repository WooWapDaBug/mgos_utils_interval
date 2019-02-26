#include <mgos.h>

#include <mgos_utils_interval.h>

#include <functional>
#include <memory>
#include <mgos_timers.h>
#include <string>

#define MGOS_TIMER_DO_ONCE false

namespace mgos_utils {

    using interval_function_t = std::function<void(void)>;

    interval::interval(int millis, interval_function_t f) : 
        repeat_millis(millis), function(f)
    {
        start();
    }

    interval::interval(std::string name, int millis, interval_function_t f) : 
        repeat_millis(millis), function(f), name(name)
    {
        start();
    }

    void interval::start() {
        if (running) { stop(); }
        running = true;
        LOG(LL_INFO, ("Interval STARTED %s", name.c_str()));
        id = mgos_set_timer(repeat_millis, MGOS_TIMER_DO_ONCE, [](void* this_interval) {
            reinterpret_cast<interval*>(this_interval)->do_it();
        }, this);
    }

    void interval::do_it() {
        if (running && function) {
            function(); 
        }
        // Check again as the called function might stop the interval
        if (running) {
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
        name = other.name;
        function = other.function;
        repeat_millis = other.repeat_millis;
        start();
        return *this;
    }

    interval::~interval() {
        stop();
    }
}
