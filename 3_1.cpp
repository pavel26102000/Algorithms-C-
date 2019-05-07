#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

std::vector<std::vector<int>> Build_ST(const std::vector<int> &array) {
    std::vector<std::vector<int>> table;
    table.resize(array.size());
    for (int i = 0; i != array.size(); ++i) {
        table[i].resize(static_cast<size_t>(std::log2(array.size())) + 1);
    }
    for (int col = 0; col < table[0].size(); ++col) {
        for (int row = 0; row != array.size(); ++row) {
            if (col == 0) {
                table[row][col] = array[row];
            } else {
                if (row + static_cast<size_t>(std::pow(2, col - 1)) < array.size()) {
                    table[row][col] = std::min(table[row][col - 1],
                                               table[row +
                                                     static_cast<size_t>(std::pow(2, col - 1))][
                                                       col - 1]);
                } else {
                    table[row][col] = table[row][col - 1];
                }
            }
        }
    }
    return table;
}

int fl_log(int n) {
    if (n == 1) {
        return 0;
    } else {
        return fl_log(n / 2) + 1;
    }
}

std::vector<int> Build_Logs(int n) {
    std::vector<int> result;
    result.reserve(static_cast<unsigned long>(n));
    for (int i = 1; i != n + 1; ++i) {
        result.push_back(fl_log(i));
    }
    return result;
}

int main() {
    int num_elems, num_requests, fst_elem;
    std::cin >> num_elems >> num_requests >> fst_elem;
    std::vector<int> numbers(num_elems);
    for (int i = 0; i != num_elems; ++i) {
        numbers[i] = fst_elem;
        fst_elem = (23 * fst_elem + 21563) % 16714589;
    }
    auto table = Build_ST(numbers);
    std::vector<int> logs = Build_Logs(num_elems);
    int u, v;
    std::cin >> u >> v;
    for (int i = 1; i != num_requests + 1; ++i) {
        --v; --u;
        bool swapped = false;
        if (v < u) {
            std::swap(v, u);
            swapped = true;
        }
        int l = v - u;
        int ans = std::min(table[u][logs[l - 1]],
                           table[v - static_cast<int>(std::pow(2, logs[l - 1])) + 1][logs[l -
                                                                                          1]]);
        if (swapped) {
            std::swap(u, v);
        }
        ++v; ++u;
        if (i == num_requests) {
            std::cout << u << " " << v << " " << ans << "\n";
        }
        u = ((17 * u + 751 + ans + 2 * i) % num_elems) + 1;
        v = ((13 * v + 593 + ans + 5 * i) % num_elems) + 1;
    }
    return 0;
}
