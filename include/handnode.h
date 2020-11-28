#pragma once
#include <stddef.h>     // for size_t
#include <stdint.h>     // for uint8_t
#include <iostream>     // for ostream, ptrdiff_t
#include <iterator>     // for forward_iterator_tag
#include <string>       // for string
#include <vector>       // for vector

#include "piecetype.h"  // for Piece

namespace Mahjong {

  class Node {
  public:
    enum Type {
      Error,
      ChiSet,
      PonSet,
      Pair,
      Single,
      Root
    };
    ~Node();
    int id;
    Type type;
    Piece start;
    Node* parent;
    std::vector<Node*> leaves;
    size_t leafPosInParent;
    class iterator {
      Node* root;
      bool end;
      friend Node;
      explicit iterator(Node* root, bool end): root(root),end(end){}
    public:
      auto operator++() -> iterator&;
      auto operator*() const -> Node&;
      auto operator!=(const iterator& other) const -> bool;
      // iterator traits
      using difference_type = std::ptrdiff_t;
      using pointer = Node*;
      using reference = Node&;
      using iterator_category = std::forward_iterator_tag;
    };
    class const_iterator {
      iterator itr;
      friend Node;
      explicit const_iterator(const Node* root, bool end): itr(const_cast<Node*>(root),end){}
    public:
      auto operator++() -> const_iterator&;
      auto operator*() const -> const Node&;
      auto operator!=(const const_iterator& other) const -> bool;
      // iterator traits
      using difference_type = std::ptrdiff_t;
      using pointer = const Node*;
      using reference = const Node&;
      using iterator_category = std::forward_iterator_tag;
    };
    static auto TypeToStr(uint8_t nodetype) -> std::string;
    auto DumpAsTGF(std::ostream& os) const -> std::ostream&;
    auto DumpAsDot(std::ostream& os) const -> std::ostream&;
    auto AsBranchVectors() const -> std::vector<std::vector<const Node*>>;
    auto IsComplete() const -> bool;
    auto begin() const -> const_iterator;
    auto end() const -> const_iterator;
    auto begin() -> iterator;
    auto end() -> iterator;
    inline auto operator==(const Node rhs) const -> bool{
      return type == rhs.type && start == rhs.start;
    }
  };

};

auto operator<<(std::ostream& os, const Mahjong::Node& node) -> std::ostream&;
