#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "DebugUtilty.h"

namespace DICOM 
{

	struct Tag
	{
		uint16_t group;
		uint16_t element;
	};

	struct Field
	{
		Tag tag{ 0,0 };
		char VR[2] = { 'X','X' };
		uint16_t size = 0;
		std::vector<uint8_t> buffer;

	};

	class Reader
	{
		public:
			Reader(const std::string& path);
			~Reader();

			bool Open();
			bool Good();
			void Close();

			void Setc(int64_t offset);
			void Movec(int64_t offset);

			size_t ReadByteBlock(void* blcok, uint64_t n);
			
			bool ReadUint8 (uint8_t* value);
			bool ReadUint16(uint16_t* value);
			bool ReadUint32(uint32_t* value);
			bool ReadUint64(uint64_t* value);

			bool ReadTag(Tag& tag);
			bool ReadVR(char target[2]);
			void ReadField(Field& field);

			bool MoveToTag(const Tag& tag);

	
		private:
			std::string m_Path;
			std::ifstream m_BinStream;


	};

	
}