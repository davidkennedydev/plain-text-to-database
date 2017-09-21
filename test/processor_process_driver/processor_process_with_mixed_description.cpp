#include "plain_text_to_database/processor.h"
#include <iostream>
#include <list>
#include <string>

using std::string;

struct Person {
  string name;
  string gender;
  string city;
  string age;
};

int main(void) {
  using namespace PlainTextToDatabase;

  const string connection_string = "mongodb://test:test@localhost/testdb";
  const string collection_name = "processor_process_with_mixed_description";

  Region person {
    // name,    position,   length
    { "name",   1,          10    },    

    // name,    length
    { "gender", 1     },    

    // name,    position,   delimiter
    { "city",   14,         "@@"     },    
  
    // name,    position,   length
    { "age",    25,         2     }     
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

  processor.Process("persons.txt", person, collection_name);

  size_t elements = collection.count(document{} << finalize);
  assert(elements == 4);

  std::list<Person> expected {
    { "David     ", "m" , "Gyn", "24" },
    { "Jorge     ", "m" , "RJ", "27" },
    { "Adriano   ", "m" , "RJ", "32" },
    { "Flavio    ", "m" , "RJ", "32" }
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
