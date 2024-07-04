#include "BioModeler.h"
#include "DICOM/DcmFile.h"

int main()
{
	std::string path("C:\\OPENGL\\BioModeler\\Data\\Test_img1");

	DICOM::DcmFile file(path);

	/*if (reader.Open()) 
	{
		std::cout << "\t File been opeen sucesfully" << std::endl;
	}*/


	return 0;
}
