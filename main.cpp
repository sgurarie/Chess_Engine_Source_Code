#include "MoveGenerator.h"
#include <fstream>
#include <sstream>
#include <mach/mach.h>

using namespace std;
typedef long long ll;

ll getMemoryUsage() {
    task_basic_info info;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS) {
        return 0;
    }
    return info.resident_size; // bytes
}

int main() {

    MoveGenerator game;
    game.playGame();
}


