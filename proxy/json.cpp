#include "json.h"

#include "utils/debug.h"

#include <boost/property_tree/json_parser.hpp>

typedef unsigned int t_count;

using boost::property_tree::ptree;

void t_json_debug::operator ()(std::iostream & stream, ptree & pt) {
  boost::property_tree::write_json(stream, pt);
}

void json_copy_object(std::istream & in, std::ostream & out) {
  bool in_str = false;
  bool escape = false;
  t_count stack_size = 0;

  char c;
  while(in >> c) {

    if(escape) {
      escape = false;
    }
    else if(c == '"') {
      in_str = !in_str;
    }
    else if(in_str){
      escape = c == '\\';
    }
    else if(c == '[' || c == '{'){
      stack_size++;
    }
    else if(c == ']' || c == '}'){
      stack_size--;
      if(!stack_size) {
        out << c;
        break;
      }
    }
    
    if(stack_size && (in_str || isprint(c)))
      out << c;
  }
}

t_json_parser_service::t_json_parser_service(const t_json_service::t_ptr & srv):srv(srv) {
}

void t_json_parser_service::operator()(std::iostream & stream) {

  while(stream) {
    boost::property_tree::ptree pt;
    debug("Parsing Start");
    std::stringstream object;
    json_copy_object(stream, object);
    std::cerr << object.str();
    boost::property_tree::read_json(object, pt);
    debug("Parsing Complete");
    (*srv)(stream, pt);
  }
  debug("Ended Connection");
}
