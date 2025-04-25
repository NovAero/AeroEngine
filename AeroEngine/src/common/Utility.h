#pragma once

#ifdef ASSERT
#undef ASSERT
#endif // ASSERT

#ifdef RELEASE

#define ASSERT( isTrue, ... ) (void)(isTrue)
#define ASSERT_SUCCEEDED( hr, ... ) (void)(hr)

#else //!RELEASE
#define ASSERT(isFalse, ...) \
		if(!(bool)(isFalse)) { \
			__debugbreak(); \
		}
#define ASSERT_SUCCEEDED( hr, ... ) \
        if (FAILED(hr)) { \
            __debugbreak(); \
        }

#endif // RELEASE
