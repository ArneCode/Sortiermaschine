/**
 * @file header.h
 * @brief definiert variablen-types die überall im Programm benutzt werden
 * @version 0.1
 * @date 2022-05-26
 */
#ifndef HEADER_H
#define HEADER_H
/**
 * @brief Ein Zeit typ
 */
using time_t = unsigned long;
/**
 * @brief ein Funktions-typ
 * 
 * @tparam ReturnT der Rückgabewert der Funktion die referenziert wird
 */
template <typename ReturnT = void>
using func_t=ReturnT(*)(); //defining a function type
#endif