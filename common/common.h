// common.h
//
#pragma once
#ifdef _WIN32
#	include <crtdbg.h> // _ASSERTE
#	include <atlconv.h> // USES_CONVERSION, A2W
#	include <stdlib.h> // _wgetenv_s
#else
#	include <assert.h> // assert
#	define _ASSERTE assert
#	include <string.h>	// strdup
#	include <libgen.h>	// dirname
#endif
#include <string>
#include <vector>

namespace common { 
    
	inline std::string getDir(const std::string& path)
	{
        std::string dirPath;
#ifdef _WIN32	
        {
            char drive[_MAX_DRIVE] = { 0, }; // 드라이브 명
            char dir[_MAX_DIR] = { 0, }; // 디렉토리 경로
            _splitpath_s(path.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, nullptr, 0, nullptr, 0);
			dirPath = std::string(drive) + dir;
        }
#else
        {
            char* clonePath = strdup(path.c_str());
			dirPath = dirname(clonePath);
            free(clonePath);
			dirPath += "/";
        }
#endif
        return dirPath;
	}

	inline void addEnvPathW(const std::wstring& path)
	{
#ifdef _WIN32
		size_t requiredSize;
		_wgetenv_s(&requiredSize, nullptr, 0, L"PATH");
		std::vector<wchar_t> envPath(requiredSize, 0);
		_wgetenv_s(&requiredSize, &envPath[0], requiredSize, L"PATH");

		std::wstring addEnvPath = path + L";";
		addEnvPath += &envPath[0];
		_wputenv_s(L"PATH", addEnvPath.c_str());
#endif
	}

	inline void addEnvPathA(const std::string& path)
	{
#ifdef _WIN32
		USES_CONVERSION;
		addEnvPathW(A2W(path.c_str()));
#endif
	}

	inline std::string pathAddSeparator(const std::string& dirPath) 
	{
#ifdef _WIN32
		const char PATH_SEPARATOR = '\\';
#else
		const char PATH_SEPARATOR = '/';
#endif
		std::string addDirPath = dirPath;
		if (dirPath.back() != PATH_SEPARATOR) {
			addDirPath += PATH_SEPARATOR;
		}

		return addDirPath;
	}

} // common
