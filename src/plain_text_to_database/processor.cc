#include "processor.h"

#include <fstream>
#include <bsoncxx/json.hpp>
#include <vector>

#include <map>

namespace PlainTextToDatabase {

mongocxx::instance instance;
size_t Processor::kBulkSize = 1;

Processor::Processor (const string connection_string, Region& record_description) : 
    uri(connection_string),
    client(uri),
    record_description(record_description) {
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

inline std::unique_ptr<document> BuildBson(Region &record_description, 
            std::istream &stream,
            document* bson = new document()) {
  const size_t begin = stream.tellg();
  for (Region& region : record_description.regions) {
    const size_t current_record_position = static_cast<size_t>(stream.tellg()) 
      - begin + 1;
    stream.ignore(region.position - current_record_position);
    if (stream.eof()) break;

    if (region.regions.size() > 0) {
      *bson << region.name << open_document;
      BuildBson(record_description, stream, bson);
      *bson << close_document;
    } else if (region.length > 0)
      *bson << region.name << Read(stream, region.length);
    else
      *bson << region.name << GetUntil(stream, region.end_delimiter);
  }
  if (!record_description.end_delimiter.empty())
     GetUntil(stream, record_description.end_delimiter);

  return stream.good() ? std::unique_ptr<document>(bson) : nullptr;
}

void Processor::Process(const string file_path, const string collection_name) {
  std::ifstream file(file_path);
  std::vector<bsoncxx::document::value> bulk;
  mongocxx::collection collection = this->database[collection_name];

  while ( DocumentPointer bson = BuildBson(this->record_description, file) ) {
    bulk.emplace_back(bson->extract());

    if (bulk.size() >= kBulkSize)
      collection.insert_many(bulk), bulk.clear();
  }
  
  if (bulk.empty() == false)
    collection.insert_many(bulk), bulk.clear();
}

ConfigurableProcessor Processor::GroupBy(string name) {
  return ConfigurableProcessor(*this, name);
}

void ConfigurableProcessor::Process(string file_path) {
  std::ifstream file(file_path);
  std::map<string, std::vector<bsoncxx::document::value>> bulks;

  while ( DocumentPointer bson = BuildBson(processor.record_description, file) ) {
    string collection_name(bson->view()[this->region_name].get_utf8().value);

    bulks[collection_name].emplace_back(bson->extract());

    if (bulks[collection_name].size() >= processor.kBulkSize)
      processor.database[collection_name].insert_many(bulks[collection_name]), bulks[collection_name].clear();
  }

  for (auto bulk : bulks)
    if (bulk.second.empty() == false)
      processor.database[bulk.first].insert_many(bulk.second);
}

} /* PlainTextToDatabase */ 

