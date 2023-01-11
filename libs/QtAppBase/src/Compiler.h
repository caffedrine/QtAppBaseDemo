#ifndef _COMPILER_H_
#define _COMPILER_H_

// Inspired from spdlog core.h

#define CORE_HAS_CPP14_ATTRIBUTE(attribute)  (__cplusplus >= 201402L && __has_cpp_attribute(x))
#define CORE_HAS_CPP17_ATTRIBUTE(attribute)  (__cplusplus >= 201703L && __has_cpp_attribute(x))

#ifndef CORE_NODISCARD
#  if CORE_HAS_CPP17_ATTRIBUTE(nodiscard)
#    define CORE_NODISCARD [[nodiscard]]
#  else
#    define CORE_NODISCARD
#  endif
#endif



#endif // _COMPILER_H_
