#include "plain_text_to_database/region.h"
#include <sstream>
using namespace PlainTextToDatabase;

#include <cassert>

int main(void) {

  Region region {
    { "age", 6, 2 },
    { "name", 1, 4 }
  };

  auto it = region.regions.begin();
  assert( it->name == "name" );
  assert( it->length == 4 );
  assert( it->ignore == false );
  ++it;
  assert( it->name.empty() );
  assert( it->length == 1 );
  assert( it->ignore );
  ++it;
  assert( it->name == "age" );
  assert( it->length == 2 );
  assert( it->ignore == false );
  
  return 0;
}
