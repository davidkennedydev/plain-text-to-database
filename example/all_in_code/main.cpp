#include <string>
using std::string;

#include "plain_text_to_database.h"

int main(void) {
  const string FILE_PATH = "/files/business/stock.txt";
  const string CONNECTION_STRING = "Database=StockDB;user=AppStock;password=42ZzZ;";
  const string COLLECTION_NAME = "Stock";

  // Data on file are separeted by regions;
  // Regions are defined by position and lenght or a group of another regions.
  // Positions are relative to inner region boundary.

  // A region that describe each entry on file.
  Region record {
      // name, start position (one-based), length
      { "id", 1, 8 },
      { "model", 9, 20 },
      { "year", 30, 4 },
      { "price", 35, 10 },
      // name, start position (one-based), a inner region
      { "last owners", 46, 

        // inner region
        {
          // name, start position (one-based), lenght
          { "name", 1, 40 },
          { "address", 41, 80 }
        }

      }
    };
  // optional line delimiter, end the region when find this delimiter.
  stockRecord.end_delimiter = "\r\n";

  // Create a file processor, put data on database defined in connection string
  PlainTextToDatabase::Processor processor(CONNECTION_STRING);

  // Process the file with the record structure and put on collection
  processor.Process(FILE_PATH, record, COLLECTION_NAME);

  return 0;
}
