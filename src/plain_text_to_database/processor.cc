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

using namespace bsoncxx::builder::stream;

inline std::unique_ptr<document> BuildBson(Region &record_description, 
            std::istream &stream,
            document* bson = new document()) {
  size_t begin = stream.tellg();
  for (Region region : record_description.regions) {
    if (region.position > 0)
      stream.seekg(begin + region.position - 1);

    if (region.regions.size() > 0) {
      *bson << region.name << open_document;
      BuildBson(record_description, stream, bson);
      *bson << close_document;
    } else if (region.length > 0) {
      std::string value;
      stream.width(region.length), stream >> value;
      *bson << region.name << value;
    }
  }
  stream.seekg(begin + record_description.length);

  return stream.good() ? std::unique_ptr<document>(bson) : nullptr;
}

void Processor::Process(const string file_path, Region& record_description, 
                        const string collection_name) {
  std::ifstream file(file_path);
  std::vector<bsoncxx::document::value> bulk;
  mongocxx::collection collection = this->database[collection_name];

  record_description.CalculateLength();

  while ( auto bson = BuildBson(record_description, file) ) {
    bulk.emplace_back(bson->extract());

    if (bulk.size() >= kBulkSize)
      collection.insert_many(bulk), bulk.clear();
  }
  
  if (bulk.empty() == false)
    collection.insert_many(bulk), bulk.clear();
}

} /* PlainTextToDatabase */ 

