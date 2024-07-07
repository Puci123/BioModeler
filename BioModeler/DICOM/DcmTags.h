#pragma once

#include "Reader.h"
#include <unordered_map>
#include <string>

namespace DICOM 
{
	namespace TAGS
	{
		//Header infromation
		inline constexpr Tag MEDIA_CLASS       { 0x0002,0x0002 };
		inline constexpr Tag TRASFER_SYNTAX	   { 0x0002,0x0010 };

		//Patinet infromation
		inline constexpr Tag PATINET_NAME		{ 0x0010,0x0010 };
		inline constexpr Tag BODY_PART			{ 0x0018,0x0015 };

		//image data
		inline constexpr Tag ROWS				{ 0x0028,0X0010 };
		inline constexpr Tag COLUMNS			{ 0x0028,0X0011 };
		inline constexpr Tag IMAGE_POS			{ 0x0020,0x0032 };
		inline constexpr Tag IMAGE_ORENATATION  { 0x0020,0x0037 };
		inline constexpr Tag PIXEL_SPACING		{ 0x0028,0x0030 };
		inline constexpr Tag BITS_ALLOCETED		{ 0x0028,0x0100 };
		inline constexpr Tag BITS_STORED		{ 0x0028,0x0101 };
		inline constexpr Tag HIGH_BIT		    { 0x0028,0x0102 };
		inline constexpr Tag PIXEL_REPS			{ 0x0028,0x0103 };

		inline constexpr Tag PIXEL_DATA			{ 0x7FE0,0x0010 };
	}

	namespace MEDIA
	{
		inline constexpr std::string_view X_RAY_IMAGE		= "1.2.840.10008.5.1.4.1.1.1.1";
	}

	namespace TRASFER
	{
		inline constexpr std::string_view IMPLICT			= "1.2.840.10008.1.2";



		enum class BitOrders
		{
			unspecifed		= 0,
			litleEndian		= 0,
			bigEndian		= 0
		};
	}

	


	inline std::unordered_map<std::string, std::string> MediaClasses
	{
		{"test","test" },
		{"1.2.840.10008.5.1.4.1.1.1.1", "Digital X-Ray Image IOD"}
	};

	inline std::unordered_map<std::string, std::string> TrasferClasses
	{
		{"test","test"},
		{"1.2.840.10008.1.2", "Implicit VR Endian: Default Transfer Syntax for DICOM"}
	};

	//============================== Functions to convert UID to string description ==============================//
	inline bool MediaUID2String(std::string uid, std::string& target)
	{
		auto it = MediaClasses.find(uid);
		if (it == MediaClasses.end()) return false;

		target = it->second;
		return true;
	}

	inline bool TrasferUID2String(std::string uid, std::string& target)
	{
		auto it = TrasferClasses.find(uid);
		if (it == TrasferClasses.end()) return false;

		target = it->second;
		return true;
	}
}
