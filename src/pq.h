#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define PQ_EXPORT __declspec(dllexport)
#else
  #define PQ_EXPORT
#endif

PQ_EXPORT void pq();
PQ_EXPORT void pq_print_vector(const std::vector<std::string> &strings);
