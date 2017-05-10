#pragma once

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <string>
using std::string;

#include "region.h"

namespace PlainTextToDatabase {

class Processor {
  public:
    Processor (const string connection_string);

    // Process the text file and put data on database.
    //
    // Process the file on file_path based on record_description structure and
    // save the data on database collection collection_name.
    void Process(const string file_path, Region& record_description, 
                 const string collection_name);

    static size_t kBulkSize;

  private:
    mongocxx::uri uri;
    mongocxx::client client;
    mongocxx::database database;
};

} /* PlainTextToDatabase */ 

