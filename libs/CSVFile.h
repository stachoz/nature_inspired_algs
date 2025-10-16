#pragma once

#include <fstream>
#include <sstream>
#include <mutex>
#include <string>
#include <utility>
#include <vector>
#include <filesystem>
#include <stdexcept>

class CSVFile {
public:
    explicit CSVFile(std::filesystem::path path)
        : file_path(std::move(path)) {
        open_file();
    }

    ~CSVFile() {
        close_file();
    }

    void open_file() {
        std::filesystem::create_directories(file_path.parent_path());

        if (std::filesystem::exists(file_path)) {
            const auto stem = file_path.stem().string();
            const auto ext = file_path.extension().string();
            int counter = 1;
            while (std::filesystem::exists(file_path)) {
                file_path = file_path.parent_path() / (stem + "(" + std::to_string(counter++) + ")" + ext);
            }
        }

        file.open(file_path, std::ios::out);
        if (!file.is_open()) {
            throw std::runtime_error("CSVFile::open_file: cannot open file: " + file_path.string());
        }
    }

    template<typename TupleContainer>
    void append(const TupleContainer& data) {
        for(const auto& row : data) {
            std::apply([this](auto&&... args) {
                append_row(args...);
            }, row);
        }
    }

    template<typename... Args>
    void append_row(const Args&... args) {
        std::lock_guard lock(mutex);
        if (!file.is_open()) {
            throw std::runtime_error("CSVFile::append: file is not open: " + file_path.string());
        }
        ((file << args <<','), ...);
        file.seekp(-1, std::ios::cur);
        file << '\n';
    }

    void close_file() {
        std::lock_guard lock(mutex);
        if (file.is_open()) {
            file.close();
        }
    }

    template<typename Func>
    void process_data(Func&& process_func, char delimiter = ',') const {
        std::ifstream in(file_path);
        if (!in.is_open()) {
            throw std::runtime_error("CSVFile::process_data: cannot open file: " + file_path.string());
        }

        std::string line;
        size_t line_number = 0;
        while (std::getline(in, line)) {
            if (line.empty()) {
                continue;
            }
            ++line_number;

            std::vector<std::string> fields;
            std::istringstream iss(line);
            std::string value;

            while(std::getline(iss, value, delimiter)) {
                fields.push_back(value);
            }

            process_func(line_number, fields);
        }
    }

    std::filesystem::path get_filepath() const {
        return file_path;
    }

private:
    mutable std::mutex mutex;
    std::filesystem::path file_path;
    std::ofstream file;
};
