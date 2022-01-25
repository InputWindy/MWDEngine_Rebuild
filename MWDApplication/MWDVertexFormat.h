#pragma once
#include "MWDBind.h"
namespace MWDEngine
{
	//ά��һ�׶������ݽ�����ʽ
	class  MWDVertexFormat : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDVertexFormat)
		DECLARE_RTTI(MWDVertexFormat, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDVertexFormat)
	public:
		//������������
		enum
		{
			VF_POSITION,		//Location=0
			VF_TEXCOORD,		//Location=1
			VF_NORMAL,			//Location=2
			VF_TANGENT,			//Location=3
			VF_BINORMAL,		//Location=4
			VF_PSIZE,			//Location=5
			VF_COLOR,			//Location=6
			VF_FOG,				//Location=7
			VF_DEPTH,			//Location=8
			VF_BLENDWEIGHT,		//Location=9
			VF_BLENDINDICES,	//Location=10
			VF_MAX
		};
		//ά��һ�ֶ������ݣ������������ͣ�offset�����壬����idx��
		struct VERTEXFORMAT_TYPE
		{
		public:
			VERTEXFORMAT_TYPE()
			{
				OffSet = 0;
				DataType = 0;
				Semantics = 0;//����
				SemanticsIndex = 0;
			}
			~VERTEXFORMAT_TYPE()
			{

			}
			UINT OffSet;
			UINT DataType;			//�������ͣ�int��float��double......��
			UINT Semantics;			//�������ͣ�position��normal��texcoord......��
			UINT SemanticsIndex;	//��index�׶�������
		};

	public:
		//ά���������ݵ�����
		MWDArray<MWDVertexFormat::VERTEXFORMAT_TYPE> m_FormatArray;

	};
	DECLARE_Ptr(MWDVertexFormat);
	MWDTYPE_MARCO(MWDVertexFormat);

}


