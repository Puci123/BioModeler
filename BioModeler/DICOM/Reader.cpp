#include "Reader.h"



//================================= HELPERS  ================================= //


static bool compareVr(char vr[2], const std::string& vrb)
{
	return (vrb.size() == 2 && std::strncmp(vr, vrb.c_str(), 2) == 0);
}



//================================= Reader class  ================================= //

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

	return true;
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

	if (compareVr(field.VR, "UL"))
	{
		ReadUint16(&field.size);
		field.buffer.resize(field.size);
		ReadByteBlock(&field.buffer[0], field.size);
	}
	else if(compareVr(field.VR, "OB"))
	{
		
		Movec(2);					//Skip two bytes used for alligiemnt ??
		
		uint32_t size = 0;
		ReadUint32(&size);

		field.size = static_cast<uint16_t>(size);	//WARINIG: for test puropse only!
		field.buffer.resize(field.size);
		ReadByteBlock(&field.buffer[0], field.size);
	}
	else if(compareVr(field.VR,"UI"))
	{
		//SAME AS UL
		ReadUint16(&field.size);
		field.buffer.resize(field.size);
		ReadByteBlock(&field.buffer[0], field.size);
	}
	else if (compareVr(field.VR, "SH"))
	{
		//SAME AS UL
		ReadUint16(&field.size);
		field.buffer.resize(field.size);
		ReadByteBlock(&field.buffer[0], field.size);
	}
	//else if(compareVr(field.VR, "AE"))
	//{
	//	//SAME AS UL
	//	ReadUint16(&field.size);
	//	field.buffer.resize(field.size);
	//	ReadByteBlock(&field.buffer[0], field.size);
	//}
	else
	{
		LOG_ERROR("UNRECOGNIZED FILED TYPE!! in filed with tag: " << std::hex << field.tag.group << " " << field.tag.element);
		LOG("BIT in file: " << m_BinStream.tellg());

		//BUFFOR SNAPSHOT 
		Movec(-6);
		int8_t bufforSnapShot[256];
		ReadByteBlock(bufforSnapShot, 256);
		//
		ASSERT(false);
	}

	//Romve /0 bytes at end buffer ??
	while (field.buffer.back() == 0)
	{
		field.buffer.pop_back();
		field.size--;
	}

	LOG("Sucesfyl read filed with tag: " << std::hex << field.tag.group << " "  << field.tag.element);
	return;
	
	
}

void DICOM::Reader::ReadToTag(const Tag& tag)
{
	Tag temp{ 0,0 };

	while (temp.element != tag.element || temp.group != tag.group)
	{
		ReadTag(temp);
		Movec(-2);		//Allingin window
	}

	Movec(-2); //Move to begiining of tag

	LOG("Tag founed: " << std::hex << temp.group << " " << temp.element << " TAG pos:" << std::dec << m_BinStream.tellg());

	//NOTE: DEBUG ONLY
	//int8_t bufforSnapShot[256];
	//ReadByteBlock(bufforSnapShot, 256);
}
