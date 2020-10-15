#pragma once
#include <vector>
#include <iostream>

#include "piecetype.h"

enum NodeType {
  ChiSet,
  PonSet,
  Pair,
  Single,
  Root
};

class Node {
public:
  ~Node();
  int id;
  NodeType type;
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
  auto DumpAsTGF(std::ostream& os) -> std::ostream&;
  auto DumpAsDot(std::ostream& os) -> std::ostream&;
  auto begin() const -> const_iterator;
  auto end() const -> const_iterator;
  auto begin() -> iterator;
  auto end() -> iterator;
};

auto operator<<(std::ostream& os, const Node& node) -> std::ostream&;
