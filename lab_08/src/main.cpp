#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <random>
#include <iomanip>
#include <string>
#include <filesystem>
#include <set>

namespace fs = std::filesystem;

struct Point {
    int id;
    std::vector<double> objectives;
};

bool dominates(const Point& b, const Point& a) {
    bool strictly_better = false;
    for (size_t i = 0; i < b.objectives.size(); ++i) {
        if (b.objectives[i] < a.objectives[i]) return false;
        if (b.objectives[i] > a.objectives[i]) strictly_better = true;
    }
    return strictly_better;
}

std::vector<Point> naive_nondominated(const std::vector<Point>& P) {
    std::vector<Point> result;
    for (size_t i = 0; i < P.size(); ++i) {
        bool is_dominated = false;
        for (size_t j = 0; j < P.size(); ++j) {
            if (i == j) continue;
            if (dominates(P[j], P[i])) {
                is_dominated = true;
                break;
            }
        }
        if (!is_dominated) result.push_back(P[i]);
    }
    return result;
}

std::vector<Point> front_kung(std::vector<Point>& P) {
    if (P.size() <= 1) return P;

    size_t mid = P.size() / 2;
    std::vector<Point> T_half(P.begin(), P.begin() + mid);
    std::vector<Point> B_half(P.begin() + mid, P.end());

    std::vector<Point> T = front_kung(T_half);
    std::vector<Point> B = front_kung(B_half);

    std::vector<Point> result = T;
    for (const auto& b_pt : B) {
        bool dominated_by_T = false;
        for (const auto& t_pt : T) {
            if (dominates(t_pt, b_pt)) {
                dominated_by_T = true;
                break;
            }
        }
        if (!dominated_by_T) result.push_back(b_pt);
    }
    return result;
}

std::vector<Point> kung_algorithm(std::vector<Point> P) {
    if (P.empty()) return {};
    std::sort(P.begin(), P.end(), [](const Point& a, const Point& b) {
        if (a.objectives[0] != b.objectives[0])
            return a.objectives[0] > b.objectives[0];
        return a.objectives[1] > b.objectives[1];
    });
    return front_kung(P);
}

void save_to_csv(const std::string& filename, const std::vector<Point>& all, const std::vector<Point>& non_dom) {
    fs::path dir(RESULTS_DIR);
    try {
        if (!fs::exists(dir)) fs::create_directories(dir);
        fs::path full_path = fs::absolute(dir / filename);
        std::ofstream ofs(full_path);

        if (!ofs.is_open()) throw std::runtime_error("Blad otwarcia pliku: " + full_path.string());

        ofs << "ID,";
        for (size_t i = 0; i < all[0].objectives.size(); ++i) ofs << "f" << i+1 << ",";
        ofs << "Is_Nondominated\n";

        std::set<int> nd_ids;
        for (const auto& nd : non_dom) nd_ids.insert(nd.id);

        for (const auto& p : all) {
            ofs << p.id << ",";
            for (double v : p.objectives) ofs << std::fixed << std::setprecision(6) << v << ",";
            ofs << (nd_ids.count(p.id) ? "1" : "0") << "\n";
        }
        std::cout << "[SUCCESS] Wyniki zapisano: " << full_path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

bool compare_results(const std::vector<Point>& res1, const std::vector<Point>& res2) {
    if (res1.size() != res2.size()) return false;
    std::set<int> ids1, ids2;
    for (const auto& p : res1) ids1.insert(p.id);
    for (const auto& p : res2) ids2.insert(p.id);
    return ids1 == ids2;
}

int main() {
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 100.0);

    auto generate = [&](int n, int m) {
        std::vector<Point> pts;
        for(int i = 0; i < n; ++i) {
            std::vector<double> obj(m);
            for(int j = 0; j < m; ++j) obj[j] = dist(rng);
            pts.push_back({i, obj});
        }
        return pts;
    };

    std::cout << "--- PODZADANIE 1: 100 pkt, 2D ---" << std::endl;
    auto data2d = generate(100, 2);
    auto kung2d = kung_algorithm(data2d);
    auto naive2d = naive_nondominated(data2d);

    save_to_csv("zad1_2d_kung.csv", data2d, kung2d);
    save_to_csv("zad1_2d_naive.csv", data2d, naive2d);
    std::cout << "Weryfikacja (2D): " << (compare_results(kung2d, naive2d) ? "IDENTYCZNE" : "ROZNE!") << std::endl;

    std::cout << "\n--- PODZADANIE 1: 1000 pkt, 5D ---" << std::endl;
    auto data5d = generate(1000, 5);
    auto kung5d = kung_algorithm(data5d);
    auto naive5d = naive_nondominated(data5d);

    save_to_csv("zad1_5d_kung.csv", data5d, kung5d);
    save_to_csv("zad1_5d_naive.csv", data5d, naive5d);
    std::cout << "Weryfikacja (5D): " << (compare_results(kung5d, naive5d) ? "IDENTYCZNE" : "ROZNE!") << std::endl;

    std::cout << "\n--- PODZADANIE 2 ---" << std::endl;
    std::vector<Point> p2_data;
    std::ifstream input("/home/stachoz/Projects/nature_inspired_algs/nature_inspired_algs/lab_08/punkty.txt");

    if (!input.is_open()) {
        std::cerr << "[ERROR] Brak pliku punkty.txt w katalogu roboczym!" << std::endl;
    } else {
        double x, y;
        int id_c = 0;
        while (input >> x >> y) p2_data.push_back({id_c++, {x, y}});
        input.close();

        fs::path p2_path = fs::absolute(fs::path(RESULTS_DIR) / "zad2_fronty.csv");
        std::ofstream p2_out(p2_path);

        if (p2_out.is_open()) {
            p2_out << "Front,Obj1,Obj2\n";
            int f_idx = 1;
            while (!p2_data.empty()) {
                auto current_front = kung_algorithm(p2_data);
                for (const auto& f_pt : current_front) {
                    p2_out << f_idx << "," << f_pt.objectives[0] << "," << f_pt.objectives[1] << "\n";
                    p2_data.erase(std::remove_if(p2_data.begin(), p2_data.end(), [&](const Point& p) {
                        return p.id == f_pt.id;
                    }), p2_data.end());
                }
                f_idx++;
            }
            std::cout << "[SUCCESS] Ranking zakonczony. Fronty: " << f_idx - 1 << std::endl;
            std::cout << "Zapisano: " << p2_path << std::endl;
        }
    }
    return 0;
}