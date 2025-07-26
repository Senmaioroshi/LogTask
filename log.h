#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <string>
#include <mutex>
#include <ctime>
#include <memory>
using namespace std;
enum class LogLevel {
    NOTE,
    ALERT,
    ERR
};

class Log {
private:
    ofstream file;
    LogLevel min_level;
    mutable mutex mtx;

public:
    Log(const string& filename, LogLevel level = LogLevel::NOTE);
    ~Log();
    void write(const string& msg, LogLevel level = LogLevel::NOTE);
    void set_level(LogLevel new_level);
    static shared_ptr<Log> create_logger(const string& filename, LogLevel level = LogLevel::NOTE);

private:
    string level_to_str(LogLevel level) const;
};

#endif