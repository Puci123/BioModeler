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
	

	m_Reader.ReadToTag(TAGS::MEDIA_CLASS);
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
	}


	m_Reader.ReadToTag(TAGS::TRASFER_SYNTAX);
	m_Reader.ReadField(m_TrasferSyntax);

	std::string trasferSyntax    = FieldToString(m_TrasferSyntax);
	std::string trasferSyntaxStr = "Unrecogniezed";

	if (!TrasferUID2String(trasferSyntax, trasferSyntaxStr))
	{
		LOG_ERROR("Unrecogniezed media uid: " << trasferSyntax);
	}


	LOG("\n##################### SUCESFULLY OPEN #####################\n"  
		"\tMedia class: " << FieldToString(m_MediaClass) << " (" << mediaClassStr << ")" << "\n"
		"\tTrasfer syntax: " << FieldToString(m_TrasferSyntax) << " (" << trasferSyntaxStr << ")" << std::endl);



	m_IsValidFile = true;


	m_Reader.ReadToTag(TAGS::ROWS);
	m_Reader.ReadToTag(TAGS::COLUMNS);
	m_Reader.ReadToTag(TAGS::PIXEL_DATA);

	return;

}

DICOM::DcmFile::~DcmFile() {}


std::string DICOM::DcmFile::FieldToString(const Field& field)
{

	return std::string(field.buffer.begin(), field.buffer.end());
}
