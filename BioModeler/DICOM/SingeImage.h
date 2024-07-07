#pragma once
#include <iostream>
#include <vector>

#include "StbiWrite.h"

class SingelImage
{
	public:
		SingelImage();
		SingelImage(uint32_t width, uint32_t height, uint32_t bppAllocted, uint32_t bppUsed);

		void SetPixel(uint32_t x, uint32_t y, uint16_t value);
		void SaveToFile(const std::string& path);
		void SkipBand(uint32_t n, uint32_t start = 0);


		~SingelImage();


	private:
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_BppAllocted;
		uint32_t m_BppUsed;

		uint32_t m_BiggestValue = 0;
		uint32_t m_SmalestValue = 100000000;

		std::vector<uint16_t> m_PixelData;

};
