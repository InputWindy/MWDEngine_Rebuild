#pragma once
#include "../MWDBind.h"
#include "MWDTexture.h"
namespace MWDEngine {
	//维护一组Texture
	class  MWDShaderSampler : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDShaderSampler)
		DECLARE_RTTI(MWDShaderSampler, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDShaderSampler)
	public:
		//uiRegisterNum是texture通道数：默认是1通道
		MWDShaderSampler(const MWDName& Name, unsigned int uiTexType, unsigned int uiRegisterNum = 1, unsigned int uiRegisterIndex = 0);
		virtual ~MWDShaderSampler();

		FORCEINLINE unsigned int GetTexType()const
		{
			return m_uiTexType;
		}
		FORCEINLINE MWDTexture* GetTex(unsigned int uiIndex = 0)const
		{
			return m_pTextureArray[uiIndex];
		}
		FORCEINLINE const MWDName& GetName()const
		{
			return m_Name;
		}
		FORCEINLINE void SetTexture(MWDTexture* pTex, unsigned int uiIndex)
		{
			if (uiIndex >= m_uiRegisterNum)
			{
				return;
			}
			m_pTextureArray[uiIndex] = pTex;
		}
		//相当于texture数目
		FORCEINLINE unsigned int GetRegisterIndex()const
		{
			return m_uiRegisterIndex;
		}
		//相当于texture总数
		FORCEINLINE unsigned int GetRegisterNum()const
		{
			return m_uiRegisterNum;
		}

	protected:

		friend class MWDRenderer;
		MWDShaderSampler();

	private:

		unsigned int				m_uiTexType;
		MWDArray<MWDTexture*>		m_pTextureArray;
		MWDName						m_Name;
		unsigned int				m_uiRegisterIndex;
		unsigned int				m_uiRegisterNum;
	};
	MWDTYPE_MARCO(MWDShaderSampler);
	DECLARE_Ptr(MWDShaderSampler);
}



