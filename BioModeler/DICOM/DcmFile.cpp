#include "DcmFile.h"

DICOM::DcmFile::DcmFile(const std::string& path)
	: m_Reader(path)
{
	m_Reader.Open();
	if (!m_Reader.Good()) return;
	
	std::string dcmSting("XXXX");
	m_Reader.Setc(128); //Skip first 128 bytes 
	m_Reader.ReadByteBlock(&dcmSting[0], 4);

	if (dcmSting != "DICM")
	{
		LOG_ERROR("INVALID FORAMT");
		return;
	}

	m_IsValidFile = true;


	Field field;
	m_Reader.ReadField(field);

	return;

}

DICOM::DcmFile::~DcmFile()
{
}
