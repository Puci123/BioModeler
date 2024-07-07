#include "BioModeler.h"
#include "DICOM/DcmFile.h"
#include "Stbi.h"


int main()
{
	std::string path("C:\\OPENGL\\BioModeler\\Data\\Test_img1");

	DICOM::DcmFile file(path);
	if (!file.LoadMedia())
	{
		std::cout << "Failed to load media " << std::endl;
	}
	else
	{
		std::cout << "Media OK!" << std::endl;
	}



	return 0;
}
