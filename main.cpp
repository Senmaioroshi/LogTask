#include <iostream>
#include <thread>
#include <atomic>
#include <memory>
#include "log.h"

using namespace std;

atomic<bool> running(true);

void log_thread(shared_ptr<Log> logger, const string& msg, LogLevel level) {
    logger->write(msg, level);
}

int main() {
    string filename;
    string level_str;
    string message;

    do {
        cout << "Введите имя файла журнала (.txt): ";
        getline(cin, filename);
    } while (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt");

    LogLevel level = LogLevel::NOTE;
    do {
        cout << "Введите минимальный уровень логирования (NOTE, ALERT, ERR): ";
        getline(cin, level_str);
        
        if (level_str == "ALERT") level = LogLevel::ALERT;
        else if (level_str == "ERR") level = LogLevel::ERR;
    } while (level_str != "NOTE" && level_str != "ALERT" && level_str != "ERR");

    auto logger = Log::create_logger(filename, level);

    while (running) {
        cout << "Введите сообщение (или 'q' для выхода): ";
        getline(cin, message);

        if (message == "q") {
            running = false;
            continue;
        }

        LogLevel msg_level = LogLevel::NOTE;
        do {
            cout << "Введите уровень сообщения (NOTE, ALERT, ERR): ";
            getline(cin, level_str);
            
            if (level_str == "ALERT") msg_level = LogLevel::ALERT;
            else if (level_str == "ERR") msg_level = LogLevel::ERR;
        } while (level_str != "NOTE" && level_str != "ALERT" && level_str != "ERR");

        thread t(log_thread, logger, message, msg_level);
        t.detach();
    }

    cout << "Работа завершена." << endl;
    return 0;
}