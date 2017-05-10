#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <bsoncxx/json.hpp>
#include "plain_text_to_database/processor.h"
#include <cassert>
#include <string>
using std::string;
using namespace PlainTextToDatabase;
#include <iostream>

struct Registry {
  string name;
  string age;
};

string getValue(bsoncxx::document::view & document, string key) {
  return document[key].get_utf8().value.to_string();
}

int main(void) {
  Region person {
    { "name", "," },
    { "age", "\n" }
  };

  Processor::kBulkSize = 3;

  const std::string connection_string = "mongodb://test:test@localhost/testdb",
        collection_name = "processor_process_by_name_and_delimiter";

  using namespace mongocxx; 
  using namespace bsoncxx::builder::stream;

  uri uri(connection_string);
  client client(uri);
  database db = client["testdb"];
  collection collection = db[collection_name];

  collection.drop();

  Processor processor(connection_string);
  processor.Process("personCSV.txt", person, collection_name);

  cursor cursor = collection.find(document{} << finalize);
  Registry entries[4] {
    { "David    ", " 24" },
    { "Jorge    ", " 27" },
    { "Adriano  ", " 32" },
    { "Flavio   ", " 32" }
  };

  int i = 0;
  for (bsoncxx::document::view doc : cursor) {
    string name = doc["name"].get_utf8().value.to_string();
    string age = doc["age"].get_utf8().value.to_string();

    assert( entries[i].name == getValue(doc, "name") );
    assert( entries[i].age == getValue(doc, "age") );
    ++i;
  }
  
  return 0;
}
