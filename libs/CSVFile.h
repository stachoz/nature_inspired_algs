#pragma once

#include <fstream>
#include <sstream>
#include <mutex>
#include <string>
#include <utility>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <locale>
#include <iomanip>
#include <algorithm>

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

    template<typename T>
    std::string format_field(const T& value) {
        if constexpr (std::is_floating_point_v<T>) {
            std::stringstream ss;
            ss.imbue(std::locale("pl_PL.UTF-8"));
            ss << std::fixed << std::setprecision(10) << value;
            return ss.str();
        } else {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
    }

    std::string format_field(const double& value) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(10) << value;
        std::string result = ss.str();

        std::replace(result.begin(), result.end(), '.', ',');

        return result;
    }

    template<typename T, typename = std::enable_if_t<!std::is_floating_point_v<T>>>
    std::string format_field(const T& value) {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }


    template<typename... Args>
    void append_row(const Args&... args) {
        std::lock_guard lock(mutex);
        if (!file.is_open()) {
            throw std::runtime_error("CSVFile::append: file is not open: " + file_path.string());
        }

        auto write_field = [this](const auto& arg) {
            if constexpr (std::is_floating_point_v<std::decay_t<decltype(arg)>>) {
                 file << format_field(arg);
            } else {
                 file << arg;
            }
            file << ';';
        };

        (write_field(args), ...);

        file.seekp(-1, std::ios::cur);
        file << '\n';
    }

    template <typename T>
    void append_vector_as_row(const std::vector<T>& v) {
        if (!file.is_open()) {
            throw std::runtime_error("CSVFile::append: file is not open");
        }

        for (size_t i = 0; i < v.size(); i++) {
            if constexpr (std::is_floating_point_v<T>) {
                file << format_field(v[i]);
            } else {
                file << v[i];
            }

            if (i + 1 < v.size()) {
                file << ';';
            }
        }
        file << '\n';
    }

    void close_file() {
        if (file.is_open()) {
            file.close();
        }
    }

    template<typename Func>
    void process_data(Func&& process_func, char delimiter = ';') const {
        std::ifstream in(file_path);
        if (!in.is_open()) {
            throw std::runtime_error("CSVFile::process_data: cannot open file: " + file_path.string());
        }

        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) {
                continue;
            }

            std::vector<std::string> fields;
            std::istringstream iss(line);
            std::string value;

            while(std::getline(iss, value, delimiter)) {
                fields.push_back(value);
            }

            process_func(fields);
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