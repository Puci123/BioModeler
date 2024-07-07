#pragma once

#include "Reader.h"
#include "DcmTags.h"
#include "SingeImage.h"

namespace DICOM
{

	class DcmFile
	{
		public:
			DcmFile(const std::string& path);
			~DcmFile();

			inline bool Good() const { return m_IsValidFile; }
			bool LoadMedia();


		private:
			Reader m_Reader;

			bool      m_IsValidFile  = false;
			uint64_t  m_HeaderLenght = 0;

			Field m_MediaClass;
			Field m_TrasferSyntax;
			TRASFER::BitOrders m_BitOrder = TRASFER::BitOrders::unspecifed;


			SingelImage m_Media;

			bool LoadXRay();

			std::string FieldToString(const Field& field);

	};
}

