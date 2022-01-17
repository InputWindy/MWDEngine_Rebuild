#pragma once
#include "../MWDSystem/MWDMemManager.h"
#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
namespace MWDEngine {
	class MWDImage
	{
		public:
			MWDImage() {
				m_Data = NULL;
				m_Width = 0;
				m_Height = 0;
				m_DataSize = 0;
			};
			MWDImage(const char* pFileName) {
				m_Data = NULL;
				m_Width = 0;
				m_Height = 0;
				m_DataSize = 0;
				Load(pFileName);
			};
			~MWDImage() {
				//stbi_image_free(m_Data);
				m_Data = NULL;
			};
							
			bool Load(const char* pFileName) {
				m_Data = NULL;// stbi_load(pFileName, &m_Width, &m_Height, &m_nrChannel, 0);
				return true;
			};
			bool LoadFromBuffer(unsigned char* pBuffer, unsigned int uiSize) {
				m_Data = pBuffer;
				m_DataSize = uiSize;
				return true;
			};

			FORCEINLINE unsigned int GetWidth()const
			{
				return m_Width;
			}
			FORCEINLINE unsigned int GetHeight()const
			{
				return m_Height;
			}
			FORCEINLINE unsigned char* GetBuffer()
			{
				return m_Data;
			}
			FORCEINLINE unsigned int GetImgDataSize()const
			{
				return m_DataSize;
			}
		protected:
			//图像宽高
			int m_Width;
			int m_Height;

			//颜色通道数量
			int m_nrChannel;

			//图片数据
			

			//图片原始数据
			unsigned int m_DataSize;
			unsigned char* m_Data;

	};

}


