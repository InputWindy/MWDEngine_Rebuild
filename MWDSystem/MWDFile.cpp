#include "pch.h"
#include "MWDFile.h"
namespace MWDEngine {
	//uiOrigin=  SEEK_SET�� �ļ���ͷ SEEK_CUR�� ��ǰλ�� SEEK_END�� �ļ���β
	bool MWDFile::Seek(unsigned int uiOffSet, unsigned int uiOrigin)
	{
		if (!m_File)
		{
			return false;
		}
		
		fseek(m_File, uiOffSet, uiOrigin);
		return true;
	}

	//OpenMode���飺��
	//https://baike.baidu.com/item/fopen_s/6183225
	bool MWDFile::Open(const TCHAR* pFileName, TCHAR OpenMode)
	{
		//����Ѿ������ļ����ȹص�
		if (m_File)
		{
			fclose(m_File);
		}
		//���Բ�������
		if (!pFileName)
		{
			return false;
		}
		//�ļ�������
		unsigned int uiLen = MWDStrLen(pFileName);
		if (uiLen < MWDMAX_PATH - 1)
		{
			//���ļ�����ֵ��File���������
			if (!MWDMemcpy(m_FileName, pFileName, uiLen + 1))
				return false;
		}
		else
		{
			return false;
		}
		//m_OpenMode��ֵ
		m_OpenMode = OpenMode;
		//_tstat��ȡ�ļ���Ϣ
			/*
			struct _stat64i32
			{
				_dev_t         st_dev;
				_ino_t         st_ino;
				unsigned short st_mode;
				short          st_nlink;
				short          st_uid;
				short          st_gid;
				_dev_t         st_rdev;
				_off_t         st_size;
				__time64_t     st_atime;
				__time64_t     st_mtime;
				__time64_t     st_ctime;
			};*/
		struct _stat64i32 kStat;
		if (_tstat(pFileName, &kStat) != 0)
		{
			return false;
		}
		m_FileSize = kStat.st_size;

		//����ɹ�������ֵ��0
		errno_t uiError = _tfopen_s(&m_File, pFileName, &m_OpenMode);
		if (uiError)
		{
			return 0;
		}
		if (!m_File)
		{
			return 0;
		}

		return true;
	}
	bool MWDFile::Write(const void* pBuffer, unsigned int uiSize, unsigned int uiCount)
	{
		if (!m_File)
		{
			return false;
		}
		if (!pBuffer || !uiSize || !uiCount)
		{
			return false;
		}
		fwrite(pBuffer, uiSize, uiCount, m_File);
		return true;
	}
	bool MWDFile::Read(void* pBuffer, unsigned int uiSize, unsigned int uiCount) {
		if (!m_File)
		{
			return false;
		}
		if (!pBuffer || !uiSize || !uiCount)
		{
			return false;
		}
		//m_File���Ǹ���������
		fread(pBuffer, uiSize, uiCount, m_File);
		return true;
	}
	bool MWDFile::GetLine(void* pBuffer, unsigned int uiSize) {
		if (!m_File)
		{
			return false;
		}
		if (!pBuffer || !uiSize)
		{
			return false;
		}
		if (!_fgetts((TCHAR*)pBuffer, uiSize, m_File))
			return false;
		return true;
	}
}
