#pragma once

#ifdef _DEBUG
#define ASSERT(x, errorMSG) if(!(x) ) \
					  {  \
						std::cerr << "ERROR: " <<  errorMSG << std::endl; \
						exit(EXIT_FAILURE); }
#else
#define ASSERT(x, errorMSG)
#endif // _DEBUG