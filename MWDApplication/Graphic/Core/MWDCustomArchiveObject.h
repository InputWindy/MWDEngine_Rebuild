#pragma once
#include "MWDGraphic.h"
#include "../MWDDataStruct/MWDString.h"
#include "MWDType.h"
#include "../MWDDataStruct/MWDMap.h"
namespace MWDEngine {
	/*class MWDStream;
	class MWDEditorElement;*/

	//�Զ���浵Obj
	class MWDCustomArchiveObject :public MWDMemObject
	{
	public:
		MWDCustomArchiveObject() {};
		virtual ~MWDCustomArchiveObject() {};
		virtual void Archive(MWDStream& Stream) {};

		////CustomType
		//virtual void CopyFrom(MWDCustomArchiveObject*, MWDMap<MWDObject*, MWDObject*>& CloneMap) = 0;
		////δʵ�ֵ�API
		//virtual MWDEditorElement* CreateEElement(MWDString& Name, MWDObject* pOwner) { return NULL; };
	};
	CUSTOMTYPE_MARCO(MWDCustomArchiveObject)
}
