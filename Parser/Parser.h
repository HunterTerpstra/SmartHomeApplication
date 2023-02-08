/**
 * @file Parser.h
 * @author Mohammed Al Darwish
 * @brief Parser class used to parse XML style strings 
 * @date 2022-11-27
 * Parser class that uses Boost to parse XML style strings and return a parse tree
 */

#ifndef Parser_h
#define Parser_h

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/detail/file_parser_error.hpp>
#include <iostream>

namespace pt = boost::property_tree;

/**
 * @brief Parser class used to parse XML style strings
 * 
 */
class Parser{

    public:
        /**
         * @brief Construct a new Parser object
         * 
         */
        Parser();

        /**
         * @brief Destroy the Parser object
         * 
         */
        ~Parser();

        /**
         * @brief Get the Parse Tree object
         * 
         * @param xml_str a string in XML format 
         * @return a parse tree made from the provided XML string
         */
        pt::ptree* getParseTree(std::string xml_str);

};

#endif