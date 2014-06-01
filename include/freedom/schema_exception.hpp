/*
 * File:   schema_exception.h
 * Author: batman
 *
 * Created on October 31, 2013, 11:48 PM
 */

#ifndef SCHEMA_EXCEPTION_H
#define SCHEMA_EXCEPTION_H

#include <stdexcept>

/**
 * comment in, if rapidjson should assert
 * instead of throw an exception.
 * YOUT DONT WANT DIS ENABLED IN PRODUCTION(!!!)
 */
//#define ASSERT_JSON 1

namespace Freedom5 {

/**
 * Disables assertion aborts if a message is invalid.
 */
class rapidjson_exception : public std::runtime_error {
public:
  rapidjson_exception(const char *error = "Message Parse Error")
      : std::runtime_error(error) {}
};
}

#ifndef ASSERT_JSON
#define RAPIDJSON_ASSERT(x)                         \
  if(x);                                            \
  else throw Freedom5::rapidjson_exception();  

#endif	/* SCHEMA_EXCEPTION_H */
#endif
