#include "MWDMeshRenderer.h"
#include "MWDEntity.h"
#include "MWDRenderer.h"
namespace MWDEngine {
	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDMeshRenderer)
	ADD_PRIORITY(MWDComponent)
	INITIAL_WITH_INIT_TERMINAL_END(MWDMeshRenderer)

	BEGIN_ADD_PROPERTY(MWDMeshRenderer)
	END_ADD_PROPERTY

	MWDMeshRenderer::MWDMeshRenderer(MWDEntity* pParent)
	{
		MWDMAC_ASSERT(pParent)
		m_data = NULL;
		m_Material = NULL;
		SetEntity(pParent);

		MWDEntity* parent = GetEntity();
		MWDMeshComponent* mesh_component = parent->GetComponentByType<MWDMeshComponent>();
		MWDMAC_ASSERT(mesh_component)
		m_data = mesh_component->GetMeshData();
		MWDMAC_ASSERT(m_data)
	}

	//向MWDRenderer传递渲染数据，并调用MWDRenderer的DrawMesh进行绘制。绘制结果存储在MWDRenderer的Contex里。
	bool MWDMeshRenderer::Draw(MWDRenderer* pRenderer)
	{
		if (!pRenderer) {
			return false;
		}
		pRenderer->SetRenderCtx(m_data);
		bool flag = false;
		MWDPass mainPass = MWDPass(m_Material->m_RenderState, m_Material, m_Material->m_VShader, m_Material->m_FShader);
		flag = mainPass.Draw(pRenderer);
		if (!flag) {
			return false;
		}

		int pass_num = m_Material->m_PassArr.GetNum();
		for (int i = 0; i < pass_num; ++i) {
			flag = m_Material->m_PassArr[i]->Draw(pRenderer);
			if (!flag) {
				return false;
			}
		}
		return pRenderer->DrawMesh();
	}
}