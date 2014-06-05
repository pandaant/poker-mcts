#ifndef SCHEMA_EXCEPTION_H
#define SCHEMA_EXCEPTION_H

#include <stdexcept>

/// comment in, if rapidjson should assert
/// instead of throwing an exception.
/// DONT WANT THIS ENABLED IN PRODUCTION(!!!)
#define ASSERT_JSON 1

namespace freedom {

// ----------------------------------------------------------------------
/// @brief   Exception for failed parsing of json documents.
// ----------------------------------------------------------------------
class rapidjson_exception : public std::runtime_error {
public:
  explicit rapidjson_exception(const char *error = "Message Parse Error")
      : std::runtime_error(error) {}
};
}

#ifndef ASSERT_JSON
#define RAPIDJSON_ASSERT(x)                                                    \
  if (x)                                                                       \
    ;                                                                          \
  else                                                                         \
    throw freedom::rapidjson_exception();

#endif
#endif
