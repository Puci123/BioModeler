#include "BioModeler.h"
#include "DICOM/DcmFile.h"
#include "Stbi.h"


int main()
{
	std::string path("C:\\OPENGL\\BioModeler\\Data\\Test_img1");
	std::string savePath("TestIMG2.bmp");

	DICOM::DcmFile file(path);
	if (!file.LoadMedia())
	{
		std::cout << "Failed to load media " << std::endl;
	}
	else
	{
		std::cout << "Media OK!" << std::endl;
		file.SaveToFile(savePath);
		file.OpenImage(savePath);
	
	}



	return 0;
}
