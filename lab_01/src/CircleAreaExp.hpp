#pragma once
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numbers>
#include <numeric>
#include <random>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

#include "CSVFile.h"

class CircleAreaExp {
public:
    explicit CircleAreaExp(size_t points, std::filesystem::path path) :
        points_num(points), csv_file(std::move(path)) {}

    void generate_data() {

        unsigned int num_threads = std::thread::hardware_concurrency();
        if(num_threads == 0) {
            num_threads = 1;
        }
        std::vector<std::thread> threads;

        size_t chunk_size = points_num / num_threads;
        size_t chunk_remainder = points_num % num_threads;

        for(int i = 0; i < num_threads; i++) {

            size_t this_chunk_size = chunk_size + (i == num_threads - 1 ? chunk_remainder : 0);

            threads.emplace_back([this_chunk_size, this]() {
                constexpr int local_buff_size = 10'000;

                std::vector<std::tuple<double, double, bool>> local_points;
                local_points.reserve(local_buff_size);

                std::mt19937 gen(std::random_device{}());
                std::uniform_real_distribution<double> dist_x {rect.a.first,rect.b.first};
                std::uniform_real_distribution<double> dist_y {rect.a.second,rect.d.second};

                for(size_t j = 0; j < this_chunk_size; j++) {
                    const double x = dist_x(gen);
                    const double y = dist_y(gen);

                    local_points.emplace_back(x, y, circ.contains({x, y}));

                    if(local_points.size() >= local_buff_size) {
                        csv_file.append(local_points);
                        local_points.clear();
                    }
                }

                csv_file.append(local_points);
            });
        }

        for(auto& t : threads) {
            t.join();
        }

        csv_file.close_file();

    }

    void calculate_results(const std::unordered_set<size_t>& targets) const {
        std::vector<std::pair<size_t, size_t>> results; /// target - inside counter pair

        size_t line_counter = 1;
        size_t inside_counter = 0;

        auto compute_points_inside = [this, &line_counter, &inside_counter, &results, &targets](const std::vector<std::string>& fields) {

            if(fields[2]== "1") {
                inside_counter++;
            }

            if(targets.contains(line_counter) || line_counter == points_num) {
                results.emplace_back(line_counter, inside_counter);
            }

            line_counter++;
        };

        csv_file.process_data(compute_points_inside);
        present_results(results);
    }

    void present_results(const std::vector<std::pair<size_t, size_t>>& results) const {
        const double rect_area = rect.area();
        std::cout << "generated points: " << points_num << std::endl;
        std::cout << "computed rectangle area: " << rect_area << std::endl;
        std::cout << "computed circle area: " << circ.area() << std::endl;

        for(const auto& [target, result] : results) {
            std::cout << target << "-> " << result <<  " monte carlo area: " << rect_area * static_cast<double>(result) / target << std::endl;
        }
    }

    struct Rectangle {
        std::pair<double,double> a;
        std::pair<double,double> b;
        std::pair<double,double> c;
        std::pair<double,double> d;

        double area() const {
            double x_len = std::abs(a.first - b.first);
            double y_len = std::abs(a.second - d.second);
            return x_len * y_len;
        }
    };

    struct Circle {
        std::pair<double, double> mid_co;
        double radius;

        bool contains(std::pair<double, double> point) const {
            auto& [mid_x, mid_y] = mid_co;
            auto& [point_x, point_y] = point;

            const double x_distance = mid_x - point_x;
            const double y_distance = mid_y - point_y;

            return x_distance * x_distance + y_distance * y_distance <= radius;
        }

        double area() const {
            return std::numbers::pi * radius * radius;
        }
    };

    struct Point {
        double x;
        double y;
        bool is_inside_circ;
    };

private:
    size_t points_num;
    CSVFile csv_file;
    Rectangle rect {{2,2},{7,2},{7,6},{2,6}};
    Circle circ { {5, 4},1};
};
