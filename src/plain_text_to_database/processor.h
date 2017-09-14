#pragma once

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <string>
using std::string;

#include "region.h"

namespace PlainTextToDatabase {

class ConfigurableProcessor;

class Processor {
  friend class ConfigurableProcessor;
  public:

    Processor(const string connection_string, Region& record_description);

    // Process the text file and put data on database.
    //
    // Process the file on file_path based on record_description structure and
    // save the data on database collection collection_name.
    void Process(const string file_path, 
                 const string collection_name);

    // Configure extraction grouping entries by region name
    //
    // The default collection name is the value on grouped subregion
    ConfigurableProcessor GroupBy(string name);

    static size_t kBulkSize;

  private:
    mongocxx::uri uri;
    mongocxx::client client;
    mongocxx::database database;

    Region& record_description;
};

class ConfigurableProcessor {
  public:
    ConfigurableProcessor(const Processor& processor,
                          const string region_name) :
      processor(processor),
      region_name(region_name) {}

    // Process file based on previous configuration
    void Process(string file_path);

  private:
    const Processor& processor;
    string region_name;
};

} /* PlainTextToDatabase */ 

