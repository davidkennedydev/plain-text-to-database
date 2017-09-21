#include "include/processor.h"
#include "src/processor.cc"
#include <sstream>
using namespace PlainTextToDatabase;

#include <cassert>

int main(void) {

  Region region {
    { "name", 1, 4 },
    { "age", 5, 2 }
  };

  std::stringstream data { "Jon 30" };

  auto bson = ::PlainTextToDatabase::BuildBson(region, data)->extract();
  
  std::string json = bsoncxx::to_json(bson.view());
  assert(json == "{ \"name\" : \"Jon \", \"age\" : \"30\" }");
  
  return 0;
}
