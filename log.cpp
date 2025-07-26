#include "log.h"
#include <iostream>

Log::Log(const string& filename, LogLevel level)
    : min_level(level)
{
    file.open(filename, ios::app);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл журнала: " + filename);
    }
}

Log::~Log() {
    if (file.is_open()) {
        file.close();
    }
}

void Log::write(const string& msg, LogLevel level) {
    lock_guard<mutex> lock(mtx);
    
    if (level >= min_level) {
        time_t now = time(nullptr);
        tm timeinfo;
        localtime_r(&now, &timeinfo);
        
        char time_str[30];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &timeinfo);
        
        try {
            file << "[" << time_str << "] [" << level_to_str(level) << "]: " << msg << endl;
            file.flush();
        } catch (const exception& e) {
            cerr << "Ошибка записи в журнал: " << e.what() << endl;
        }
    }
}

void Log::set_level(LogLevel new_level) {
    lock_guard<mutex> lock(mtx);
    min_level = new_level;
}

shared_ptr<Log> Log::create_logger(const string& filename, LogLevel level) {
    return make_shared<Log>(filename, level);
}

string Log::level_to_str(LogLevel level) const {
    switch (level) {
        case LogLevel::NOTE:    return "NOTE";
        case LogLevel::ALERT:  return "ALERT";
        case LogLevel::ERR:    return "ERR";
        default:               return "UNKNOWN";
    }
}