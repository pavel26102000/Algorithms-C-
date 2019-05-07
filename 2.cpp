#include <iostream>
#include <cmath>
#include <vector>

class SegTree {
private:
    std::vector<int64_t> data;

    void BuildTree(const std::vector<int>& array, int left, int right, int vert);

public:
    explicit SegTree(int size);

    void Update(int t_left, int t_right, int v, int left, int right, int add);

    int64_t Get(int vert, int t_left, int t_right, int pos);
};

SegTree::SegTree(int size) {
    data.resize(4 * size);
}

void SegTree::BuildTree(const std::vector<int> &array, int left, int right, int vert) {
    if (left == right) {
        data[vert] = array[left];
    } else {
        int middle = (left + right) / 2;
        BuildTree(array, left, middle, vert * 2 + 1);
        BuildTree(array, middle + 1, right, vert * 2 + 2);
    }
}

void SegTree::Update(int t_left, int t_right, int vert, int left, int right, int add) {
    if (left > right) {
        return;
    }
    if (left == t_left && right == t_right) {
        data[vert] += add;
    } else {
        int t_middle = (t_left + t_right) / 2;
        Update(t_left, t_middle, 2 * vert + 1, left, std::min(right, t_middle), add);
        Update(t_middle + 1, t_right, 2 * vert + 2, std::max(left, t_middle + 1), right, add);
    }
}

int64_t SegTree::Get(int vert, int t_left, int t_right, int pos) {
    if (t_left == t_right) {
        return data[vert];
    }
    int t_middle = (t_left + t_right) / 2;
    if (pos <= t_middle) {
        return data[vert] + Get(vert * 2 + 1, t_left, t_middle, pos);
    } else {
        return data[vert] + Get(2 * vert + 2, t_middle + 1, t_right, pos);
    }
}

std::vector<std::vector<int>> build_divisors(int n) {
    std::vector<std::vector<int>> result;
    result.resize(n);
    for (int i = 0; i != n; ++i) {
        for (int d = 1; d <= std::sqrt(i + 1); ++d) {
            if ((i + 1) % d == 0) {
                result[i].push_back(d);
                if (d != std::sqrt(i + 1)) {
                    result[i].push_back((i + 1) / d);
                }
            }
        }
    }
    return result;
}


int main() {
    int num_particles;
    std::cin >> num_particles;
    auto divisors = build_divisors(num_particles);
    std::vector<int> charges;
    charges.reserve(num_particles);
    for (int i = 0; i != num_particles; ++i) {
        int charge;
        std::cin >> charge;
        charges.push_back(charge);
    }
    SegTree tree(num_particles);
    int num_requests;
    std::cin >> num_requests;
    for (int i = 0; i != num_requests; ++i) {
        int req_type;
        std::cin >> req_type;
        if (req_type == 1) {
            int index;
            int64_t ans = 0;
            std::cin >> index;
            for (auto div : divisors[--index]) {
                ans += tree.Get(0, 0, charges.size() - 1, --div);
            }
            std::cout << charges[index] + ans << "\n";
        } else if (req_type == 2) {
            int left, right, add;
            std::cin >> left >> right >> add;
            tree.Update(0, charges.size() - 1, 0, --left, --right, add);
        }
    }
    return 0;
}