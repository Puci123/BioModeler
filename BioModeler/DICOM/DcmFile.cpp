#include "DcmFile.h"

#define	 READ_TAG_SAFE(TAG,TARGET) if (!m_Reader.MoveToTag(TAG)){\
										LOG_ERROR("Can't find crucial tag: " << TAG.group << " " << TAG.element);\
										return false;}\
									m_Reader.ReadField(TARGET)




bool TestStrings(const std::string& a, const std::string& b)
{
	if (a.size() != b.size())
	{
		LOG_WARINIG("Diffrent lenghts");
		return false;
	}

	return true;
}


DICOM::DcmFile::DcmFile(const std::string& path)
	: m_Reader(path)
{
	LOG("Opening dicom file....");

	m_Reader.Open();
	if (!m_Reader.Good()) return;
	
	std::string dcmSting("XXXX");
	m_Reader.Setc(128);								//Skip first 128 bytes contatis prambule
	m_Reader.ReadByteBlock(&dcmSting[0], 4);

	if (dcmSting != "DICM")
	{
		LOG_ERROR("INVALID FORAMT OF FILE: " << path);
		return;
	}
	

	m_Reader.MoveToTag(TAGS::MEDIA_CLASS);
	m_Reader.ReadField(m_MediaClass);

	std::string mediaClass     = FieldToString(m_MediaClass);
	std::string mediaClassStr  = "Unrecogniezed";
	std::string temp = "1.2.840.10008.5.1.4.1.1.1.1";


	//NOTE: DEBUG
	TestStrings(mediaClass, "1.2.840.10008.5.1.4.1.1.1.1");
	//


	if (!MediaUID2String(mediaClass, mediaClassStr))
	{
		LOG_ERROR("Unrecogniezed media uid: " << mediaClass);
		return;
	}


	m_Reader.MoveToTag(TAGS::TRASFER_SYNTAX);
	m_Reader.ReadField(m_TrasferSyntax);

	std::string trasferSyntax    = FieldToString(m_TrasferSyntax);
	std::string trasferSyntaxStr = "Unrecogniezed";

	if (!TrasferUID2String(trasferSyntax, trasferSyntaxStr))
	{
		LOG_ERROR("Unrecogniezed trasfer syntax: " << trasferSyntax);
		return;
	}

	//Specifie byte order
	if (trasferSyntax == TRASFER::IMPLICT)
	{
		m_BitOrder = TRASFER::BitOrders::litleEndian;
	}
	else
	{
		LOG_WARINIG("Assuming ltile endian as bytes order");
		m_BitOrder = TRASFER::BitOrders::litleEndian;

	}



	LOG("\n##################### SUCESFULLY OPEN #####################\n"  
		"\tMedia class: " << FieldToString(m_MediaClass) << " (" << mediaClassStr << ")" << "\n"
		"\tTrasfer syntax: " << FieldToString(m_TrasferSyntax) << " (" << trasferSyntaxStr << ")" << std::endl);



	m_IsValidFile = true;
	return;

}

DICOM::DcmFile::~DcmFile() {}

void DICOM::DcmFile::SaveToFile(const std::string& path)
{
	m_Media.SaveToFile(path);
}

void DICOM::DcmFile::OpenImage(const std::string& path)
{
	std::wstring widePath(path.begin(),path.end());
	HINSTANCE result = ShellExecuteW(NULL, L"open", widePath.c_str(), NULL, NULL, SW_SHOW);
}


bool DICOM::DcmFile::LoadMedia()
{
	LOG("Loding media file ....");
	if (!Good()) 
	{
		LOG_WARINIG("Can't meda data, file is in not in good state");
		return false;
	}

	if (FieldToString(m_MediaClass) == MEDIA::X_RAY_IMAGE)	return LoadXRay();
	if (FieldToString(m_MediaClass) == MEDIA::CT_IMAGE)	    return LoadCT();

	return false;
}


bool DICOM::DcmFile::LoadXRay()
{
	//TO DO: LOAD REQIERD DATA
	//https://dicom.nema.org/dicom/2013/output/chtml/part03/sect_A.26.html
	//https://dicom.nema.org/dicom/2013/output/chtml/part03/sect_C.7.html#sect_C.7.6.3


	Field rows;
	Field columns;
	Field bppAloc;
	Field bppUsed;
	Field highBit;
	Field reprezenatation;
	Field biggestValue;
	Field smallestValu;

	READ_TAG_SAFE(TAGS::ROWS,rows);
	READ_TAG_SAFE(TAGS::COLUMNS, columns);
	READ_TAG_SAFE(TAGS::BITS_ALLOCETED, bppAloc);
	READ_TAG_SAFE(TAGS::BITS_STORED, bppUsed);
	READ_TAG_SAFE(TAGS::HIGH_BIT, highBit);
	READ_TAG_SAFE(TAGS::PIXEL_REPS, reprezenatation);
		
	uint16_t width = 0;
	uint16_t height = 0;
	uint16_t alloceted = 0;	//Bits value
	uint16_t used = 0;		//Bits value

	memcpy(&width, &columns.buffer[0], 2);
	memcpy(&height, &rows.buffer[0], 2);
	memcpy(&alloceted, &bppAloc.buffer[0], 2);
	memcpy(&used, &bppUsed.buffer[0], 2);

	m_Media = SingelImage(width,height,alloceted,used);
	LOG("Image size: " << std::dec << width << "x" << height << " BPP aloceted: " << alloceted << " BPP used: " << used);

	//Convert bits 2 bytes round up
	alloceted = std::ceil(alloceted / 8.f);
	used = std::ceil(used / 8.f);


	//Load pixel data
	m_Reader.MoveToTag(TAGS::PIXEL_DATA);
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			//NOTE: temporary use alloced bits value to determin size
			uint16_t value = 0;
			m_Reader.ReadUint16(&value);
			//

			m_Media.SetPixel(x, y, value);
		}
	}

	LOG("Scucesfully loded media file");
	return true;
}

bool DICOM::DcmFile::LoadCT()
{
	/*
		https://dicom.nema.org/dicom/2013/output/chtml/part03/sect_C.7.html#sect_C.7.6.3
		https://dicom.nema.org/dicom/2013/output/chtml/part03/sect_C.7.html#sect_C.7.6.1
		https://dicom.nema.org/dicom/2013/output/chtml/part03/sect_A.3.html
	*/

	Field rows;
	Field columns;
	Field bppAloc;
	Field bppUsed;
	Field highBit;
	Field reprezenatation;

	READ_TAG_SAFE(TAGS::ROWS, rows);
	READ_TAG_SAFE(TAGS::COLUMNS, columns);
	READ_TAG_SAFE(TAGS::BITS_ALLOCETED, bppAloc);
	READ_TAG_SAFE(TAGS::BITS_STORED, bppUsed);
	READ_TAG_SAFE(TAGS::HIGH_BIT, highBit);
	READ_TAG_SAFE(TAGS::PIXEL_REPS, reprezenatation);

	uint16_t width = 0;
	uint16_t height = 0;
	uint16_t alloceted = 0;	//Bits value
	uint16_t used = 0;		//Bits value

	memcpy(&width, &columns.buffer[0], 2);
	memcpy(&height, &rows.buffer[0], 2);
	memcpy(&alloceted, &bppAloc.buffer[0], 2);
	memcpy(&used, &bppUsed.buffer[0], 2);

	alloceted = std::ceil(alloceted / 8.f);
	used = std::ceil(used / 8.f);


	//Read pixel data
	m_Reader.MoveToTag(TAGS::PIXEL_DATA);
	
	m_Media = SingelImage(width, height, alloceted, used);
	LOG("Image size: " << std::dec << width << "x" << height << " BPP aloceted: " << alloceted << " BPP used: " << used);
	
	//TO DO: 
	//load jpg to vector
	//use stbi_load_from_memory to convert it to bit map


	return true;
}


std::string DICOM::DcmFile::FieldToString(const Field& field)
{

	return std::string(field.buffer.begin(), field.buffer.end());
}


