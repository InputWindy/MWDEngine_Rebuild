#pragma once
#include <MWDMemManager.h>
#include "../Graphic/Core/MWDObject.h"
namespace MWDEngine {

	//使用方法：实例化（填写buffer存储的数据类型），然后一直AddData即可
	class MWDDataBuffer:public MWDObject
	{
		friend class MWDVertexBuffer;
	public:
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

		DECLARE_RTTI_NoCreateFun(MWDDataBuffer, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDDataBuffer)
	protected:
		void*				m_buffer;					//Byte流
		UINT				m_type;						//数据类型
		UINT				m_byteLength;				//Byte数
		UINT				m_num;						//顶点数

		//当前数据类型步长
		static inline UINT	ms_DataTypeByte[DataType_MAXNUM] = {
			4,8,12,16,
			2,4,
			4,4,
			4,4,4,
			8,8,8,
			4,8,
			4
		};       
		//当前数据类型通道数
		static inline UINT	ms_DataTypeChannel[DataType_MAXNUM] = {
			1,2,3,4,
			1,1,
			4,4,
			2,2,2,
			4,4,4,
			2,4,
			4

		};

	public :
		virtual ~MWDDataBuffer() { 
			MWDMAC_DELETEA(m_buffer);
		};
		MWDDataBuffer(UINT data_type) {
			m_type = data_type;
			m_buffer = NULL;
			m_num = 0;
			m_byteLength = 0;
		}
		void operator+=(const MWDDataBuffer& buf) {
			AddData(buf.m_buffer,buf.m_num,buf.m_type);
		}

		//开辟空间（不能为0）
		bool CreateEmptyBuffer(UINT num) {
			if (m_type >= DataType_MAXNUM || !num)
				return 0;

			m_num = num;
			MWDMAC_DELETEA(m_buffer);

			m_buffer = new unsigned char[GetSize()];

			if (!m_buffer)
				return 0;
			MWDMemset(m_buffer, 0, GetSize());
			m_byteLength = GetSize();
			return 1;

		};

		//覆盖掉原来的数据（连数据类型都改变了）
		void SetData(void* buf, UINT num, UINT type) {
			if (m_type >= DataType_MAXNUM || !buf || !num) {
				//cout << "buf=" << buf << endl;
				return ;
			}
			MWDMAC_DELETEA(m_buffer);
			m_type = type;
			m_num = num;
			m_buffer = new unsigned char[GetSize()];

			if (!m_buffer)
				return ;
			MWDMemcpy(m_buffer, buf, GetSize());
			m_byteLength = GetSize();
			return ;

		};


		//在原来基础上追加数据，类型要相同（不能追加0个数据）
		void AddData(void* buf, UINT num, UINT type) {
			//不能追加空数据
			if (type >= DataType_MAXNUM || !buf || !num) {
				cout << "error:1" << endl;
					return ;
			}
			//不能追加不同类型数据
			if (m_type != MWDMAX_INTEGER && m_type != type)
			{
				cout << "error:2" << endl;
				return;
			}
			//如果当前DataBuffer是空的，就创建num个空间
			if (!m_buffer) {
				CreateEmptyBuffer(num);
				SetData(buf,num,m_type);
				return;
			}
			unsigned char* Temp = NULL;
			Temp = new unsigned char[(num + m_num) * GetStride()];
			if (!Temp)
			{
				cout << "error:3" << endl;
				return;
			}
			MWDMemcpy(Temp, m_buffer, GetStride() * m_num);
			MWDMemcpy(Temp + GetStride() * m_num, buf, GetStride() * num);
			MWDMAC_DELETEA(m_buffer);
			m_num += num;
			m_buffer = Temp;
			m_byteLength = GetSize();

		};

		//获取数据缓存
		void* GetData() {
			return m_buffer;
		};
		//获取顶点数
		unsigned int GetNum() {
			return m_num;
		}; 
		//返回数据类型
		unsigned int GetDataType() {
			return m_type;
		};
		//获取步长(一个顶点的Byte长度)
		unsigned int GetStride() {
			return ms_DataTypeByte[m_type];
		};
		//获取通道数
		unsigned int GetChannel() {
			return ms_DataTypeChannel[m_type];
		};
		//获取第i个顶点的数据地址
		void* GetOneVertexData(UINT i,int& byte_size) {
			byte_size = GetStride();
			return (char*)m_buffer + GetStride() * i;
		};
		//获取总Byte长度
		unsigned int GetSize() { 
			return GetStride() * m_num;
		}

		void PrintUINT() {
			for(int i=0;i<GetNum();++i){
				cout << *((unsigned int*)m_buffer + i * sizeof(unsigned int)) << " ";
			}
		}
		void PrintFloat() {
			for (int i = 0; i < GetNum(); ++i) {
				cout << *((float*)m_buffer + i * sizeof(float)) << " ";
			}
		}
	};
	DECLARE_Ptr(MWDDataBuffer);
	MWDTYPE_MARCO(MWDDataBuffer);
}


