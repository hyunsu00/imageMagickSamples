// imageMagickSample04.cpp
//

#include <iostream> // std::cout
#include <string> // std::string
#include "common.h"

#ifdef _WIN32
#include "MagickCore/studio.h"
#include "MagickWand/MagickWand.h"
int Main(int argc, char* argv[])
{
	std::string samplesDir = common::pathAddSeparator(common::getDir(argv[0]) + "samples");
	std::string resultDir = common::pathAddSeparator(common::getDir(argv[0]) + "result");

	//  magick -size 100x100 xc:red ( rose: -rotate -90 ) +append show:
	/*const char* args[] = { "-size", "100x100", "xc:red",
					 "(", "rose:", "-rotate", "-90", ")",
					 "+append", "show:", NULL 
	};*/

	// magick convert lena.jpg -resize 50 % lena.png
	/*std::string inputFile = samplesDir + "lena.jpg";
	std::string outputFile = resultDir + "lena.png";
	const char* args[] = { 
		inputFile.c_str(), "-resize", "50%%", outputFile.c_str(), NULL
	};*/

	// magick convert -crop 300x200 logo: ./result/cropped_%d.png
	std::string inputFile = samplesDir + "chessboard.png";
	std::string outputFile = resultDir + "cropped_%d.png";
	const char* args[] = { 
		inputFile.c_str(), "-crop", "449x465", outputFile.c_str(), NULL
	};

	int arg_count;
	for (arg_count = 0; args[arg_count] != (char*)NULL; arg_count++);

	MagickCoreGenesis(argv[0], MagickFalse);
	{
		MagickCLI* cli_wand = AcquireMagickCLI((ImageInfo*)NULL, (ExceptionInfo*)NULL);

		ProcessCommandOptions(cli_wand, arg_count, const_cast<char**>(args), 0);

		/* Note use of 'True' to report all exceptions - including non-fatals */
		if (CLICatchException(cli_wand, MagickTrue) != MagickFalse) {
			fprintf(stderr, "Major Error Detected\n");
		}

		cli_wand = DestroyMagickCLI(cli_wand);
	}
	MagickCoreTerminus();

	return 0;
}
#else
#include <stdio.h> // system
#include <string.h>	// strdup
#include <libgen.h>	// dirname
#include <unistd.h> // execl
#include <sys/wait.h> // wait
int Main(int argc, char* argv[])
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
	}
	else if (pid == 0) {
		std::cout << "[자식 프로세스 호출] : pid = " << pid << std::endl;
		int ret = execl("/usr/bin/convert", "convert", "-crop", "449x465", imagePath.c_str(), "./results/cropped_%d.png", 0);
		perror("execl() failed to run convert");
	}
	else {
		std::cout << "fork() 함수 실패" << std::endl;
	}

	return 0;
}
#endif

int main(int argc, char** argv)
{
	return Main(argc, argv);
}
