#pragma once
#include <vector>

class Combinations {
  int n;
  int k;
  class const_iterator {
    int n;
    int k;
    std::vector<bool> selection;
    bool end;
    friend Combinations;
    explicit const_iterator(int n, int k, bool end):n(n),k(k),end(end){
      selection.resize(n);
      std::fill(selection.begin(), selection.begin() + k, true);
    }
    public:
      auto operator++() -> const_iterator& {
        end = !std::prev_permutation(selection.begin(), selection.end());
        return *this;
      }
      inline auto operator==(const const_iterator& other) const -> bool{
        return end == other.end;
      }
      inline auto operator!=(const const_iterator& other) const -> bool{
        return end != other.end;
      }
      auto operator*() const -> std::vector<int>{
        std::vector<int> indx;
        for (int i = 0; i < n; ++i) {
          if (selection[i]) {
            indx.emplace_back(i);
          }
        }
        return indx;
      };
    using difference_type = int;
    using iterator_category = std::input_iterator_tag;
  };
  public:
    Combinations(int n, int k):n(n),k(k){}
    auto begin() const noexcept -> const_iterator {
      return const_iterator(n,k,false);
    }
    auto end() const noexcept -> const_iterator {
      return const_iterator(n,k,true);
    }
};
