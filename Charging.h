//Util items for Charging namespace
#pragma once
#ifndef CHARGING_H_INCLUDED
#define CHARGING_H_INCLUDED

#include <iostream>

//release build defines NDEBUG so we define _DEBUG if not
#ifndef NDEBUG
#define _DEBUG
#endif

//macro for sending debug output to cout if debug build or not if it's a release build
#ifdef _DEBUG
#define Debug(x) std::cout << x
#else
#define Debug(x)
#endif

#endif // CHARGING_H_INCLUDED
