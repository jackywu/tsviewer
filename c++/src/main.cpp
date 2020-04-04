#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <map>
#include <cxxopts.hpp>
#include <pprint/pprint.hpp>

using namespace std;


int main(int argc, char** argv)
{
    cxxopts::Options options("tsviewer", "A tsv file pretty-printer");
    options.add_options()
        ("f,file", "tsv file", cxxopts::value<string>())
        ("l,line", "line limit", cxxopts::value<int>())
        ("c,column", "column to print", cxxopts::value<string>())
        ("h,help", "print usage");

    string filepath;
    int line = 0;
    string column;
    vector<string> fields;

    try {
        auto result = options.parse(argc, argv);
        if(result.count("help")) {
            cout << options.help() << endl;
            exit(0);
        }

        // check param: file
        if (!result.count("file")) {
            cout << endl << "> file option is required" << endl << endl;
            cout << options.help() << endl;
            exit(0);
        }
        filepath = result["file"].as<string>();

        // check param: line
        if (result.count("line")) {
            try {
                line = result["line"].as<int>();
            }catch(exception& e) {
                cout << endl << "> invalid integer: " << e.what() << endl << endl;
                cout << options.help() << endl;
                exit(0);
            }
        }

        // check param: column
        if (result.count("column")) {
            try {
                column = result["column"].as<string>();
            }catch(exception& e) {
                cout << endl << "> invalid string: " << e.what() << endl << endl;
                cout << options.help() << endl;
                exit(0);
            }
            regex delimiter(",");
            std::copy(
                    sregex_token_iterator(column.begin(), column.end(), delimiter, -1),
                    sregex_token_iterator(),
                    back_inserter(fields)
                    );
        }
    }catch(exception& e) {
        cout << endl << "> parse arguments failed: " << e.what() << endl << endl;
        cout << options.help() << endl;
        exit(0);
    }


    ifstream fin(filepath);
    if (!fin) {
        cout << endl << "> file is not exist" << endl << endl;
        cout << options.help() << endl;
        exit(0);
    }

    pprint::PrettyPrinter printer;
    string row;
    regex delimiter("\t");
    map<string, string> row_map;
    map<string, string> filted_map;
    vector<string> header;
    vector<string> row_vector;

    int count = 0;
    while (getline(fin, row)) {
        count += 1;
        if (count == 1) {
            // save the header
            std::copy(
                    sregex_token_iterator(row.begin(), row.end(), delimiter, -1),
                    sregex_token_iterator(),
                    back_inserter(header)
                    );
        } else {
            std::copy(
                    sregex_token_iterator(row.begin(), row.end(), delimiter, -1),
                    sregex_token_iterator(),
                    back_inserter(row_vector)
                    );

            std::transform(
                    header.begin(), header.end(), row_vector.begin(),
                    inserter(row_map, row_map.begin()),
                    [] (string h, string e) {
                    return make_pair(h, e);
                    }
                    );

            if (!fields.empty()) {
                std::copy_if(row_map.begin(), row_map.end(),
                        std::inserter(filted_map, filted_map.begin()),
                        [&fields](std::pair<string, string> pair){
                        return std::count(fields.begin(), fields.end(), pair.first) > 0;
                        });
                printer.print(filted_map);
                filted_map.clear();
            } else {
                printer.print(row_map);
            }

            row_vector.clear();
            row_map.clear();
            if (count == line+1) {
                exit(0);
            }
        }
    }

    return 0;
}
