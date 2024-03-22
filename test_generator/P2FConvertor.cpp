#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

#include "universal/include/universal/posit/posit.hpp"
#include "json.hpp"

using json_t = nlohmann::json;


void print_helper() {
    std::cout << "Usage: P2FConvertor <json> <field> <dimension>\n"
                 "Converts a JSON file with posits to floating point numbers\n";
}

json_t parse_data(int argc, char** argv) {
    if (argc != 4) {
        print_helper();
        exit(2);
    }
    std::ifstream file(argv[1]);
    json_t j;
    try {
        file >> j;
    } catch (nlohmann::detail::parse_error err) {
        std::cerr << err.what() << std::endl;
        exit(2);
    }
    return j;
}

template<class T>struct tag{using type=T;};
template<class Tag>using type=typename Tag::type;

template<class T, size_t n>
    struct n_dim_vec:tag< std::vector< type< n_dim_vec<T, n-1> > > > {};
  template<class T>
    struct n_dim_vec<T, 0>:tag<T>{};
  template<class T, size_t n>
    using n_dim_vec_t = type<n_dim_vec<T,n>>;

void convert_path_1d(json_t json, std::string field) {
    try {
        auto data = json["memories"][field].get<n_dim_vec_t<unsigned long, 1>>();
        std::vector<double> data_float(data.size());
        std::transform(
            data.cbegin(),
            data.cend(),
            data_float.begin(),
            [](unsigned long v) -> double {
                sw::unum::posit <32, 2> p;
                p.set_raw_bits(v);
                return (double) p;
        });
        json_t transformed;
        transformed = json;
        transformed["memories"][field] = data_float;
        std::cout << transformed.dump(2);
    }  catch(nlohmann::json::type_error err) {
        std::cerr << "[Error] Expected `" << field << ".data' field with type float[]" << std::endl;
        exit(2);
    }
}

void convert_path_2d(json_t json, std::string field) {
    try {
        auto data = json["memories"][field].get<n_dim_vec_t<unsigned long, 2>>();
        std::vector<std::vector<double>> data_float(data.size(), std::vector<double>(data[0].size(), 0));
        for( int i = 0; i < data.size() ; i++) {
            std::transform(
                data[i].cbegin(),
                data[i].cend(),
                data_float[i].begin(),
                [](unsigned long v) -> double {
                    sw::unum::posit <32, 2> p;
                    p.set_raw_bits(v);
                    return (double) p;
            });
        }
        json_t transformed;
        transformed = json;
        transformed["memories"][field] = data_float;
        std::cout << transformed.dump(2);
    }  catch(nlohmann::json::type_error err) {
        std::cerr << "[Error] Expected `" << field << ".data' field with type float[]" << std::endl;
        exit(2);
    }
}

int main(int argc, char *argv[]) {
    auto j = parse_data(argc, argv);
    if(strcmp(argv[3], "1") == 0) {
        convert_path_1d(j, argv[2]);
    }
    else if(strcmp(argv[3], "2") == 0) {
        convert_path_2d(j, argv[2]);
    }
    else {
        print_helper();
        exit(2);
    }
}