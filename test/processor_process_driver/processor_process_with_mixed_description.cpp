#include "plain_text_to_database/processor.h"
using namespace PlainTextToDatabase;

#include <iostream>

int main(void) {
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
  person.end_delimiter = "\n"; // define delimiter for each record

  Processor::kBulkSize = 3;

  Processor processor("mongodb://test:test@localhost/testdb");
  processor.Process("person2.txt", person, "process_mixed_description");
  
  // TODO: make assertions.
  
  return 0;
}
