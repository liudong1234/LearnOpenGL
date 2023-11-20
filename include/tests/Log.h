#pragma 
#include <iostream>
#if _DEBUG
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x) ;
#endif