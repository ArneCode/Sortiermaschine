//definitions that are used all over the programm
#ifndef HEADER_H
#define HEADER_H
using time_t = unsigned long; //defining a time type
template <typename ReturnT = void>
using func_t=ReturnT(*)(); //defining a function type
#endif