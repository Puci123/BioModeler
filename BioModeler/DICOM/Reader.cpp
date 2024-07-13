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
	ASSERT(m_BinStream.is_open());
	m_BinStream.seekg(offset, std::ios::beg);
}

void DICOM::Reader::Movec(int64_t offset)
{
	m_BinStream.seekg(m_BinStream.tellg() + offset, std::ios::beg);
}

size_t DICOM::Reader::ReadByteBlock(void* blcok, uint64_t n)
{
	ASSERT(Good());
	m_BinStream.read(reinterpret_cast<char*>(blcok), n);
	return m_BinStream.gcount();
}

bool DICOM::Reader::ReadUint8(uint8_t* value)
{
	return ReadByteBlock(value, 1) == 1;
	
}

bool DICOM::Reader::ReadUint16(uint16_t* value)
{
	return ReadByteBlock(value, 2) == 2;

}

bool DICOM::Reader::ReadUint32(uint32_t* value)
{
	return ReadByteBlock(value, 4) == 4;

}

bool DICOM::Reader::ReadUint64(uint64_t* value)
{
	return ReadByteBlock(value, 8) == 8;

}

bool DICOM::Reader::ReadTag(Tag& tag)
{
	uint16_t group = 0;
	uint16_t element = 0;

	if(!ReadUint16(&group)) return false;
	if(!ReadUint16(&element)) return false;


	tag.element = element;
	tag.group   = group;

	return true;
}

bool DICOM::Reader::ReadVR(char target[2])
{
	uint8_t first;
	uint8_t second;


	if (!ReadUint8(&first))  return false;
	if (!ReadUint8(&second)) return false;

	if (first == 0)
	{

		Movec(-2);
		return true;
	}

	if (second == 0)
	{
		Movec(-2);
		return true;
	}

	target[0] = first;
	target[1] = second;

	return true;
}

void DICOM::Reader::ReadField(Field& field)
{
	
	ReadTag(field.tag);
	ReadVR(field.VR); //TODO: ASSUME IMPLICT VT
	

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
	//else if(compareVr(field.VR, "\0\0"))
	//{
	//	//S
	//	ReadUint16(&field.size);
	//	field.buffer.resize(field.size);
	//	ReadByteBlock(&field.buffer[0], field.size);
	//}
	else if(compareVr(field.VR,"US"))
	{
		ReadUint16(&field.size);
		field.buffer.resize(field.size);
		ReadByteBlock(&field.buffer[0], field.size);
	}
	else
	{
		LOG_WARINIG("SKIPPING UNSPECIFED VR: " << field.VR[0] << field.VR[1]);
		
		uint32_t lenght = 0;
		ReadUint32(&lenght);

		field.buffer.resize(lenght);
		field.size = static_cast<uint16_t>(lenght);
		ReadByteBlock(&field.buffer[0], lenght);
		LOG("Sucesfyl read filed with tag: " << std::hex << field.tag.group << " " << field.tag.element);
		return;
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

bool DICOM::Reader::MoveToTag(const Tag& tag)
{
	Tag temp{ 0,0 };
	bool found = false;
	size_t cursorsPos = m_BinStream.tellg();

	while (Good())
	{
		if (!ReadTag(temp))
		{
			break;
		}
		Movec(-2);		//Allingin window

		if ((temp.element == tag.element && temp.group == tag.group))
		{
			found = true;
			break;
		}
	}

	Movec(-2); //Move to begiining of tag

	if (found)
	{
		LOG("Tag founed: " << std::hex << temp.group << " " << temp.element << " TAG pos:" << std::dec << m_BinStream.tellg());
		
		
		uint8_t bufferSnap[256];
		ReadByteBlock(bufferSnap, 256);
		Movec(-256);
		

		return true;
	}

	
	
	LOG_WARINIG("Tag is missing: " << std::hex << tag.group << " " << tag.element);

	//Clear bits and restors cursor pos
	m_BinStream.clear();
	Setc(cursorsPos);
	m_BinStream.tellg();
	return false;
}
