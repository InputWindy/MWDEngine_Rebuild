#pragma once
#include "MWDSystem.h"
namespace MWDEngine {
	class MWDSYSTEM_API MWDFile
	{
		public:
			enum
			{
				MWDMAX_PATH = 256
			};
			MWDFile() {
				m_File = NULL;
				m_OpenMode =(TCHAR)"";
				m_FileSize = 0;
			};															
			~MWDFile() {
				if (m_File)
				{
					fclose(m_File);
					m_File = NULL;
				}
			};															

			
			//��һ���ļ���ָ��ȡ����ļ���HANDLE
			// ʾ������
			// 
			//file->Open(_T("C:/Users/InputWindy/Desktop/MWDEngine_git/MWDEngine/test.txt"),*_T("w"));
			//=========================================================================================
			bool Open(const TCHAR* pFileName, TCHAR OpenMode);					
			//��ȡHANDLE֮�����ִ��Read��Write��Seek��GetLine��GetFileSize��Flush
			bool Write(const void* pBuffer, unsigned int uiSize, unsigned int uiCount); 
			bool Read(void* pBuffer, unsigned int uiSize, unsigned int uiCount);
			bool Seek(unsigned int uiOffSet, unsigned int uiOrigin);					
			bool GetLine(void* pBuffer, unsigned int uiSize);

			FORCEINLINE unsigned int GetFileSize()const
			{
				return m_FileSize;
			}						
			FORCEINLINE bool Flush() { return(fflush(m_File) == 0); };//��ջ���������	
		protected:
			FILE* m_File;//void*														
			TCHAR m_OpenMode;													
			unsigned int m_FileSize;													
			TCHAR m_FileName[MWDMAX_PATH];												
	};
}
