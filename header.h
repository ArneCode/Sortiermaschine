/**
 * @file header.h
 * @brief Definiert variablen-types die überall im Programm benutzt werden
 */
#ifndef HEADER_H
#define HEADER_H
/**
 * @brief Ein Zeit typ
 */
using time_t = unsigned long;
/**
 * @brief Ein Funktions-typ
 * 
 * @tparam ReturnT der Rückgabewert der Funktion die referenziert wird
 */
template <typename ReturnT = void>
using func_t=ReturnT(*)(); //defining a function type
#endif