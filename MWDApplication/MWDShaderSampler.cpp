#include "MWDShaderSampler.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDShaderSampler)
    ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDShaderSampler)

    BEGIN_ADD_PROPERTY(MWDShaderSampler)
    REGISTER_PROPERTY(m_uiTexType, TexType, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_Name, Name, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiRegisterIndex, RegisterIndex, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiRegisterNum, RegisterNum, MWDProperty::F_SAVE_LOAD_CLONE)
    END_ADD_PROPERTY

    MWDShaderSampler::MWDShaderSampler(const MWDName& Name, unsigned int uiTexType, unsigned int uiRegisterNum, unsigned int uiRegisterIndex) {
        m_Name = Name;
        m_uiTexType = uiTexType;
        m_uiRegisterIndex = uiRegisterIndex;
        m_uiRegisterNum = uiRegisterNum;
        m_pTextureArray.ResetBufferNum(uiRegisterNum);
    };

    MWDShaderSampler::~MWDShaderSampler() {
        m_pTextureArray.Destroy();
    }
    MWDShaderSampler::MWDShaderSampler()
    {
        m_uiTexType = MWDTexture::TT_2D;
        m_pTextureArray.Clear();
        m_uiRegisterNum = 1;
    }
    ;
}