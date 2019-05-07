#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>

std::vector<std::tuple<bool, bool, int, int, int>> BuildSQRT(const std::vector<int> &array) {
    std::vector<std::tuple<bool, bool, int, int, int>> result;
    int blockSz = static_cast<int>(std::ceil(std::sqrt(array.size())));
    for (int i = 0; i < array.size(); i += blockSz) {
        bool res = true;
        int j = i;
        for (; j < array.size() - 1 && j < i + blockSz - 1; ++j) {
            if (array[j] > array[j + 1]) {
                res = false;
                break;
            }
        }
        if (i + blockSz - 1 < array.size()) {
            result.emplace_back(res, false, 0, array[i], array[i + blockSz - 1]);
        } else {
            result.emplace_back(res, false, 0, array[i], array[array.size() - 1]);
        }
    }
    return result;
}

void update_tuple(std::tuple<bool, bool, int, int, int> &tup,
                  bool b1, bool b2, int i1, int i2, int i3) {
    std::get<0>(tup) = b1;
    std::get<1>(tup) = b2;
    std::get<2>(tup) = i1;
    std::get<3>(tup) = i2;
    std::get<4>(tup) = i3;
}

int main() {
    int num_months, num_requests;
    std::cin >> num_months >> num_requests;
    std::vector<int> m_vals;
    for (int i = 0; i != num_months; ++i) {
        int val;
        std::cin >> val;
        m_vals.push_back(val);
    }
    auto up_data = BuildSQRT(m_vals);
    int blockSz = static_cast<int>(std::ceil(std::sqrt(num_months)));
    for (int i = 0; i != num_requests; ++i) {
        int req_type, left, right;
        std::cin >> req_type >> left >> right;
        --left;
        --right;
        int left_pos = left / blockSz, right_pos = right / blockSz;
        if (req_type == 1) {
            int val;
            std::cin >> val;
            bool new_mono = true;
            int j = left_pos * blockSz;
            for (; j < num_months && j < (left_pos + 1) * blockSz; ++j) {
                if (std::get<1>(up_data[left_pos])) {
                    m_vals[j] = std::get<2>(up_data[left_pos]);
                }
                if (j >= left && j <= right) {
                    m_vals[j] = val;
                }
                if (m_vals[j - 1] > m_vals[j] && j != left_pos * blockSz) {
                    new_mono = false;
                }
            }
            update_tuple(up_data[left_pos], new_mono, false, 0, m_vals[left_pos * blockSz],
                         m_vals[j - 1]);
            j = left_pos + 1;
            for (; j < right_pos; ++j) {
                update_tuple(up_data[j], true, true, val, val, val);
            }
            new_mono = true;
            j = right_pos * blockSz;
            for (; j < num_months && j < (right_pos + 1) * blockSz; ++j) {
                if (std::get<1>(up_data[right_pos])) {
                    m_vals[j] = std::get<2>(up_data[right_pos]);
                }
                if (j <= right && j >= left) {
                    m_vals[j] = val;
                }
                if (m_vals[j - 1] > m_vals[j] && j != right_pos * blockSz) {
                    new_mono = false;
                }
            }
            update_tuple(up_data[right_pos], new_mono, false, 0, m_vals[right_pos * blockSz],
                         m_vals[j - 1]);
        } else if (req_type == 2) {
            bool ans = true;
            int j = left_pos * blockSz;
            bool new_mono = true;
            for (; j < (left_pos + 1) * blockSz && j < num_months; ++j) {
                if (std::get<1>(up_data[left_pos])) {
                    m_vals[j] = std::get<2>(up_data[left_pos]);
                } else if (m_vals[j] > m_vals[j + 1] && j < (left_pos + 1) * blockSz - 1) {
                    if (j >= left && j < right) {
                        ans = false;
                    }
                    new_mono = false;
                }
            }
            if (left_pos != right_pos &&
                std::get<4>(up_data[left_pos]) > std::get<3>(up_data[left_pos + 1])) {
                ans = false;
            }
            update_tuple(up_data[left_pos], new_mono, false, 0, m_vals[left_pos * blockSz],
                         m_vals[j - 1]);
            for (int k = left_pos + 1; k < right_pos; ++k) {
                if (!std::get<0>(up_data[k]) ||
                    std::get<4>(up_data[k]) > std::get<3>(up_data[k + 1])) {
                    ans = false;
                }
            }
            j = right_pos * blockSz;
            new_mono = true;
            for (; j < (right_pos + 1) * blockSz && j < num_months; ++j) {
                if (std::get<1>(up_data[right_pos])) {
                    m_vals[j] = std::get<2>(up_data[right_pos]);
                } else if (m_vals[j] > m_vals[j + 1]) {
                    if (j >= left && j < right) {
                        ans = false;
                    }
                    if (j < (right_pos + 1) * blockSz - 1) {
                        new_mono = false;
                    }
                }
            }
            update_tuple(up_data[right_pos], new_mono, false, 0, m_vals[right_pos * blockSz],
                         m_vals[j - 1]);
            if (ans) {
                std::cout << "Yes\n";
            } else {
                std::cout << "No\n";
            }
        }
    }
}