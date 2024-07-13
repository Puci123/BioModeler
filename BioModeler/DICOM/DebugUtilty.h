#pragma once
#include <iostream>



#define LOG(X)		   (std::cout << "[LOG] " << X << std::endl)
#define LOG_WARINIG(X) (std::cout << "\033[1;33m" << "[WARING] " << "\033[0m" << X  << "  " << __FILE__ << " line: " << __LINE__ << std::endl)
#define LOG_ERROR(X)   (std::cout << "\033[1;31m" << "[ERROR!] " << "\033[0m" << X  << "  " << __FILE__ << " line: " << __LINE__ << std::endl)

#define ASSERT(X)	   if(!(X)) __debugbreak()

						