#pragma once

#include <initializer_list>
#include <vector>
#include <algorithm>
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
  // where field name and delimiter are defined.
  Region (string name, string delimiter) : 
    name(name), position(0), length(0), end_delimiter(delimiter) {}

  // Construct simple region 
  // where field name and delimiter are defined.
  Region (string name, size_t position, string delimiter) : 
    name(name), position(position), length(0), end_delimiter(delimiter) {}

  // Construct a root region.
  // This region haven't a name and by assumption starts at begin.
  Region (const std::initializer_list<Region> regions, string delimiter = "") : 
    Region("", regions, delimiter) {}

  // Construct a root region.
  // This region haven't a name and by assumption starts at begin.
  Region (string name, const std::initializer_list<Region> regions, string delimiter = "") : 
    Region(name, 0, regions, delimiter) {}

  // Construct a region defined by subregions.
  // This region have subregions that define his structure.
  Region (string name, unsigned position, const std::initializer_list<Region> regions, string delimiter) : 
    name(name), position(position), regions(regions) {
    this->CalculateFixedLength();
    this->CalculatePositions();
    std::sort(this->regions.begin(), this->regions.end());
    this->end_delimiter = delimiter;
  }

  static const size_t kUndefined = 0;

  void CalculatePositions() {
    if (this->regions.front().position == kUndefined)
      this->regions.front().position = 1;
    for (std::vector<Region>::iterator it = this->regions.begin() + 1; 
        it != this->regions.end(); ++it) {
      if (it->position == kUndefined)
        it->position = (it - 1)->position + (it - 1)->length;
    }
  }

  void CalculateFixedLength() {
    if (this->length > kUndefined) return;
    for (Region& sub_region : this->regions) {
      sub_region.CalculateFixedLength();
      if (sub_region.length == kUndefined) {
        this->length = kUndefined;
        break;
      }
      this->length += sub_region.length;
    }
  }

  string name;
  size_t position, length = 0;

  std::vector<Region> regions;

  string end_delimiter;

  bool operator<(Region region) const {
    return this->position < region.position;
  }

};

} /* PlainTextToDatabase */ 
