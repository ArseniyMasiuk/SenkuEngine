
#ifdef _WIN32
	#ifdef _WIN64
		#define PLATFORM_WINDOWS
	#else
		#error	"x86 Builds are not supported!"
	#endif // _WIN64

#endif // _WIN32
// here can be added other defines to pick platform you want to use but for now it is only avaliable x64 windows
