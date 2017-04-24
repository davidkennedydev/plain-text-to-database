#include <string>
using std::string;

#include "PlainTextToDatabase.h"

int main(void) {
  const string FILE_PATH = "/files/business/stock.txt";
  const string CONNECTION_STRING = "Database=StockDB;user=AppStock;password=42ZzZ;";
  const string COLLECTION_NAME = "Stock";

  // Data on file are separeted by regions;
  // Regions are defined by position and lenght or a group of another regions.
  // Positions are relative to inner region boundary.

  // A region that describe each entry on file.
  Region record("stock.json");

  // Create a file processor, put data on database defined in connection string
  PlainTextToDatabase::Processor processor(CONNECTION_STRING);

  // Process the file with the record structure and put on collection
  processor.process(FILE_PATH, record, COLLECTION_NAME);

  return 0;
}
