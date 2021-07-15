// imageMagickSample03.cpp
//
#ifdef _WIN32
#include <MagickCore/MagickCore.h>
#else
#include <magick/MagickCore.h>
#endif
#include <string> // std::to_string
#include <iostream> // std::cout
#include <vector> // std::vector
#include <chrono> // std::chrono
#include <string.h> // strcpy


int CropImageToTiles(const Image* image, const int cropWidth, const int cropHeight, std::vector<Image*>& imageVector)
{
	ExceptionInfo* exception = AcquireExceptionInfo();

	int imgWidth = static_cast<int>(image->columns);
	int imgHeight = static_cast<int>(image->rows);

	int yPos = 0;
	while (yPos < imgHeight) {
		int bhSize = ((yPos + cropHeight) > imgHeight) * (cropHeight - (yPos + cropHeight - imgHeight)) + ((yPos + cropHeight) <= imgHeight) * cropHeight;

		int xPos = 0;
		while (xPos < imgWidth) {
			int bwSize = ((xPos + cropWidth) > imgWidth) * (cropWidth - (xPos + cropWidth - imgWidth)) + ((xPos + cropWidth) <= imgWidth) * cropWidth;

#if 0
			RectangleInfo rectInfo = { static_cast<size_t>(bwSize), static_cast<size_t>(bhSize), static_cast<ssize_t>(xPos), static_cast<ssize_t>(yPos) };
			Image* _image = CropImage(image, &rectInfo, exception);
#else
			// The geometry string (e.g. "100x100+10+10")
			std::string crop_geometry = std::to_string(bwSize) + "x" + std::to_string(bhSize) + "+" + std::to_string(xPos) + "+" + std::to_string(yPos);
			Image* _image = CropImageToTiles(image, crop_geometry.c_str(), exception);
#endif
			imageVector.push_back(_image);

			xPos = xPos + cropWidth;
		}

		yPos = yPos + cropHeight;
	}

	exception = DestroyExceptionInfo(exception);

	return 0;
}

int main(int argc, char** argv)
{
	// MagickError(exception->severity,exception->reason,exception->description);
	// if (exception->severity != UndefinedException) CatchException(exception);
	//
/*
#define ThrowImageException(image) \
{ \
 \
  CatchException(exception); \
  if (contrast_image != (Image *) NULL) \
    contrast_image=DestroyImage(contrast_image); \
  exit(-1); \
}
*/
	MagickCoreGenesis(*argv, MagickTrue);
	ExceptionInfo* exception = AcquireExceptionInfo();
#ifndef _WIN32
	GetExceptionInfo(exception);
#endif

	std::vector<Image*> imageVector;
	{
		ImageInfo* readImageInfo = AcquireImageInfo();
		CopyMagickString(readImageInfo->filename, "logo:", MaxTextExtent);
		Image* readImage = ReadImage(readImageInfo, exception);
		if (exception->severity != UndefinedException) {
			CatchException(exception);
		}

		const size_t split_width = 300;
		const size_t split_height = 200;

		std::cout << "[Begin] : CropImageToTiles()" << std::endl;
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		CropImageToTiles(readImage, split_width, split_height, imageVector);
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "    Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
		std::cout << "    Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
		std::cout << "    Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 << std::endl;
		std::cout << "[End] : CropImageToTiles()" << std::endl;

		ImageInfo* writeImageInfo = CloneImageInfo(readImageInfo);
		CopyMagickString(writeImageInfo->magick, "PNG", MaxTextExtent);
		// MagickWriteImage does this so I do it too.
		// writeImageInfo->adjoin = MagickTrue;
		MagickBooleanType result = MagickFalse;
		std::string imagePath;
		for (int i = 0; i < static_cast<int>(imageVector.size()); i++) {
			imagePath = std::string("logo") + std::to_string(i) + ".png";
			FILE* file = nullptr;
#ifdef _WIN32
			fopen_s(&file, imagePath.c_str(), "w+");
#else
			file = fopen(imagePath.c_str(), "w+");
#endif
			//CopyMagickString(writeImageInfo->filename, imagePath.c_str(), MaxTextExtent);
			writeImageInfo->file = file;
			result = WriteImage(writeImageInfo, imageVector[i], exception);
			fclose(file);
		}

		Image* imageReult = NULL;
		for (int i = 0; i < static_cast<int>(imageVector.size()); i++) {
			if (imageVector[i]) {
				imageReult = DestroyImage(imageVector[i]);
			}
		}

		writeImageInfo = DestroyImageInfo(writeImageInfo);
		readImageInfo = DestroyImageInfo(readImageInfo);
		readImage = DestroyImage(readImage);
	}

	exception = DestroyExceptionInfo(exception);
	MagickCoreTerminus();

    return 0;
}
