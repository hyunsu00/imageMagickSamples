﻿// imageMagickSample04.cpp
//
#include <stdio.h> // system
#include <string>
#ifdef _WIN32
#else
#	include <string.h>	// strdup
#	include <libgen.h>	// dirname
#endif
#include <iostream>

#include <unistd.h> // execl
#include <sys/wait.h> // wait
int main(int argc, char** argv)
{
	char* exePath = strdup(argv[0]);
	std::string exeDir = dirname(exePath);
	free(exePath);
	exeDir += "/";
	std::string imagePath = exeDir + "samples/chessboard.png";

#if 0
#if 1
	std::string command = std::string("convert -crop 449x465 ") + imagePath + std::string(" ./results/cropped_%d.png");
#else
	std::string command = std::string("convert -crop 12.5%x12.5% ") + imagePath + std::string(" ./results/cropped_%d.png");
#endif
    int ret = system(command.c_str());

	std::cout << "system(\"convert -crop 449x465 " << imagePath << " ./results/cropped_%d.png\")" << "=" << ret << std::endl;
#endif

	pid_t pid = fork();
	if (pid > 0) {
		std::cout << "[부모 프로세스 호출] : pid = " << pid << std::endl;
		wait((int*)0); // 자식 프로세스가 종료될때까지 기다린다.
		std::cout << "자식 프로세스 - execl() 종료됨" << std::endl;
	} else if (pid == 0) {
		std::cout << "[자식 프로세스 호출] : pid = " << pid << std::endl;
		int ret = execl("/usr/bin/convert", "convert", "-crop", "449x465", imagePath.c_str(), "./results/cropped_%d.png", 0);
		perror("execl() failed to run convert");
	} else {
		std::cout << "fork() 함수 실패" << std::endl;
	}

    return 0;
}
