#include <mgos.h>

#include <mgos_utils_interval.h>
#include <memory>

class interval_test {
    public:
        interval_test() {
            loop = mgos_utils::interval(500, [this](){
                LOG(LL_INFO, ("Test interval v1 count %i", interval_count++));
                if (interval_count > 5) {
                    loop = mgos_utils::interval(1000, [this]() {
                        LOG(LL_INFO, ("Test interval v2 count %i", interval_count++));
                        if (interval_count > 7) {
                            LOG(LL_INFO, ("Stop test interval"));
                            loop.stop();
                        }
                    });
                }
            });
        }
    private: 
        mgos_utils::interval loop;
        int interval_count = 0;
};

std::unique_ptr<interval_test> test;

extern "C" enum mgos_app_init_result mgos_app_init(void) {

    test = std::unique_ptr<interval_test>(new interval_test());

    return MGOS_APP_INIT_SUCCESS;
}
