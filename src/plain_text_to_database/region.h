#pragma once

#include <initializer_list>
using std::initializer_list;

#include <list>
using std::list;

#include <string>
using std::string;

namespace PlainTextToDatabase {

// Represent a region on positional file.
struct Region {
  // Construct simple region 
  // where field name, start position and length are defined.
  Region (string name, size_t position, size_t length) : 
    name(name), position(position), length(length) {}

  // Construct simple region 
  // where field name and length are defined.
  Region (string name, size_t length) : 
    name(name), position(0), length(length) {}

  // Construct simple region 
  // where field name and length are defined.
  Region (string name, string delimiter) : 
    name(name), position(0), length(0), end_delimiter(delimiter) {}

  // Construct a root region.
  // This region haven't a name and by assumption starts at begin.
  Region (std::initializer_list<Region> regions) : 
    name(""), position(0), regions(regions) {}

  // Construct a region defined by subregions.
  // This region have subregions that define his structure.
  Region (string name, unsigned position, initializer_list<Region> regions) : 
    name(name), position(position), regions(regions) {}

  bool CalculateLength() {
    if (this->length > 0) return true;

    this->length = this->end_delimiter.length();
    for (Region sub_region : this->regions) {
      if (sub_region.CalculateLength() == false)
        return false;
      this->length += sub_region.length;
    }
    return true;
  }

  string name;
  size_t position, length = 0;

  list<Region> regions;

  string end_delimiter;
};

} /* PlainTextToDatabase */ 
