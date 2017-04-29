#include "processor.h"

#include <fstream>
#include <bsoncxx/json.hpp>
#include <vector>

namespace PlainTextToDatabase {

mongocxx::instance instance;
size_t Processor::kBulkSize = 1;

Processor::Processor (const string connection_string) : 
    uri(connection_string),
    client(uri) {
  database = client[uri.database()];
}

using BuilderDocument = bsoncxx::builder::stream::document;
using Chars = const char * const;
inline std::unique_ptr<BuilderDocument> BuildBson(Region &record_description, 
            Chars raw_data, size_t position = 0,
            BuilderDocument* document = new BuilderDocument()) {
  for (Region region : record_description.regions) {
    if (region.regions.size() == 0) {
      Chars begin = raw_data + position;
      Chars end = begin + region.length;
      position += region.length;
      *document << region.name << std::string(begin, end);
    } else {
      *document << region.name << bsoncxx::builder::stream::open_document;
      BuildBson(record_description, raw_data, position, document);
      *document << bsoncxx::builder::stream::close_document;
    }
  }

  return std::unique_ptr<BuilderDocument>(document);
}

void Processor::Process(const string file_path, Region& record_description, 
                        const string collection_name) {
  const size_t record_size = record_description.CalculateLength();

  std::ifstream file(file_path);
  char raw_entry[record_size];
  std::vector<bsoncxx::document::value> bulk;
  mongocxx::collection collection = this->database[collection_name];

  while (file.read(raw_entry, record_size + 1)) { // TODO ignore line delimiters
    bulk.emplace_back(
      BuildBson(record_description, raw_entry)->extract()
    );

    if (bulk.size() >= kBulkSize)
      collection.insert_many(bulk), bulk.clear();
  }
  collection.insert_many(bulk), bulk.clear();
}

} /* PlainTextToDatabase */ 

