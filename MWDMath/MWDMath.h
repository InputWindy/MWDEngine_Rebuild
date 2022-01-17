#pragma once
#include "../MWDSystem/MWDSystem.h"
#include "../MWDSystem/MWDMemManager.h"
#include <windows.h> 
#include <math.h>  
#include <xmmintrin.h>
#include <emmintrin.h>
#include <iostream>
#include "../depend/glm/glm/glm.hpp"
#include "../depend/glm/glm/gtc/type_ptr.hpp"
#include "../depend/glm/glm/vec3.hpp" // glm::vec3
#include "../depend/glm/glm/vec4.hpp"  // glm::vec4
#include "../depend/glm/glm/mat4x4.hpp" // glm::mat4
#include "../depend/glm/glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "../depend/glm/glm/gtc/constants.hpp" // glm::pi
using namespace std;
#define MWDMATH_EXPORTS
#ifdef MWDMATH_EXPORTS
#define  MWDMATH_API __declspec(dllexport) 
#else
#define  MWDMATH_API __declspec(dllimport)
#endif

namespace MWDEngine {

	#pragma region 基础数学宏
	#define  MWDFLOAT 
	#ifdef MWDFLOAT 
	#define MWDREAL 	float
	#define SIN(x)		sinf(x)
	#define COS(x)		cosf(x)
	#define TAN(x)		tanf(x)
	#define ASIN(x)		asinf(x)
	#define ACOS(x)		acosf(x)
	#define ATAN(x)		atanf(x)
	#define ATAN2(y,x)	atan2f(y,x)
	#define SQRT(x)		sqrtf(x)
	#define POW(x,y)	powf(x,y)
	#define CEIL(x)		ceilf(x);
	#define EXP(x)		expf(x);
	#define FMOD(x,y)	fmodf(x,y)
	#else ifdef MWDDOUBLE
	#define MWDREAL 		double
	#define SIN(x)		sin(x)
	#define COS(x)		cos(x)
	#define TAN(x)		tan(x)
	#define ASIN(x)		asin(x)
	#define ACOS(x)		acos(x)
	#define ATAN(x) 	atan(x)
	#define ATAN2(y,x) 	atan2(y,x)
	#define SQRT(x)		sqrt(x)
	#define POW(x,y)	pow(x,y)
	#define CEIL(x)		ceil(x);
	#define EXP(x)		exp(x);
	#define FMOD(x,y)	fmod(x,y)
	#endif
	#pragma endregion

	#pragma region 精度设置
	#define EPSILON_E3 (MWDREAL)(1E-3) 
	#define EPSILON_E4 (MWDREAL)(1E-4) 
	#define EPSILON_E5 (MWDREAL)(1E-5)
	#define EPSILON_E6 (MWDREAL)(1E-6)
	#pragma endregion

	#pragma region 常用Enum
	#define MWDFRONT		0
	#define MWDBACK			1
	#define MWDON			2

	#define MWDCLIPPED		3
	#define MWDCULLED		4
	#define MWDVISIBLE		5

	#define MWDINTERSECT	3
	#define MWDOUT			4
	#define MWDIN			5

	#define MWDNOINTERSECT	6
	#pragma endregion

	#pragma region 特殊常量
	#define MWDMIN_CHAR		(-128)      
	#define MWDMAX_CHAR		127       
	#define MWDMIN_INT		(-2147483647 - 1) 
	#define MWDMAX_INT       2147483647    
	#define MWDMAX_REAL		3.402823466e+38F
	#define MWDMIN_REAL		1.175494351e-38F
	#define MWDMAX_INTEGER	0xffffffff
	#define MWDREAL_1		0.99999f		
	#define MWDPI			3.14159265f
	#define MWDPI2			1.5707963f
	#define MWD2PI			6.2831853f
	#pragma endregion

	#pragma region 阴影标志位
	#define MWDSHADOWON		0
	#define MWDSHADOWBACK	1
	#define MWDNOSHADOW		2
	#pragma endregion

	#pragma region index
	#define MWDUSHORT_INDEX	unsigned short
	#define MWDUINT_INDEX	unsigned int
	#pragma endregion

	//计算n按照align对齐的结果
	FORCEINLINE unsigned int CalcAlign(unsigned int n, unsigned int align)
	{
		return ((n + align - 1) & (~(align - 1)));
	}
	//弧度转角度
	FORCEINLINE MWDREAL RadianToAngle(MWDREAL Radian)
	{
		return (Radian * 180.0f) / MWDPI;
	}
	//角度转弧度
	FORCEINLINE MWDREAL AngleToRadian(MWDREAL Angle)
	{
		return (Angle * MWDPI) / 180.0f;
	}
	//是不是2的倍数
	FORCEINLINE bool IsTwoPower(unsigned int uiN)
	{
		return !(uiN & (uiN - 1));
	}
	//half是着色器类型
	FORCEINLINE unsigned short FloatToHalf(MWDREAL Value)
	{
		unsigned int Result;

		unsigned int IValue = ((unsigned int*)(&Value))[0];
		unsigned int Sign = (IValue & 0x80000000U) >> 16U;
		IValue = IValue & 0x7FFFFFFFU;      // Hack off the sign

		if (IValue > 0x47FFEFFFU)
		{
			// The number is too large to be represented as a half.  Saturate to infinity.
			Result = 0x7FFFU;
		}
		else
		{
			if (IValue < 0x38800000U)
			{
				// The number is too small to be represented as a normalized half.
				// Convert it to a denormalized value.
				unsigned int Shift = 113U - (IValue >> 23U);
				IValue = (0x800000U | (IValue & 0x7FFFFFU)) >> Shift;
			}
			else
			{
				// Rebias the exponent to represent the value as a normalized half.
				IValue += 0xC8000000U;
			}

			Result = ((IValue + 0x0FFFU + ((IValue >> 13U) & 1U)) >> 13U) & 0x7FFFU;
		}
		return (unsigned short)(Result | Sign);
	}
	FORCEINLINE MWDREAL HalfToFloat(unsigned short Value)
	{
		unsigned int Mantissa;
		unsigned int Exponent;
		unsigned int Result;

		Mantissa = (unsigned int)(Value & 0x03FF);

		if ((Value & 0x7C00) != 0)  // The value is normalized
		{
			Exponent = (unsigned int)((Value >> 10) & 0x1F);
		}
		else if (Mantissa != 0)     // The value is denormalized
		{
			// Normalize the value in the resulting MWDREAL
			Exponent = 1;

			do
			{
				Exponent--;
				Mantissa <<= 1;
			} while ((Mantissa & 0x0400) == 0);

			Mantissa &= 0x03FF;
		}
		else                        // The value is zero
		{
			Exponent = (unsigned int)-112;
		}

		Result = ((Value & 0x8000) << 16) | // Sign
			((Exponent + 112) << 23) | // Exponent
			(Mantissa << 13);          // Mantissa

		return *(MWDREAL*)&Result;
	}
	//f must [0,1]，区间映射
	FORCEINLINE unsigned int CompressUnitFloat(MWDREAL f, unsigned int Bit = 16)
	{
		unsigned int nIntervals = 1 << Bit;
		MWDREAL scaled = f * (nIntervals - 1);
		unsigned int rounded = (unsigned int)(scaled + 0.5f);
		if (rounded > nIntervals - 1)
		{
			rounded = nIntervals - 1;
		}
		return rounded;

	}
	FORCEINLINE unsigned int CompressFloat(MWDREAL f, MWDREAL Max, MWDREAL Min, unsigned int Bit = 16)
	{
		MWDREAL Unitf = (f - Min) / (Max - Min);
		return CompressUnitFloat(Unitf, Bit);
	}
	//反区间映射
	FORCEINLINE MWDREAL DecompressUnitFloat(unsigned int quantized, unsigned int Bit = 16)
	{
		unsigned int nIntervals = 1 << Bit;
		MWDREAL IntervalSize = 1.0f / (nIntervals - 1);

		return quantized * IntervalSize;
	}
	FORCEINLINE MWDREAL DecompressFloat(unsigned int quantized, MWDREAL Max, MWDREAL Min, unsigned int Bit = 16)
	{
		MWDREAL Unitf = DecompressUnitFloat(quantized, Bit);
		return (Min + Unitf * (Max - Min));
	}
	FORCEINLINE  MWDREAL ClampAxis(MWDREAL Angle)
	{
		// returns Angle in the range (-360,360)
		Angle = FMOD(Angle, 360.f);

		if (Angle < 0.f)
		{
			// shift to [0,360) range
			Angle += 360.f;
		}

		return Angle;
	}
	FORCEINLINE  MWDREAL NormalizeAxis(MWDREAL Angle)
	{
		// returns Angle in the range [0,360)
		Angle = ClampAxis(Angle);

		if (Angle > 180.f)
		{
			// shift to (-180,180]
			Angle -= 360.f;
		}

		return Angle;
	}
	//-360.....360 can use
	FORCEINLINE MWDREAL ClampAngle(MWDREAL AngleDegrees, MWDREAL MinAngleDegrees, MWDREAL MaxAngleDegrees)
	{
		MWDREAL MaxDelta = ClampAxis(MaxAngleDegrees - MinAngleDegrees) * 0.5f;			// 0..180
		MWDREAL RangeCenter = ClampAxis(MinAngleDegrees + MaxDelta);						// 0..360
		MWDREAL DeltaFromCenter = NormalizeAxis(AngleDegrees - RangeCenter);				// -180..180

		// maybe clamp to nearest edge
		if (DeltaFromCenter > MaxDelta)
		{
			return NormalizeAxis(RangeCenter + MaxDelta);
		}
		else if (DeltaFromCenter < -MaxDelta)
		{
			return NormalizeAxis(RangeCenter - MaxDelta);
		}

		// already in range, just return it
		return NormalizeAxis(AngleDegrees);
	}

	#pragma region 获取int和float类型的随机数
	FORCEINLINE int MWDRand() { return rand(); }
	FORCEINLINE void MWDRandInit(int Seed) { srand(Seed); }
	FORCEINLINE float MWDFRand() { return MWDRand() / (float)RAND_MAX; }
	#pragma endregion	

	const unsigned int CRC32Table[] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
	};

	bool MWDMATH_API MWDInitMath();
	void MWDMATH_API MWDInitCRCTable();
	FORCEINLINE MWDREAL MWDMATH_API GetFastSin(unsigned int i);
	FORCEINLINE MWDREAL MWDMATH_API GetFastCos(unsigned int i);
	unsigned int MWDMATH_API CRC32Compute(const void* pData, unsigned int uiDateSize);
	unsigned int inline MWDMATH_API MWDNameToID(unsigned char* buf, int nLength)
	{
		if (nLength < 1)
			return 0xffffffff;

		unsigned int crc = 0;

		for (int i(0); i != nLength; ++i)
		{
			crc = CRC32Table[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
		}
		crc = crc ^ 0xffffffff;
		return crc;
	}
	FORCEINLINE MWDMATH_API unsigned int Rounding(MWDREAL fNum);
	extern MWDREAL FastSin[361];
	extern MWDREAL FastCos[361];

}

