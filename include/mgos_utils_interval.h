#pragma once

#include <mgos_timers.h>
#include <functional>

namespace mgos_utils {
    class interval {
        using interval_function_t = std::function<void(void)>;
        public:
            interval() = default;
            interval(const interval&) = delete;
            interval& operator=(const interval&) = delete;
            interval(interval&&) = delete;
            interval& operator=(interval&& other);
            interval(int millis, interval_function_t f);
            void start();
            void stop();
            ~interval();
        private:
            bool running = false;
            void do_it();
            mgos_timer_id id;
            int repeat_millis;
            interval_function_t function;
    };
}

