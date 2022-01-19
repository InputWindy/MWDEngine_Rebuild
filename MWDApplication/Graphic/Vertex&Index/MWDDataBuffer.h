#pragma once
#include "../MWDApplication/Graphic/Core/MWDGraphic.h"
#include "../MWDApplication/Graphic/Core/MWDObject.h"
#include <MWDCompute.h>
namespace MWDEngine {

	//维护一个Mesh里的一种顶点属性的全部值
	class  MWDDataBuffer : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDDataBuffer)
		DECLARE_RTTI(MWDDataBuffer, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDDataBuffer)
	public:
		friend class MWDIndexBuffer;
		enum	//Data Type
		{
			DataType_FLOAT32_1,// 1D MWDREAL expanded to (value, 0., 0., 1.)
			DataType_FLOAT32_2,// 2D MWDREAL expanded to (value, value, 0., 1.)
			DataType_FLOAT32_3,// 3D MWDREAL expanded to (value, value, value, 1.)
			DataType_FLOAT32_4,// 4D MWDREAL

			DataType_USHORT,
			DataType_UINT,

			DataType_UBYTE4,	// 4D unsigned byte
			DataType_UBYTE4N, // Each of 4 bytes is normalized by dividing to 255.0

			DataType_SHORT2,	// 2D signed short expanded to (value, value, 0., 1.)
			DataType_SHORT2N,	// 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
			DataType_USHORT2N,// 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)

			DataType_SHORT4,	// 4D signed short
			DataType_SHORT4N,	// 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
			DataType_USHORT4N,// 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)

			DataType_FLOAT16_2,// Two 16-bit floating point values, expanded to (value, value, 0, 1)
			DataType_FLOAT16_4,// Four 16-bit floating point values

			DataType_COLOR,	// 4D packed unsigned bytes mapped to 0. to 1. range
						// Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
						DataType_MAXNUM
		};
		virtual ~MWDDataBuffer() { MWDMAC_DELETEA(m_pData); };
		MWDDataBuffer() {
			m_uiSize = 0;
			m_uiDataType = DataType_MAXNUM;
			m_uiNum = 0;
			m_pData = NULL;
		};
	public:
		char* GetOneVertexData(int i) {
			char* ret = new char[GetStride()];
			MWDMemcpy(ret, (void*)m_pData[i* GetStride()], GetStride());
			return ret;
		}
		//buffer数据类型
		FORCEINLINE unsigned int GetDataType()const { return m_uiDataType; }
		//buffer步长
		FORCEINLINE unsigned int GetStride()const { return ms_uiDataTypeByte[m_uiDataType]; }
		//buffer通道数量
		FORCEINLINE unsigned int GetChannel()const { return ms_uiDataTypeChannel[m_uiDataType]; }
		//buffer数据个数
		FORCEINLINE unsigned int GetNum()const { return m_uiNum; }
		//buffer字节数
		FORCEINLINE unsigned int GetSize()const { return GetStride() * m_uiNum; }
		//返回buffer
		FORCEINLINE void* GetData()const { return m_pData; }
		//填充buffer数据
		bool SetData(const void* pData, unsigned int uiNum, unsigned int uiDataType) {
			if (uiDataType >= DataType_MAXNUM || !pData || !uiNum)
				return 0;

			m_uiDataType = uiDataType;
			m_uiNum = uiNum;

			MWDMAC_DELETEA(m_pData);

			m_pData = new unsigned char[GetSize()];

			if (!m_pData)
				return 0;
			MWDMemcpy(m_pData, pData, GetSize());
			m_uiSize = GetSize();
			return 1;
		};
		//接续buffer数据
		bool AddData(const void* pData, unsigned int uiNum, unsigned int uiDataType) {
			if (uiDataType >= DataType_MAXNUM || !pData || !uiNum)
				return 0;
			if (m_uiDataType != MWDMAX_INTEGER && m_uiDataType != uiDataType)
				return 0;


			unsigned char* Temp = NULL;
			Temp = new unsigned char[(uiNum + m_uiNum) * GetStride()];
			if (!Temp)
				return 0;
			MWDMemcpy(Temp, m_pData, GetStride() * m_uiNum);
			MWDMemcpy(Temp + GetStride() * m_uiNum, pData, GetStride() * uiNum);

			MWDMAC_DELETEA(m_pData);
			m_uiNum += uiNum;
			m_pData = Temp;
			m_uiSize = GetSize();
			return 1;
		};
		//生成type类型，长度为num的buffer
		bool CreateEmptyBuffer(unsigned int uiNum, unsigned int uiDataType) {
			if (uiDataType >= DataType_MAXNUM || !uiNum)
				return 0;

			m_uiDataType = uiDataType;
			m_uiNum = uiNum;
			MWDMAC_DELETEA(m_pData);

			m_pData = new unsigned char[GetSize()];

			if (!m_pData)
				return 0;
			MWDMemset(m_pData, 0, GetSize());
			m_uiSize = GetSize();
			return 1;
		};
		//步长查找表
		static inline unsigned int ms_uiDataTypeByte[DataType_MAXNUM] = {
			4,8,12,16,
			2,4,
			4,4,
			4,4,4,
			8,8,8,
			4,8,
			4
		};
		//数据类型对应的通道数量
		static inline unsigned int ms_uiDataTypeChannel[DataType_MAXNUM] = {
			1,2,3,4,
			1,1,
			4,4,
			2,2,2,
			4,4,4,
			2,4,
			4
		};
	protected:
		//buffer数据类型
		unsigned int m_uiDataType;
		//buffer数据个数
		unsigned int m_uiNum;
		//buffer
		unsigned char* m_pData;
		//buffer字节数
		unsigned int m_uiSize;

	};
	DECLARE_Ptr(MWDDataBuffer);
	MWDTYPE_MARCO(MWDDataBuffer);

}

