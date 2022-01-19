#pragma once
#include "../MWDApplication/Graphic/Core/MWDGraphic.h"
#include "../MWDApplication/Graphic/Core/MWDObject.h"
#include "../MWDApplication/Graphic/Core/MWDName.h"
#include "../MWDApplication/Graphic/äÖÈ¾Æ÷/MWDBind.h"
#include "../MWDApplication/MWDTexture.h"
namespace MWDEngine {
	class  MWDShaderSampler2D : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDShaderSampler2D)
		DECLARE_RTTI(MWDShaderSampler2D, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDShaderSampler2D)
	public:
		MWDShaderSampler2D(const MWDName& Name, unsigned int uiTexType, unsigned int uiRegisterIndex, unsigned int uiRegisterNum);
		virtual ~MWDShaderSampler2D();

		FORCEINLINE unsigned int GetTexType()const
		{
			return m_uiTexType;
		}
		FORCEINLINE MWDTexture2D* GetTex(unsigned int uiIndex)const
		{
			return m_pTextureArray[uiIndex];
		}
		FORCEINLINE const MWDName& GetName()const
		{
			return m_Name;
		}
		FORCEINLINE void SetTexture(MWDTexture2D* pTex, unsigned int uiIndex)
		{
			if (uiIndex >= m_uiRegisterNum)
			{
				return;
			}
			m_pTextureArray[uiIndex] = pTex;
		}
		FORCEINLINE unsigned int GetRegisterIndex()const
		{
			return m_uiRegisterIndex;
		}
		FORCEINLINE unsigned int GetRegisterNum()const
		{
			return m_uiRegisterNum;
		}

	protected:
		friend class MWDRenderer;
		MWDShaderSampler2D();

	private:

		unsigned int				m_uiTexType;
		MWDArray<MWDTexture2DPtr> m_pTextureArray;
		MWDName						m_Name;
		unsigned int				m_uiRegisterIndex;
		unsigned int				m_uiRegisterNum;
	};
	MWDTYPE_MARCO(MWDShaderSampler2D);
	DECLARE_Ptr(MWDShaderSampler2D);
}

