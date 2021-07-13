// imageMagickSample01.cpp
//
#include <magick/MagickCore.h>
#include <iostream> // std::cout
#include <vector> // std::vector
#include <chrono> // std::chrono
#include <string.h> // strcpy

/*
int CropImageToTiles(MagickWand* mw, const int cropWidth, const int cropHeight, std::vector<MagickWand*>& mwVector)
{
	int imgWidth = static_cast<int>(MagickGetImageWidth(mw));
	int imgHeight = static_cast<int>(MagickGetImageHeight(mw));

	int yPos = 0;
	while (yPos < imgHeight) {
		int bhSize = ((yPos + cropHeight) > imgHeight) * (cropHeight - (yPos + cropHeight - imgHeight)) + ((yPos + cropHeight) <= imgHeight) * cropHeight;

		int xPos = 0;
		while (xPos < imgWidth) {
			int bwSize = ((xPos + cropWidth) > imgWidth) * (cropWidth - (xPos + cropWidth - imgWidth)) + ((xPos + cropWidth) <= imgWidth) * cropWidth;

			MagickWand* _mw = CloneMagickWand(mw);
			MagickCropImage(_mw, bwSize, bhSize, xPos, yPos);
			mwVector.push_back(_mw);

			xPos = xPos + cropWidth;
		}

		yPos = yPos + cropHeight;
	}

	return 0;
}
*/

int main(int argc, char** argv)
{
	MagickCoreGenesis(*argv, MagickTrue);
	ExceptionInfo* exception = AcquireExceptionInfo();
	GetExceptionInfo(exception);

	{
		ImageInfo* read_info = CloneImageInfo((ImageInfo *) NULL);
		CopyMagickString(read_info->filename, "logo:", MaxTextExtent);

		Image* image  = ReadImage(read_info, exception);
		Image* imagew = CloneImage(image, 640, 480, MagickTrue, exception);
		//ImageInfo* write_info = CloneImageInfo(read_info);
		ImageInfo* write_info = CloneImageInfo(NULL);
		CopyMagickString(write_info->filename, "/github/hyunsu00/imageMagickSamples/imageMagickSample03/build/samples/123.jpg", MaxTextExtent);
		//CopyMagickString(write_info->magick, "png", MaxTextExtent);
		write_info->adjoin = MagickTrue;
		MagickBooleanType result = WriteImage(write_info, imagew);

		image = DestroyImage(image);
		//imagew = DestroyImage(imagew);
		read_info = DestroyImageInfo(read_info);
		write_info = DestroyImageInfo(write_info);
	}

	exception = DestroyExceptionInfo(exception);
	MagickCoreTerminus();

    return 0;
}
