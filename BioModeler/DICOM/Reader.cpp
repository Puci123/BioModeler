#include "Reader.h"

DICOM::Reader::Reader(const std::string& path)
	: m_Path(path)
{}

DICOM::Reader::~Reader()
{
	Close();
}



bool DICOM::Reader::Open()
{
	m_BinStream =  std::ifstream(m_Path, std::ios::binary);
	
	if (!m_BinStream.good())
	{
		LOG_ERROR("FALIED TO OPEN: " << m_Path);
		return false;
	}
}

bool DICOM::Reader::Good()
{
	return (m_BinStream.good());
}

void DICOM::Reader::Close()
{
	m_BinStream.close();
}

void DICOM::Reader::Setc(int64_t offset)
{
	ASSERT(Good());
	m_BinStream.seekg(offset, std::ios::beg);
}

void DICOM::Reader::Movec(int64_t offset)
{
	m_BinStream.seekg(m_BinStream.tellg() + offset, std::ios::beg);
}

uint64_t DICOM::Reader::ReadByteBlock(void* blcok, uint64_t n)
{
	ASSERT(Good());
	m_BinStream.read(reinterpret_cast<char*>(blcok), n);
	return m_BinStream.gcount();
}

void DICOM::Reader::ReadUint8(uint8_t* value)
{
	ReadByteBlock(value, 1);
}

void DICOM::Reader::ReadUint16(uint16_t* value)
{
	ReadByteBlock(value, 2);

}

void DICOM::Reader::ReadUint32(uint32_t* value)
{
	ReadByteBlock(value, 4);

}

void DICOM::Reader::ReadUint64(uint64_t* value)
{
	ReadByteBlock(value, 8);

}

void DICOM::Reader::ReadTag(Tag& tag)
{
	uint16_t group = 0;
	uint16_t element = 0;

	ReadUint16(&group);
	ReadUint16(&element);


	tag.element = element;
	tag.group   = group;
}

void DICOM::Reader::ReadField(Field& field)
{
	ReadTag(field.tag);
	ReadByteBlock(field.VR, 2);
	
	if (std::string(field.VR) == "UL")
	{

		uint16_t lenghtOfField;
		uint32_t lenght;

		ReadUint16(&lenghtOfField);
		ReadUint32(&lenght);

		ReadTag(field.tag);
	}


	return;
	
}
