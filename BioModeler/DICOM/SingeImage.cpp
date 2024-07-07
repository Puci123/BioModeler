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

	m_PixelData[y * m_Width + x] = value;
	
}

void SingelImage::SaveToFile(const std::string& path)
{


	return;
}

SingelImage::~SingelImage()
{}
