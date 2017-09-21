#include "processor.h"
#include <iostream>
#include <list>
#include <string>

using std::string;

struct Person {
  string name;
  string age;
};

int main(void) {
  using namespace PlainTextToDatabase;

  const string connection_string = "mongodb://test:test@localhost/testdb";
  const string collection_name = "process_by_name_position_and_length";

  Region person {
    { "age", 11, 2 },
    { "name", 1, 10 }
  };
  person.end_delimiter = '\n';

  Processor::kBulkSize = 3;

  Processor processor(connection_string);

  using namespace mongocxx; 
  using namespace bsoncxx::builder::stream;

  uri uri(connection_string);
  client client(uri);
  database db = client["testdb"];
  collection collection = db[collection_name];

  collection.drop();

  processor.Process("person.txt", person, collection_name);

  size_t elements = collection.count(document{} << finalize);
  assert(elements == 4);

  std::list<Person> expected {
    { "David     ", "24" },
    { "Jorge     ", "27" },
    { "Adriano   ", "32" },
    { "Flavio    ", "32" }
  };

  auto personIterator = expected.begin();
  cursor result = collection.find(document{} << finalize);
  for (const bsoncxx::document::view & document : result) {
    Person& person = *personIterator;
    string value;

    value = document["name"].get_utf8().value.to_string(); 
    assert( value == person.name );
    value = document["age"].get_utf8().value.to_string(); 
    assert( value == person.age );

    ++personIterator;
  }
  
  
  return 0;
}
