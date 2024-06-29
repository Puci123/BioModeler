#include "BioModeler.h"
#include "DicomRead.h"
#include "DebugUtilty.h"


int main()
{
	std::string path("C:\\OPENGL\\BioModeler\\Data\\Test_img1");

	DicomRead read(path);
	if (!read.Open())
	{
		LOG_ERROR("Can't open data file: " << path);
	}
	else
	{
		LOG("File open succesfll, size: " << read.Size() << "b");
		read.Read();
	}



	return 0;
}
