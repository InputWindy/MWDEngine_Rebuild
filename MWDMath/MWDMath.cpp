#include "pch.h"
#include "MWDMath.h"
namespace MWDEngine {
	MWDREAL			FastSin[361];
	MWDREAL			FastCos[361];

	unsigned int	Rounding(MWDREAL fNum)
	{
		return unsigned int(fNum + 0.5f);
	}
	MWDREAL			GetFastSin(unsigned int i)
	{
		return FastSin[i];
	}
	MWDREAL			GetFastCos(unsigned int i)
	{
		return FastCos[i];
	}
	
//计算CRC对照表
#define POLYNOMIAL 0x04c11db7L
	unsigned int	CRCTable[256];
	bool			MWDInitMath()
	{
		MWDInitCRCTable();
		for (unsigned int i = 0; i <= 360; i++)
		{
			MWDREAL iRadian = AngleToRadian(MWDREAL(i));
			FastSin[i] = SIN(iRadian);
			FastCos[i] = COS(iRadian);
		}
		return 1;
	}
	void			MWDInitCRCTable()
	{
		register int i, j;
		register unsigned long crc_accum;
		for (i = 0; i < 256; i++)
		{
			crc_accum = ((unsigned long)i << 24);
			for (j = 0; j < 8; j++)
			{
				if (crc_accum & 0x80000000L) crc_accum = (crc_accum << 1) ^ POLYNOMIAL;
				else crc_accum = (crc_accum << 1);
			}
			CRCTable[i] = crc_accum;
		}
	}
	unsigned int	CRC32Compute(const void* pData, unsigned int uiDateSize)
	{
		unsigned int uiResult;

		if (uiDateSize == 0) return 0;

		const unsigned char* pDateTemp = (const unsigned char*)pData;
		uiResult = *pDateTemp++ << 24;
		if (uiDateSize > 1)
		{
			uiResult |= *pDateTemp++ << 16;
			if (uiDateSize > 2)
			{
				uiResult |= *pDateTemp++ << 8;
				if (uiDateSize > 3) uiResult |= *pDateTemp++;
			}
		}
		uiResult = ~uiResult;

		for (size_t i = 4; i < uiDateSize; i++)
		{
			uiResult = (uiResult << 8 | *pDateTemp++) ^ CRCTable[uiResult >> 24];
		}

		return ~uiResult;
	}
}