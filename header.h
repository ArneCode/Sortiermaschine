/**
 * @file header.h
 * @brief Definiert variablen-types die überall im Programm benutzt werden
 */
#ifndef HEADER_H //include guards (https://www.geeksforgeeks.org/include-guards-in-c/)
#define HEADER_H
/**
 * @brief Ein Zeit Typ
 */
using time_t = unsigned long;
/**
 * @brief Ein Funktions Typ
 * 
 * @tparam ReturnT der Rückgabewert der Funktion die referenziert wird
 */
template <typename ReturnT = void>
using func_t=ReturnT(*)(); //defining a function type
#endif