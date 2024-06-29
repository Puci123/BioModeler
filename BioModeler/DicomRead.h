#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <vector>

#include "DebugUtilty.h"

class DicomRead
{
	public:
		DicomRead(const std::string& path);
		~DicomRead();

		bool Open();
		bool Close();
		bool Read();


		inline int Size() const
		{
			return m_FileSize;
		}

		inline bool	Good() const 
		{ 
			return m_BinStream.good(); 
		}
		
				
	private:
		std::int32_t	m_FileSize;
		std::string     m_Path;
		std::ifstream   m_BinStream;
};

