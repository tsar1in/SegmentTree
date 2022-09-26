#include <iostream>
#include <vector>

class SegmentTree {
  std::vector<std::pair<int, int>> arr_;
  int size_tree_;
  int size_arr_;
  void Push(int node) {
    arr_[node].first += arr_[node].second;
    arr_[2 * node + 1].second += arr_[node].second;
    arr_[2 * node + 2].second += arr_[node].second;
    arr_[node].second = 0;
  }

 public:
  template <class Iterator>
  SegmentTree(Iterator begin, Iterator end) : size_tree_(0) {
    int n = end - begin;
    int h = std::__lg(n);
    int size = (1 << (h + 1));
    size_arr_ = size;
    arr_ = std::vector<std::pair<int, int>>(2 * size);
    for (int i = 0; i < n; i++) {
      arr_[i + size - 1] = std::make_pair(begin[i], 0);
      size_tree_++;
    }
    for (int i = n; i < size; i++) {
      arr_[i + size - 1] = std::make_pair(-2147483647, 0);
      size_tree_++;
    }
    for (int i = size - 2; i >= 0; i--) {
      arr_[i] = std::max(arr_[2 * i + 1], arr_[2 * i + 2]);
      size_tree_++;
    }
  }
  int QueryDown(int left, int right, int node, int a, int b) {
    if (a >= left && b <= right) {
      return arr_[node].first + arr_[node].second;
    }
    if (a > right || b < left) {
      return -2147483647;
    }
    Push(node);
    auto left_res = QueryDown(left, right, 2 * node + 1, a, (a + b) / 2);
    auto right_res = QueryDown(left, right, 2 * node + 2, ((a + b) / 2) + 1, b);
    return std::max(left_res, right_res);
  }
  void Update(int left, int right, int node, int a, int b, int delta) {
    if (a >= left && b <= right) {
      arr_[node].second += delta;
      return;
    }
    if (b < left || a > right) {
      return;
    }
    Push(node);
    Update(left, right, 2 * node + 1, a, (a + b) / 2, delta);
    Update(left, right, 2 * node + 2, ((a + b) / 2) + 1, b, delta);
    arr_[node].first = std::max(arr_[2 * node + 1].first + arr_[2 * node + 1].second,
                                arr_[2 * node + 2].first + arr_[2 * node + 2].second);
  }
  void Print() {
    for (int i = 0; i < size_tree_; i++) {
      std::cout << arr_[i].first << " " << arr_[i].second << "\n";
    }
  }
};
int main() {
  int n;
  std::cin >> n;
  int h = std::__lg(n);
  int size = (1 << (h + 1));
  std::vector<int> a(n);
  for (int i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  SegmentTree tree(a.begin(), a.end());
  int k;
  std::cin >> k;
  for (int i = 0; i < k; i++) {
    char prov;
    std::cin >> prov;
    if (prov == 'm') {
      int left, right;
      std::cin >> left >> right;
      std::cout << tree.QueryDown(left - 1, right - 1, 0, 0, size - 1) << " ";
    }
    if (prov == 'a') {
      int left, right, delta;
      std::cin >> left >> right >> delta;
      tree.Update(left - 1, right - 1, 0, 0, size - 1, delta);
    }
  }
  return 0;
}
