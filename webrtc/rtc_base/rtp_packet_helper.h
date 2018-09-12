
#ifndef RTP_RAW_PACKET_HELPER_H_
#define RTP_RAW_PACKET_HELPER_H_

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

namespace rtc{
namespace rtp{
    
static std::string rtp_exec_cmd(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

static std::string rtp_get_user_data_dir() {
    // get current pid
    pid_t currPid = ::getpid();
    
    // get current process command line
    char cmd1[128];
    sprintf(cmd1, "ps -ww -fp %d", currPid);

    std::string result1 = rtp_exec_cmd(cmd1);

    // get --user-data-dir
    std::string token = "--user-data-dir=";
    std::size_t found = result1.find(token);
    if (found == std::string::npos) {
      return "";
    }

    // find next --
    std::size_t nextPos = result1.find("--", found + token.length());
    if (nextPos != std::string::npos) {
      std::string ret = result1.substr(found + token.length(), nextPos - found - token.length());
      ret.erase(ret.find_last_not_of("\t\n\v\f\r ") + 1);
      return ret;
    }

    return "";
}


}
}

#endif
