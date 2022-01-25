#pragma once
#include "MWDTriangleMeshData.h"
namespace MWDEngine {

	//计算并填充基本集合体的Mesh索引，顶点坐标
	class MWDBasicGeometryTriangleMeshData : public MWDTriangleMeshData
	{
		DECLARE_CLASS_FUNCTION(MWDBasicGeometryTriangleMeshData)
		DECLARE_RTTI(MWDBasicGeometryTriangleMeshData, MWDMeshData)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDBasicGeometryTriangleMeshData)
	public:
		MWDBasicGeometryTriangleMeshData(const MWDAABB3& AABB) {
			MWDVector3 VertexData[8];
			MWD::GetPoint(AABB, VertexData);
			MWDDataBuffer* pVertex = new MWDDataBuffer();
			pVertex->SetData(VertexData, 8, MWDDataBuffer::DataType_FLOAT32_3);
			m_pVertexBuffer =new MWDVertexBuffer();
			m_pVertexBuffer->SetData(pVertex, MWDVertexFormat::VF_POSITION);
			
			MWDUSHORT_INDEX Index[36];
			//前面的2个三角形
			Index[0] = 0;  Index[1] = 1;  Index[2] = 2;
			Index[3] = 0;  Index[4] = 2;  Index[5] = 3;

			//后面的2个三角形
			Index[6] = 4;   Index[7] = 6;   Index[8] = 5;
			Index[9] = 4;   Index[10] = 7;  Index[11] = 6;

			//右侧面的2个三角形
			Index[12] = 0;  Index[13] = 3;  Index[14] = 7;
			Index[15] = 0;  Index[16] = 7;  Index[17] = 4;
			//左侧面的2个三角形
			Index[18] = 1;  Index[19] = 5;  Index[20] = 6;
			Index[21] = 1;  Index[22] = 6;  Index[23] = 2;
			//顶面的2个三角形	
			Index[24] = 0;  Index[25] = 5;  Index[26] = 1;
			Index[27] = 0;  Index[28] = 4;  Index[29] = 5;
			//底面的2个三角形
			Index[30] = 2;  Index[31] = 6;  Index[32] = 7;
			Index[33] = 2;  Index[34] = 7;  Index[35] = 3;

			MWDDataBuffer* pIndexDataBuffer = new MWDDataBuffer();
			pIndexDataBuffer->SetData(&Index[0], 36, MWDDataBuffer::DataType_USHORT);
			if (!m_pIndexBuffer)
			{
				m_pIndexBuffer =new MWDIndexBuffer();
			}
			m_pIndexBuffer->SetData(pIndexDataBuffer);
		};
		MWDBasicGeometryTriangleMeshData(const MWDOBB3& OBB) {
			MWDVector3 VertexData[8];
			MWD::GetPoint(OBB, VertexData);
			MWDDataBuffer* pVertex = new MWDDataBuffer;
			pVertex->SetData(VertexData, 8, MWDDataBuffer::DataType_FLOAT32_3);
			m_pVertexBuffer = new MWDVertexBuffer();
			m_pVertexBuffer->SetData(pVertex, MWDVertexFormat::VF_POSITION);
			MWDUSHORT_INDEX Index[36];
			//前面的2个三角形
			Index[0] = 0;  Index[1] = 1;  Index[2] = 2;
			Index[3] = 0;  Index[4] = 2;  Index[5] = 3;

			//后面的2个三角形
			Index[6] = 4;   Index[7] = 6;   Index[8] = 5;
			Index[9] = 4;   Index[10] = 7;  Index[11] = 6;

			//右侧面的2个三角形
			Index[12] = 0;  Index[13] = 3;  Index[14] = 7;
			Index[15] = 0;  Index[16] = 7;  Index[17] = 4;
			//左侧面的2个三角形
			Index[18] = 1;  Index[19] = 5;  Index[20] = 6;
			Index[21] = 1;  Index[22] = 6;  Index[23] = 2;
			//顶面的2个三角形	
			Index[24] = 0;  Index[25] = 5;  Index[26] = 1;
			Index[27] = 0;  Index[28] = 4;  Index[29] = 5;
			//底面的2个三角形
			Index[30] = 2;  Index[31] = 6;  Index[32] = 7;
			Index[33] = 2;  Index[34] = 7;  Index[35] = 3;

			MWDDataBuffer* pIndexDataBuffer = new MWDDataBuffer;
			pIndexDataBuffer->SetData(Index, 36, MWDDataBuffer::DataType_USHORT);

			if (!m_pIndexBuffer)
			{
				m_pIndexBuffer =new MWDIndexBuffer();

			}

			m_pIndexBuffer->SetData(pIndexDataBuffer);
		};
		MWDBasicGeometryTriangleMeshData(const MWDSphere3& Sphere) {
			MWDREAL LongitudeAngle = 10;//经度线上的划分角
			MWDREAL LatitudeAngle = 10;//维度线上的划分角

			MWDREAL AnglePI = 180.0f;
			unsigned int LongitudeNum = 0;
			while (AnglePI > 0)
			{
				LongitudeNum++;
				AnglePI -= LongitudeAngle;
			}
			LongitudeNum++;
			unsigned int LatitudeNum = unsigned int(360.0f / LatitudeAngle) + 1;

			MWDDataBuffer* pVertex = new MWDDataBuffer();
			pVertex->CreateEmptyBuffer(LongitudeNum * LatitudeNum, MWDDataBuffer::DataType_FLOAT32_3);

			MWDVector3* pVertexData = (MWDVector3*)pVertex->GetData();
			MWDREAL i = 0;
			for (unsigned int k = 0; k < LongitudeNum; k++)
			{
				if (i > 180)
					i = 180;
				MWDREAL iRadian = AngleToRadian(i);
				for (MWDREAL j = 0; j <= 360.0f; j = j + LatitudeAngle)
				{


					MWDREAL jRadian = AngleToRadian(j);
					pVertexData->x = Sphere.m_fRadius * SIN(iRadian) * COS(jRadian);
					pVertexData->z = Sphere.m_fRadius * SIN(iRadian) * SIN(jRadian);
					pVertexData->y = Sphere.m_fRadius * COS(iRadian);

					*pVertexData += Sphere.m_Center;
					pVertexData++;
				}
				i = i + LongitudeAngle;
			}
			m_pVertexBuffer =new MWDVertexBuffer();
			m_pVertexBuffer->SetData(pVertex, MWDVertexFormat::VF_POSITION);

			MWDDataBuffer* pIndexDataBuffer = new MWDDataBuffer;
			pIndexDataBuffer->CreateEmptyBuffer((LongitudeNum - 1) * LatitudeNum * 2 * 3, MWDDataBuffer::DataType_USHORT);
			MWDUSHORT_INDEX* pIndexData = (MWDUSHORT_INDEX*)pIndexDataBuffer->GetData();
			for (unsigned int i = 0; i < LongitudeNum - 1; i++)
			{
				unsigned int z_width_poly = i * LatitudeNum;
				unsigned int z_width = i * LatitudeNum;
				for (unsigned int j = 0; j < LatitudeNum; j++)
				{
					unsigned int z_width_x = z_width + j;
					unsigned int z_width_xX2 = (z_width_poly + j) * 2;
					if (j != LatitudeNum - 1)
					{
						pIndexData[z_width_xX2 * 3 + 0] = z_width_x;
						pIndexData[z_width_xX2 * 3 + 2] = z_width_x + LatitudeNum;
						pIndexData[z_width_xX2 * 3 + 1] = z_width_x + 1;

						pIndexData[(z_width_xX2 + 1) * 3 + 0] = z_width_x + 1 + LatitudeNum;
						pIndexData[(z_width_xX2 + 1) * 3 + 2] = z_width_x + 1;
						pIndexData[(z_width_xX2 + 1) * 3 + 1] = z_width_x + LatitudeNum;
					}
					else
					{
						pIndexData[z_width_xX2 * 3 + 0] = z_width_x;
						pIndexData[z_width_xX2 * 3 + 2] = z_width_x + LatitudeNum;
						pIndexData[z_width_xX2 * 3 + 1] = z_width_x - LatitudeNum + 1;

						pIndexData[(z_width_xX2 + 1) * 3 + 0] = z_width_x + 1;
						pIndexData[(z_width_xX2 + 1) * 3 + 2] = z_width_x - LatitudeNum + 1;
						pIndexData[(z_width_xX2 + 1) * 3 + 1] = z_width_x + LatitudeNum;
					}

				}
			}

			if (!m_pIndexBuffer)
			{
				m_pIndexBuffer =new MWDIndexBuffer();

			}

			m_pIndexBuffer->SetData(pIndexDataBuffer);
		};
		MWDBasicGeometryTriangleMeshData(const MWDPlane3& Plane) {
			MWDMatrix3X3W Mat;
			Mat.CreateFromLookDir(Plane.GetPoint(), Plane.GetN());

			MWDVector3 A0(Mat.M[0][0], Mat.M[0][1], Mat.M[0][2]);
			MWDVector3 A1(Mat.M[1][0], Mat.M[1][1], Mat.M[1][2]);

			MWDVector3 VertexData[4];
			VertexData[0] = Plane.GetPoint() + A0 * DRAW_MATH_ELEMENT_LENGTH + A1 * DRAW_MATH_ELEMENT_LENGTH;
			VertexData[1] = Plane.GetPoint() - A0 * DRAW_MATH_ELEMENT_LENGTH + A1 * DRAW_MATH_ELEMENT_LENGTH;
			VertexData[2] = Plane.GetPoint() - A0 * DRAW_MATH_ELEMENT_LENGTH - A1 * DRAW_MATH_ELEMENT_LENGTH;
			VertexData[3] = Plane.GetPoint() + A0 * DRAW_MATH_ELEMENT_LENGTH - A1 * DRAW_MATH_ELEMENT_LENGTH;

			MWDDataBuffer* pVertex = new MWDDataBuffer;
			pVertex->SetData(VertexData, 4, MWDDataBuffer::DataType_FLOAT32_3);
			m_pVertexBuffer =new MWDVertexBuffer();
			m_pVertexBuffer->SetData(pVertex, MWDVertexFormat::VF_POSITION);

			MWDUSHORT_INDEX Index[6];

			Index[0] = 0;  Index[1] = 1;  Index[2] = 2;
			Index[3] = 0;  Index[4] = 2;  Index[5] = 3;

			MWDDataBuffer* pIndexDataBuffer = new MWDDataBuffer;
			pIndexDataBuffer->SetData(Index, 6, MWDDataBuffer::DataType_USHORT);

			if (!m_pIndexBuffer)
			{
				m_pIndexBuffer =new MWDIndexBuffer();

			}

			m_pIndexBuffer->SetData(pIndexDataBuffer);
		};
		MWDBasicGeometryTriangleMeshData(const MWDTriangle3& Triangle) {
			MWDVector3 VertexData[3];
			Triangle.GetPoint(VertexData);
			MWDDataBuffer* pVertex = new MWDDataBuffer;
			pVertex->SetData(VertexData, 3, MWDDataBuffer::DataType_FLOAT32_3);
			m_pVertexBuffer = new MWDVertexBuffer();
			m_pVertexBuffer->SetData(pVertex, MWDVertexFormat::VF_POSITION);

			MWDUSHORT_INDEX Index[3];
			Index[0] = 0;  Index[1] = 1;  Index[2] = 2;
			MWDDataBuffer* pIndexDataBuffer = new MWDDataBuffer;
			pIndexDataBuffer->SetData(Index, 3, MWDDataBuffer::DataType_USHORT);

			if (!m_pIndexBuffer)
			{
				m_pIndexBuffer = new MWDIndexBuffer();

			}

			m_pIndexBuffer->SetData(pIndexDataBuffer);
		};
		MWDBasicGeometryTriangleMeshData(const MWDRectangle3& Rectangle) {
			MWDVector3 VertexData[4];
			Rectangle.GetPoint(VertexData);
			MWDDataBuffer* pVertex = new MWDDataBuffer;
			pVertex->SetData(VertexData, 4, MWDDataBuffer::DataType_FLOAT32_3);
			m_pVertexBuffer = new MWDVertexBuffer();
			m_pVertexBuffer->SetData(pVertex, MWDVertexFormat::VF_POSITION);

			MWDUSHORT_INDEX Index[6];

			Index[0] = 0;  Index[1] = 2;  Index[2] = 1;
			Index[3] = 0;  Index[4] = 3;  Index[5] = 2;

			MWDDataBuffer* pIndexDataBuffer = new MWDDataBuffer;
			pIndexDataBuffer->SetData(Index, 6, MWDDataBuffer::DataType_USHORT);

			if (!m_pIndexBuffer)
			{
				m_pIndexBuffer = new MWDIndexBuffer();

			}

			m_pIndexBuffer->SetData(pIndexDataBuffer);
		};
		MWDBasicGeometryTriangleMeshData(const MWDPolygon3& Polyon) {
			MWDMAC_ASSERT(Polyon.GetpPoint() && Polyon.GetPointNum());

			MWDDataBuffer* pVertex = new MWDDataBuffer;
			MWDVector3 tmp = *Polyon.GetpPoint();
			int buf[3] = { tmp.x,tmp.y,tmp.z };
			pVertex->SetData(&buf[0], Polyon.GetPointNum(), MWDDataBuffer::DataType_FLOAT32_3);
			m_pVertexBuffer = new MWDVertexBuffer(); 
			m_pVertexBuffer->SetData(pVertex, MWDVertexFormat::VF_POSITION);


			MWDDataBuffer* pIndexDataBuffer = new MWDDataBuffer;
			pIndexDataBuffer->SetData((void*)Polyon.GetpIndex(), Polyon.GetIndexNum(), MWDDataBuffer::DataType_USHORT);

			if (!m_pIndexBuffer)
			{
				m_pIndexBuffer = new MWDIndexBuffer();

			}

			m_pIndexBuffer->SetData(pIndexDataBuffer);
		};
		virtual ~MWDBasicGeometryTriangleMeshData() {

		};
	protected:
		MWDBasicGeometryTriangleMeshData() {

		};
	};
	DECLARE_Ptr(MWDBasicGeometryTriangleMeshData);
	MWDTYPE_MARCO(MWDBasicGeometryTriangleMeshData);
}


