#include "../src/plain_text_to_database/region.h"
#include "../src/plain_text_to_database/processor.cc"
using namespace PlainTextToDatabase;

#include <cassert>

int main(void) {

  Region region {
    { "name", 0, 4 },
    { "age", 5, 2 }
  };

  const char* data = "Jon 30";

  auto bson = ::PlainTextToDatabase::BuildBson(region, data)->extract();
  
  std::string json = bsoncxx::to_json(bson.view());
  assert(json == "{ \"name\" : \"Jon \", \"age\" : \"30\" }");
  
  return 0;
}