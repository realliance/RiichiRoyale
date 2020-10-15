#include "handnode.h"


auto Node::begin() const -> const_iterator{
  return const_iterator(this, false);
}

auto Node::end() const -> const_iterator{
  return const_iterator(this, true);
}

auto Node::begin() -> iterator{
  return iterator(this, false);
}

auto Node::end() -> iterator{
  return iterator(this, true);
}

Node::~Node(){
  std::vector<Node*> toDelete;
  for(Node& node : *this){
    if(parent){
      toDelete.push_back(&node);
    }
  }
  for(Node* n : toDelete){
    delete n;
  }
}

auto Node::iterator::operator++() -> iterator&{
  if(!root->leaves.empty()){
    root = root->leaves.front();
    return *this;
  }
  if(!root->parent){
    end = true;
    return *this;
  }
  const Node* traveler = root;
  size_t leafPosNext = traveler->leafPosInParent+1;
  while(traveler->parent && traveler->parent->leaves.size() <= leafPosNext ){
    leafPosNext = traveler->leafPosInParent+1;
    traveler = traveler->parent;
  }
  if(!traveler->parent){
    end = true;
    return *this;
  }

  if(traveler->parent->leaves.size() > leafPosNext && traveler->parent->leaves[leafPosNext] != root){
    root = traveler->parent->leaves[leafPosNext];
    return *this;
  }
  std::cerr << "FORWARD TRAVERSAL FAILED: Set to end." << std::endl;
  std::cerr << "ROOT: " << *root << std::endl;
  if(traveler->parent){
    std::cerr << "PARENT: " << *traveler->parent << std::endl;
  }
  std::cerr << "TRAVELER: " << *traveler << std::endl;
  end = true;
  return *this;
}

auto Node::const_iterator::operator++() -> const_iterator&{
  itr.operator++();
  return *this;
}

auto Node::const_iterator::operator*() const -> const Node&{
  return *itr.root; 
}

auto Node::iterator::operator*() const -> Node&{
  return *root;
}

auto Node::const_iterator::operator!=(const const_iterator& other) const -> bool {
  return itr != other.itr;
}

auto Node::iterator::operator!=(const Node::iterator& other) const -> bool {
  if(end != other.end){
    return true;
  }
  return false;
}

auto Node::TypeToStr(uint8_t nodetype) -> std::string{
  switch(nodetype){
    case ChiSet:
      return "Chi";
    case PonSet:
      return "Pon";
    case Pair:
      return "Pair";
    case Single:
      return "Single";
    case Root:
      return "RootNode";
    default:
      return "Invalid Type";
  }
}

auto Node::DumpAsTGF(std::ostream& os) -> std::ostream&{
  std::vector<std::string> nodes;
  std::vector<std::string> connections;
  for(const auto & node : *this){
    nodes.push_back(std::to_string(node.id) + " Piece: " + (node.type != Root ? node.start.toStr() : "Root" ) + " Type: " + TypeToStr(node.type));
    for(const auto & leaf : node.leaves){
      connections.push_back(std::to_string(node.id) + " " + std::to_string(leaf->id));
    }
  }
  for(auto const & node : nodes){
    os << node << std::endl;
  }
  os << "#" << std::endl;
  for(auto const & connection : connections){
    os << connection << std::endl;
  }
  return os;
}

auto NodeTypeToColorStr(uint8_t nodetype) -> std::string{
  switch(nodetype){
    case ChiSet:
      return "purple";
    case PonSet:
      return "yellow";
    case Pair:
      return "green";
    case Single:
      return "blue";
    case Root:
      return "black";
    default:
      return "red";
  }
}

auto NodeTypeToShapeStr(uint8_t nodetype) -> std::string{
  switch(nodetype){
    case ChiSet:
      return "house";
    case PonSet:
      return "septagon";
    case Pair:
      return "oval";
    case Single:
      return "box";
    case Root:
      return "underline";
    default:
      return "Mdiamond";
  }
}

auto Node::DumpAsDot(std::ostream& os) -> std::ostream&{
  std::vector<std::string> nodes;
  std::vector<std::string> connections;
  for(const auto & node : *this){
    nodes.push_back(
      std::to_string(node.id) + " [label=\"" + TypeToStr(node.type) + ": "
      + (node.type != Root ? node.start.toStr() : "Root" ) + "\""
      + ",shape=" + NodeTypeToShapeStr(node.type)
      + ",color=" + NodeTypeToColorStr(node.type) + "];"
    );
    for(const auto & leaf : node.leaves){
      connections.push_back(std::to_string(node.id) + " -> " + std::to_string(leaf->id) + ";");
    }
  }
  os << "digraph {" << std::endl;
  for(auto const & node : nodes){
    os << "  " << node << std::endl;
  }
  os << std::endl;
  for(auto const & connection : connections){
    os << "  " << connection << std::endl;
  }
  os << "}" << std::endl;
  return os;
}

auto operator<<(std::ostream& os, const Node& node) -> std::ostream&{
    os << "{ id: " << node.id << ", type:" << Node::TypeToStr(node.type);
    os << ", start:" << node.start.toStr();
    os << ", parent: " << (node.parent ? std::to_string(node.parent->id) : "No Parent");
    os << ", leaves: [ ";
    for(const auto & leaf : node.leaves){
      os << "id: " << leaf->id << ", ";
    }
    os << " ], " << "leafPosInParent: " << node.leafPosInParent << " },";
    os << std::endl;
    return os;
}
