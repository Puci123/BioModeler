#include "DicomRead.h"


DicomRead::DicomRead(const std::string& path)
	: m_Path(path), m_FileSize(-1)
{}

DicomRead::~DicomRead() {}

bool DicomRead::Open()
{
	m_BinStream = std::ifstream(m_Path, std::ios::binary);
	if (!m_BinStream.good()) return false;

	//Read file size
	m_BinStream.seekg(0, std::ios::end);
	m_FileSize = m_BinStream.tellg();
	m_BinStream.seekg(0, std::ios::beg);

	return true;
}

bool DicomRead::Close()
{
	m_BinStream.close();
	return true;
}



//https://dicom.nema.org/dicom/2013/output/chtml/part10/chapter_7.html
bool DicomRead::Read()
{
	if (!Good()) return false;

	std::vector<char> buffer(m_FileSize);
	m_BinStream.read(&(buffer[0]), m_FileSize);

	//Skip Prambule first 128B 
	//DICOM Prefix 4B in dicom files shude be "DICM"

	if (buffer[128] != 'D' && buffer[129] != 'C' && buffer[130] != 'I' && buffer[131] != 'M')
	{
		LOG_WARINIG("INVALID FILE FORMAT: " << m_Path);
		return false;
	}

	//132B
	//File Meta Information Group Length
	
	uint32_t fileMetaInformationGroupLength   = *reinterpret_cast<uint32_t*>(&buffer[136]);
	
	//File Meta Information Group Version 2 Bytes tag + 2 Bytes field each of them have siginfiact value 
	uint8_t metaVersion0 = 0;
	uint8_t metaVersion1 = 0; //TODO: 




	return true;
}




