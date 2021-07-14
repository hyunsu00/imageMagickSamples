// imageMagickSample02.cpp
//
#include <wand/MagickWand.h>
#include <iostream> // std::cout
#include <vector> // std::vector
#include <chrono> // std::chrono

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

int main(int argc, char** argv)
{
	MagickWandGenesis();

	std::vector<MagickWand*> mwVector;
	{
		/* Create a wand */
		MagickWand* mw = NewMagickWand();
		
		/* Read the input image */
		MagickReadImage(mw, "logo:");

		const size_t split_width = 300;
		const size_t split_height = 200;

		std::cout << "[Begin] : CropImageToTiles()" << std::endl;
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		CropImageToTiles(mw, split_width, split_height, mwVector);
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "    Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
		std::cout << "    Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
		std::cout << "    Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 << std::endl;
		std::cout << "[End] : CropImageToTiles()" << std::endl;

		/* Tidy up */
		if (mw) {
			mw = DestroyMagickWand(mw);
		}

		std::string imagePath;
		for (int i = 0; i < static_cast<int>(mwVector.size()); i++) {
			imagePath = std::string("logo") + std::to_string(i) + ".png";
			MagickWriteImage(mwVector[i], imagePath.c_str());
		}
	
		MagickWand* mwResult = NULL;
		for (int i = 0; i < static_cast<int>(mwVector.size()); i++) {
			if (mwVector[i]) {
				mwResult = DestroyMagickWand(mwVector[i]);
			}
		}
	}
	
	MagickWandTerminus();

    return 0;
}
