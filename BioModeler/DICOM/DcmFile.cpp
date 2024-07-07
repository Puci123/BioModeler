#include "DcmFile.h"


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


bool DICOM::DcmFile::LoadMedia()
{
	LOG("Loding media file ....");
	if (!Good()) 
	{
		LOG_WARINIG("Can't meda data, file is in not in good state");
		return false;
	}

	if (FieldToString(m_MediaClass) == MEDIA::X_RAY_IMAGE) return LoadXRay();


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

	if (!m_Reader.MoveToTag(TAGS::ROWS))
	{
		LOG_ERROR("Can't find crucial tag: " << TAGS::ROWS.group << " " << TAGS::ROWS.element);
		return false;
	}
	m_Reader.ReadField(rows);

	if (!m_Reader.MoveToTag(TAGS::COLUMNS))
	{
		LOG_ERROR("Can't find crucial tag: " << TAGS::COLUMNS.group << " " << TAGS::COLUMNS.element);
		return false;
	}
	m_Reader.ReadField(columns);

	if (!m_Reader.MoveToTag(TAGS::BITS_ALLOCETED))
	{
		LOG_ERROR("Can't find crucial tag: " << TAGS::BITS_ALLOCETED.group << " " << TAGS::BITS_ALLOCETED.element);
		return false;
	}
	m_Reader.ReadField(bppAloc);

	if (!m_Reader.MoveToTag(TAGS::BITS_STORED))
	{
		LOG_ERROR("Can't find crucial tag: " << TAGS::BITS_STORED.group << " " << TAGS::BITS_STORED.element);
		return false;
	}
	m_Reader.ReadField(bppUsed);

	if (!m_Reader.MoveToTag(TAGS::HIGH_BIT))
	{
		LOG_ERROR("Can't find crucial tag: " << TAGS::HIGH_BIT.group << " " << TAGS::HIGH_BIT.element);
		return false;
	}
	m_Reader.ReadField(highBit);

	if (!m_Reader.MoveToTag(TAGS::PIXEL_REPS))
	{
		LOG_ERROR("Can't find crucial tag: " << TAGS::PIXEL_REPS.group << " " << TAGS::PIXEL_REPS.element);
		return false;
	}
	m_Reader.ReadField(reprezenatation);

	//TODO: us information about hight bit and pixel reprezentation;

	//Colums and rows alaced and usedbits use VR = US fixed size = 2B
	//NOTE: CHEK if lenght is okey
	
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
	
	//m_Media.SaveToFile("testOutput1.bmp");
	return true;
}


std::string DICOM::DcmFile::FieldToString(const Field& field)
{

	return std::string(field.buffer.begin(), field.buffer.end());
}


