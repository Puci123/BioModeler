#pragma once
#include <iostream>
#include <vector>

class SingelImage
{
	public:
		SingelImage();
		SingelImage(uint32_t width, uint32_t height, uint32_t bppAllocted, uint32_t bppUsed);

		void SetPixel(uint32_t x, uint32_t y, uint16_t value);
		void SaveToFile(const std::string& path);

		~SingelImage();


	private:
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_BppAllocted;
		uint32_t m_BppUsed;

		std::vector<uint16_t> m_PixelData;

};
