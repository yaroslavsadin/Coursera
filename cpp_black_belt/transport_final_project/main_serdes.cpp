#ifndef DEBUG
#include <iostream>
#include <fstream>
#include <string_view>
#include <iomanip>
#include "io.h"
#include "json.h"

using namespace std;

string ReadFileData(const string& file_name) {
  ifstream file(file_name, ios::binary | ios::ate);
  const ifstream::pos_type end_pos = file.tellg();
  file.seekg(0, ios::beg);

  string data(end_pos, '\0');
  file.read(&data[0], end_pos);
  return data;
}

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: transport_catalog_part_o [make_base|process_requests]\n";
    return 5;
  }

  const string_view mode(argv[1]);

  if (mode == "make_base") {

    Json::Document doc = Json::Load(cin);
    TransportCatalog handler(doc);
    try {  
      handler.ProcessRequests().Serialize();
    } catch(exception& e) {
      cerr << "make_base" << e.what() << endl;
      throw;
    }

  } else if (mode == "process_requests") {

    Json::Document doc = Json::Load(cin);
    TransportCatalog handler(doc);
    try {
      auto responses = handler.Deserialize().ProcessRequests().GetResponses();
      cout << setprecision(6);
      Json::Print(responses, cout);
    } catch(exception& e) {
      cerr << "process_requests" << e.what() << endl;
      throw;
    }

  }

  return 0;
}
#endif