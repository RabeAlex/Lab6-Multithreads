// Copyright 2018 Your Name <your_email>

#include <header.hpp>

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
        for (size_t i = 0; i < threadNumber; ++i) {
            std::thread th(hashHandler);
            allThreads.push_back(std::move(th));
        }
        for (size_t i = 0; i < threadNumber; ++i) {
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
            const std::string hash =
             picosha2::hash256_hex_string(stringForHash);
            if (hash.find("0000", 60) == 60) {
                BOOST_LOG_TRIVIAL(info) << stringForHash
                 << " - " << hash << std::endl;
                break;
            } else {
                BOOST_LOG_TRIVIAL(trace) << stringForHash
                 << " - " << hash << std::endl;
            }
        }
    }
};

void init_logging() {
    boost::log::add_file_log(
            boost::log::keywords::file_name = "info.log",
            boost::log::keywords::format =
                    "[%LineID%][%TimeStamp%][%ThreadID%][%Severity%]%Message%");
    boost::log::add_console_log(
            std::cout,
            boost::log::keywords::format =
                    "[%LineID%][%TimeStamp%][%ThreadID%][%Severity%]%Message%");
}

int main() {
    init_logging();
    boost::log::add_common_attributes();
    hash test;
}
