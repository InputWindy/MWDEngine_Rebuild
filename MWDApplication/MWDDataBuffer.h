#pragma once
#include <MWDMemManager.h>
#include "../Graphic/Core/MWDObject.h"
namespace MWDEngine {
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

		DECLARE_CLASS_FUNCTION(MWDDataBuffer)
		DECLARE_RTTI(MWDDataBuffer, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDDataBuffer)
	protected:
		void*				m_buffer;					//Byte��
		UINT				m_type;						//��������
		UINT				m_byteLength;				//Byte��
		UINT				m_num;						//������

		//��ǰ�������Ͳ���
		static inline UINT	ms_DataTypeByte[DataType_MAXNUM] = {
			4,8,12,16,
			2,4,
			4,4,
			4,4,4,
			8,8,8,
			4,8,
			4
		};       
		//��ǰ��������ͨ����
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

		MWDDataBuffer() {
			m_buffer = NULL;
		}

		bool CreateEmptyBuffer(UINT num, UINT data_type) {
			if (m_type >= DataType_MAXNUM || !num)
				return 0;

			m_type = data_type;
			m_num = num;
			MWDMAC_DELETEA(m_buffer);

			m_buffer = new unsigned char[GetSize()];

			if (!m_buffer)
				return 0;
			MWDMemset(m_buffer, 0, GetSize());
			m_byteLength = GetSize();
			return 1;

		};
		void SetData(void* buf, UINT num, UINT type) {
			if (m_type >= DataType_MAXNUM || !buf || !num) {
				//cout << "buf=" << buf << endl;
				return ;
			}
				
			m_type = type;
			m_num = num;

			MWDMAC_DELETEA(m_buffer);

			m_buffer = new unsigned char[GetSize()];

			if (!m_buffer)
				return ;
			MWDMemcpy(m_buffer, buf, GetSize());
			m_byteLength = GetSize();
			return ;

		};
		void AddData(void* buf, UINT num, UINT type) {
			if (type >= DataType_MAXNUM || !buf || !num)
				return ;
			if (m_type != MWDMAX_INTEGER && m_type != type)
				return ;


			unsigned char* Temp = NULL;
			Temp = new unsigned char[(num + m_num) * GetStride()];
			if (!Temp)
				return ;
			MWDMemcpy(Temp, m_buffer, GetStride() * m_num);
			MWDMemcpy(Temp + GetStride() * m_num, m_buffer, GetStride() * num);

			MWDMAC_DELETEA(m_buffer);
			m_num += num;
			m_buffer = Temp;
			m_byteLength = GetSize();

		};

		//��ȡ���ݻ���
		void* GetData() {
			return m_buffer;
		};
		//��ȡ������
		unsigned int GetNum() {
			return m_num;
		}; 
		//������������
		unsigned int GetDataType() {
			return m_type;
		};
		//��ȡ����
		unsigned int GetStride() {
			return ms_DataTypeByte[m_type];
		};
		//��ȡͨ����
		unsigned int GetChannel() {
			return ms_DataTypeChannel[m_type];
		};
		//��ȡ��i�����������
		void* GetOneVertexData(UINT i) {
			char* ret = new char[GetStride()];
			MWDMemcpy(ret,&((char*)m_buffer)[GetStride()*i] , GetStride());
			return ret;

		};
		//buffer���ݸ���
		unsigned int GetNum()const {
			return m_num; 
		}
		//��ȡ��Byte����
		unsigned int GetSize() { 
			return GetStride() * m_num;
		}


	};
	DECLARE_Ptr(MWDDataBuffer);
	MWDTYPE_MARCO(MWDDataBuffer);
}


