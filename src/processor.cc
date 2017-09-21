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

inline std::string GetUntil(std::istream &stream, std::string delimiter) {
  std::string value;
  size_t match = 0;
  do {
    value += stream.get();
    match = value.back() == delimiter[match]? match + 1 : 0;
  } while (stream.good() && match < delimiter.length());
  value.erase(value.length() - delimiter.length());

  return value;
}

inline std::string Read(std::istream &stream, const size_t length) {
  char data[length];
  stream.read(data, length);
  return std::string(data, data + length);
}

using namespace bsoncxx::builder::stream;
using DocumentPointer = std::unique_ptr<document>;

void RecursiveBuildBson(Region &region, 
    std::istream &stream,
    size_t relative_position,
    document& bson) {
  const size_t stream_position = static_cast<size_t>(stream.tellg());
  const size_t current_record_position = stream_position - relative_position;
  stream.ignore((region.position - 1) - current_record_position);

  if (region.regions.size() > 0) {
    bson << region.name << open_document;
    const size_t stream_subregion_record_position = stream.tellg();
    for (Region& sub_region : region.regions) {
      RecursiveBuildBson(sub_region, stream, stream_subregion_record_position, bson);
    }
    bson << close_document;
  } else if (region.length > 0)
    bson << region.name << Read(stream, region.length);
  else
    bson << region.name << GetUntil(stream, region.end_delimiter); 
}

std::unique_ptr<document> BuildBson(Region &region_description, std::istream &stream) {
  std::unique_ptr<document> bson( new document{} );
  const size_t stream_record_start_position = stream.tellg();
  for (Region& region : region_description.regions) {
    if (stream.eof())
      return nullptr;
    RecursiveBuildBson(region, stream, stream_record_start_position, *bson);
  }
  if (region_description.end_delimiter.empty() == false)
    GetUntil(stream, region_description.end_delimiter);
  return bson;
}

void Processor::Process(const string file_path, Region& record_description, 
                        const string collection_name) {
  std::ifstream file(file_path);
  std::vector<bsoncxx::document::value> bulk;
  mongocxx::collection collection = this->database[collection_name];

  while ( DocumentPointer bson = BuildBson(record_description, file) ) {
    bulk.emplace_back(bson->extract());

    if (bulk.size() >= kBulkSize)
      collection.insert_many(bulk), bulk.clear();
  }
  
  if (bulk.empty() == false)
    collection.insert_many(bulk), bulk.clear();
}

} /* PlainTextToDatabase */ 

