// imageMagickSample01.cpp
//
#include <Magick++.h> // Magick::Geometry, Magick::Image
#include <iostream> // std::cout
#include <vector> // std::vector
#include <chrono> // std::chrono
#include "common.h"

int CropImageToTiles(const Magick::Image& image, const int cropWidth, const int cropHeight, std::vector<Magick::Image>& imageVector)
{
	const Magick::Geometry geometry = image.size();

	int imgWidth = static_cast<int>(geometry.width());
	int imgHeight = static_cast<int>(geometry.height());

	int yPos = 0;
	while (yPos < imgHeight) {
		int bhSize = ((yPos + cropHeight) > imgHeight) * (cropHeight - (yPos + cropHeight - imgHeight)) + ((yPos + cropHeight) <= imgHeight) * cropHeight;

		int xPos = 0;
		while (xPos < imgWidth) {
			int bwSize = ((xPos + cropWidth) > imgWidth) * (cropWidth - (xPos + cropWidth - imgWidth)) + ((xPos + cropWidth) <= imgWidth) * cropWidth;

			Magick::Image _image = image;
			_image.crop(Magick::Geometry(bwSize, bhSize, xPos, yPos));
			imageVector.push_back(_image);

			xPos = xPos + cropWidth;
		}

		yPos = yPos + cropHeight;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	std::string resultDir = common::pathAddSeparator(common::getDir(argv[0]) + "result");

    Magick::InitializeMagick(argv[0]);

    Magick::Image image;
	std::vector<Magick::Image> imageVector;
    try {
        image.read("logo:");

		const size_t split_width = 300;
		const size_t split_height = 200;

		std::cout << "[Begin] : CropImageToTiles()" << std::endl;
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		CropImageToTiles(image, split_width, split_height, imageVector);
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "    Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
		std::cout << "    Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
		std::cout << "    Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 << std::endl;
		std::cout << "[End] : CropImageToTiles()" << std::endl;

		std::string imagePath;
		for (int i = 0; i < static_cast<int>(imageVector.size()); i++) {
			imagePath = resultDir + std::string("logo") + std::to_string(i) + ".png";
			imageVector[i].write(imagePath);
		}

    } catch (Magick::Exception& error_) {
        std::cout << "Caught exception: " << error_.what() << std::endl;
        return 1;
    }

    return 0;
}
