/* 
 *
 * Copyright 2015 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * 
 * Timming API
 * 
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 * 
 */

#ifndef __ESAT_TIME_H__
#define __ESAT_TIME_H__

namespace esat {

// returns time in milliseconds with microsecond preccision
double Time();

// suspends the execution of the current thread. The time given is in ms.
// The time suspended is not extremely accurate be careful when calling sleep.
void Sleep(unsigned int ms);

} //esat

#endif //__ESAT_TIME_H__ 
