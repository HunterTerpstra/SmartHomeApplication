/**
 * @file Parser.cpp
 * @author Mohammed Al Darwish
 * @brief Parser class used to parse XML style strings 
 * @date 2022-11-27
 * Parser class that uses Boost to parse XML style strings and return a parse tree
 */

#include "Parser.h"

Parser::Parser() = default;

Parser::~Parser() = default;

pt::ptree* Parser::getParseTree(std::string xml_str){
    
    // create a temporary tree
    pt::ptree* tmp_tree = new pt::ptree();

    try {
        std::stringstream ss; 
        ss << xml_str;
        read_xml(ss, *tmp_tree); // turn XML string into tree
    } catch (pt::xml_parser_error &e) {
        std::cout << "Failed to parse the xml string." << e.what();
    } catch (...) {
        std::cout << "Failed !!!";
    }

    return tmp_tree; // return compiled tree
} // end of get pt

// int main(){
//     std::string xml_str0 = "<?xml version = \"1.0\" encoding = \"utf-8\"?>\
//     <tv>\
//         <series>\
//             <name>Breaking Bad</name>\
//             <genre>crime</genre>\
//             <year>2008-2013</year>\
//             <creator>Vince Gilligan</creator>\
//             <imdb>9.4</imdb>\
//             <stream>Netflix</stream>\
//         </series>\
//         <series>\
//             <name>Game Of Thrones</name>\
//             <genre>Adventure</genre>\
//             <year>2011-2019</year>\
//             <creator>David Benioff & D.B. Weiss</creator>\
//             <imdb>9.4</imdb>\
//             <stream>Netflix</stream>\
//        </series>\
//     </tv>";

//     std::string xml_str2 = "<?xml version=\"1.0\"?>\
//     <Node>\
//         <name>Peter</name>\
//         <threshold>1.0</threshold>\
//         <input>\
//             <name>node_name_one</name>\
//             <weight>0.5</weight>\
//         </input>\
//         <input>\
//             <name>node_name_two</name>\
//             <weight>0.5</weight>\
//         </input>\
//         <output>\
//             <name>node_name_three</name>\
//             <weight>0.5</weight>\
//         </output>\
//         <output>\
//             <name>node_name_four</name>\
//             <weight>0.5</weight>\
//         </output>\
//     </Node>";

//     std::string xml_str3 = "<?xml version=\"1.0\"?><Node><name>Peter</name><threshold>1.0</threshold><input><name>node_name_one</name><weight>0.5</weight></input><input><name>node_name_two</name><weight>0.5</weight></input><output><name>node_name_three</name><weight>0.5</weight></output><output><name>node_name_four</name><weight>0.5</weight></output></Node>";

//     // Create an empty property tree object
//     pt::ptree* tree = getParseTree(xml_str2);

//     // get_child is used to find the node in xml for iterating over its children.
//     // Note: get_child throws if the path cannot be resolved. 
//     for (auto& p : tree->get_child("Node")) {
//         std::cout << "[" << p.first << "]" << ": "; // first is is used for the <Tag>
//         std::cout << "[" << p.second.data() << "]" << std::endl; // second is used for the <>value<>
//         for (auto& c : p.second) {
//             std::cout << "Tag: [" << c.first.data() << "], ";
//             std::cout << "Value: [" << c.second.data() << "]" << std::endl;
//         }   
//     }

//     return 0;
// } // end of main

// end of file