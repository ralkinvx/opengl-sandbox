#pragma once

#include <iostream>
#include <ostream>
#include <string>


namespace logger {

    enum class Severity {
        FATAL,
        ERR,
        WARN,
        INFO
    };

    inline void log(Severity severity, std::string tag, std::string message) {
        std::string color;
        std::string prefix;

        switch (severity) {
            case Severity::FATAL:   color = "\033[91m"; prefix = "FATAL";   break;
            case Severity::ERR:   color = "\033[31m"; prefix = "ERROR";   break;
            case Severity::WARN: color = "\033[33m"; prefix = "WARNING"; break;
            case Severity::INFO:    color = "\033[0m";  prefix = "INFO";    break;
        };

        switch (severity) {
            case Severity::FATAL: std::cerr << color << "[" << prefix << "]" << " (" << tag << ") " << message << "\033[0m" << std::endl; break;

            case Severity::ERR: std::cout << color << "[" << prefix << "]" << " (" << tag << ") " << message << "\033[0m" << "\n"; break;

            case Severity::WARN: std::cout << color << "[" << prefix << "]" << " (" << tag << ") " << message << "\033[0m" << "\n"; break;

            case Severity::INFO: std::cout << color << "[" << prefix << "]" << " (" << tag << ") " << message << "\033[0m" << "\n"; break;
        };
        
    };

    inline int self_test() {

        int success = 0;

        auto cout_check = []() -> int {
            if (std::cout.fail()) {
                log(Severity::FATAL, "logger_self-test", "std::cout has failed.");
                return -1;
            }

            return 0;
        };

        logger::log(logger::Severity::FATAL, "logger_self-test", "test 1/4");
        
        if (std::cerr.fail()) {
            logger::log(Severity::ERR, "logger_self-test", "std::cerr has failed.");
            return -1;
        } else {
            success += 1;
            logger::log(Severity::FATAL, "logger_self-test", "success " + std::to_string(success) + "/4");
        }

        logger::log(logger::Severity::ERR, "logger_self-test", "test 2/4");
        if (cout_check() != 0) { return -1; }
        else {
            success += 1;
            logger::log(Severity::ERR, "logger_self-test", "success " + std::to_string(success) + "/4");
        }

        logger::log(logger::Severity::WARN, "logger_self-test", "test 3/4");
        if (cout_check() != 0) {return -1;}
        else {
            success += 1;
            logger::log(Severity::WARN, "logger_self-test", "success " + std::to_string(success) + "/4");
        }

        logger::log(logger::Severity::INFO, "logger_self-test", "test 4/4");
        if (cout_check() != 0) {return -1;}
        else {
            success += 1;
            logger::log(Severity::INFO, "logger_self-test", "success " + std::to_string(success) + "/4");
        }

        if (success == 4) {
            logger::log(Severity::INFO, "logger_self-test", "Logger self-test completed successfully.");
        } else {
            logger::log(Severity::ERR, "logger_self-test", "Logger self-test completed with errors. Logging may not work properly.");
        }

        return 0;
    }
}
