// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <iostream>
#include "picosha2.h"
#include <thread>
#include <vector>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/expressions.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

class hash {
public:
    size_t threadNumber;
    std::vector <std::thread> allThreads;

    hash() {
        threadNumber = std::thread::hardware_concurrency();
        setThreads();
    }

    explicit hash(int number) {
        threadNumber = number;
        setThreads();
    }

    void setThreads() {
        for (size_t i = 0; i < threadNumber; i++) {
            std::thread th(hashHandler);
            allThreads.push_back(std::move(th));
        }
        for (size_t i = 0; i < threadNumber; i++) {
            allThreads.at(i).join();
        }
    }

    static void hashHandler() {
        while (true){
            int stringLength = 30;
            std::string stringForHash;
            for (int i = 0; i < stringLength; ++i) {
                stringForHash.push_back(32+std::rand()%94);
            }
            const std::string hash = picosha2::hash256_hex_string(stringForHash);
            if (hash.find("0000", 60) == 60) {
                BOOST_LOG_TRIVIAL(info) << stringForHash << " - " << hash << std::endl;
                break;
            } else {
                BOOST_LOG_TRIVIAL(trace) << stringForHash << " - " << hash << std::endl;
            }
        }
    }
};

void init_logging() {
    logging::add_file_log (
            keywords::file_name = "info.log",
            keywords::format = "[%LineID%] [%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );
    logging::add_console_log (
            std::cout,
            keywords::format = "[%LineID%] [%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );
}

int main() {
    init_logging();
    logging::add_common_attributes();
    hash test;
}

#endif // INCLUDE_HEADER_HPP_
