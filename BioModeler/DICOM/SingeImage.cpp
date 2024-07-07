#include "SingeImage.h"
#include "DebugUtilty.h"


SingelImage::SingelImage() : SingelImage(0, 0, 0, 0)
{}

SingelImage::SingelImage(uint32_t width, uint32_t height, uint32_t bppAllocted, uint32_t bppUsed)
	:m_Width(width), m_Height(height), m_BppAllocted(bppAllocted), m_BppUsed(bppUsed)
{

	m_PixelData.resize(width * height);
}



void SingelImage::SetPixel(uint32_t x, uint32_t y, uint16_t value)
{
	ASSERT(x < m_Width);
	ASSERT(y < m_Height);

	if (value > m_BiggestValue) m_BiggestValue = value;
	if (value < m_SmalestValue) m_SmalestValue = value;
	
	m_PixelData[y * m_Width + x] = value;
	
}

void SingelImage::SaveToFile(const std::string& path)
{
	std::vector<uint8_t> tempBuffer;
	tempBuffer.resize(m_Width * m_Height);
	
	//Conveert pixel values to 1B per chanel [0-255]
	for (size_t y = 0; y < m_Height; y++)
	{
		for (size_t x = 0; x < m_Width; x++)
		{
			//uint8_t value = std::floor((( ) / static_cast<float>(m_BiggestValue - m_SmalestValue)) * 256.f);
			uint8_t value = std::min(255, static_cast<int32_t>(m_PixelData[y * m_Width + x]));
			tempBuffer[y * m_Width + x] = value;
		}
	}

	stbi_write_bmp(path.c_str(), m_Width, m_Height, 1, &tempBuffer[0]);
	return;
	
}

SingelImage::~SingelImage()
{}
