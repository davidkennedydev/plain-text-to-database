#include "plain_text_to_database/processor.h"
using namespace PlainTextToDatabase;

#include <iostream>

int main(void) {
  Region person {
    { "name", 1, 10 },
    { "age", 2 },
  };
  person.end_delimiter = "\n";

  Processor::kBulkSize = 3;

  Processor processor("mongodb://test:test@localhost/testdb");
  processor.Process("person.txt", person, "process_mixed_description");
  
  // TODO: make assertions.
  
  return 0;
}
