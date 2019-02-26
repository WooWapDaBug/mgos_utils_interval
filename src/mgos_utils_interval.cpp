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
        repeat_millis(millis), function(f)
    {
        this->name = name;
        start();
    }

    void interval::start() {
        if (running) { 
            stop(); 
        }
        running = true;
        id = mgos_set_timer(repeat_millis, MGOS_TIMER_REPEAT, [](void* user_data) {
            auto this_interval = reinterpret_cast<interval*>(user_data);
            //LOG(LL_INFO, ("Interval %s ID Running: %i", this_interval->name.c_str(), this_interval->id));
            this_interval->do_it();
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
