#include "plain_text_to_database/processor.h"
using namespace PlainTextToDatabase;

#include <iostream>

int main(void) {
  Region person {
    { "name", 10 },
    { "age", 2 }
  };
  person.end_delimiter = '\n';

  Processor::kBulkSize = 3;

  Processor processor("mongodb://test:test@localhost/testdb");
  processor.Process("person.txt", person,
      "processor_process_by_name_and_length");

  // TODO: make assertions.
  
  return 0;
}
