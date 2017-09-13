#include <cassert>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <bsoncxx/json.hpp>
#include <iostream>
#include <bsoncxx/document/view.hpp>
#include "plain_text_to_database/processor.h"
using namespace PlainTextToDatabase;

#include <iostream>
#include <string>
using std::string;

string getValue(const bsoncxx::document::view & document, const string &&key) {
  return document[key].get_utf8().value.to_string();
}

struct Person {
  string name, gender, city, age;
};

int main(void) {
  // Define the file layout.
  Region person {
    // name,    position,   delimiter
    { "city",   14,         "@@" }, // consider a gap of 2 positions
    // name,    position,   length
    { "name",   1,          10   }, // positions one-based (file start by one)
    // name,    length
    { "gender", 1     },            // continue reading based on last position
    // name,    position,   length
    { "age",    25,         2    }  // consider a variable gap
  };
  person.end_delimiter = "\n"; // define delimiter for each record

  // define the bulk size
  Processor::kBulkSize = 3;

  const std::string connection_string = "mongodb://test:test@localhost/testdb";
  std::string collection_name = "process_mixed_description";

  // create a processor
  Processor processor(connection_string, person);

  using namespace mongocxx; 
  using namespace bsoncxx::builder::stream;

  uri uri(connection_string);
  client client(uri);
  database db = client["testdb"];
  collection collection = db[collection_name];

  collection.drop();

  std::cout << "Testing read empty file...";
  processor.Process("empty.txt", collection_name);

  // Get all documents on collection.
  cursor result = collection.find(document{} << finalize);
  size_t result_size = std::distance(result.begin(), result.end());

  assert( result_size == 0 );
  std::cout << "ok." << std::endl;

  std::cout << "Test read a file with many field format definitions..." << std::endl;
  
  processor.Process("persons.txt", collection_name);

  // Expected entries on file.
  Person entries[] {
    {"David     ", "m", "Gyn", "24" },
    {"Jorge     ", "m", "RJ" , "27" },
    {"Adriano   ", "m", "RJ" , "32" },
    {"Flavio    ", "m", "RJ" , "32" }
  };
  size_t entries_size = (sizeof entries / sizeof(Person));

  // Get all documents on collection.
  result = collection.find(document{} << finalize);
  //result_size = std::distance(result.begin(), result.end());

  //assert(entries_size == result_size);

  size_t i = 0;
  for (const bsoncxx::document::view & document : result) {
    std::cout << "\tDocument: " << bsoncxx::to_json(document) << std::endl;
    std::cout << "\t\tRead fields with fixed position and length...";
    assert( entries[i].name == getValue(document, "name") );
    std::cout << "ok." << std::endl;
    std::cout << "\t\tRead fields defined by length...";
    assert( entries[i].age == getValue(document, "age") );
    std::cout << "ok." << std::endl;
    std::cout << "\t\tRead fields defined by position and delimiter...";
    assert( entries[i].gender == getValue(document, "gender") );
    std::cout << "ok." << std::endl;
    std::cout << "\t\tRead fields defined by position after unfixed position field...";
    assert( entries[i].city == getValue(document, "city") );
    std::cout << "ok." << std::endl << std::endl;
    ++i;
  }
  std::cout << "done." << std::endl;
  
  return 0;
}
