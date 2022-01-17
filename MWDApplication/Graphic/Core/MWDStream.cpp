
#include "MWDStream.h"
namespace MWDEngine {
	//¶ÁÈ¡ÎÄ¼þ
    bool MWDStream::NewLoad(const TCHAR* const pcFileName)
    {
		MWDFile* pFile = new MWDFile();
		if (!pFile)
		{
			return 0;
		}

		if (!pFile->Open(pcFileName, *_T("r")))
		{
			MWDMAC_DELETE(pFile);
			return 0;
		}

		unsigned int uiBufferSize = pFile->GetFileSize();
		unsigned char* pBuffer = new unsigned char[uiBufferSize];
		if (!pBuffer)
		{
			MWDMAC_DELETE(pFile);
			return 0;
		}
		if (!pFile->Read(pBuffer, uiBufferSize, 1))
		{
			MWDMAC_DELETE(pFile);
			return 0;
		}
		MWDMAC_DELETE(pFile);

		if (!NewLoadFromBuffer(pBuffer, uiBufferSize))
		{
			MWDMAC_DELETEA(pBuffer);
			return false;
		}
		MWDMAC_DELETEA(pBuffer);

		return 1;
    }
	bool MWDStream::NewSave(const TCHAR* const pcFileName)
	{
		unsigned int iObjectNum = m_pVObjectArray.GetNum();
		m_uiBufferSize = 0;

		
		m_uiBufferSize += sizeof(unsigned int);
		
		m_uiBufferSize += sizeof(unsigned int);

		MWDArray<ObjectTable> ObjectTable;
		ObjectTable.ResetBufferNum(iObjectNum);
		// obejct table
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			//addr
			m_uiBufferSize += sizeof(MWDObject*);

			ObjectTable[i].m_pAddr = m_pVObjectArray[i];

			//rtti name
			m_uiBufferSize += GetStrDistUse(m_pVObjectArray[i]->GetType().GetName());

			ObjectTable[i].m_RttiName = m_pVObjectArray[i]->GetType().GetName();

			// object property size
			m_uiBufferSize += sizeof(unsigned int);

			// object property num
			m_uiBufferSize += sizeof(unsigned int);

		}

		unsigned int uiObjectContentAddr = m_uiBufferSize;

		m_uiStreamFlag = AT_SIZE;
		//object property size 
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			ObjectTable[i].m_uiOffSet = m_uiBufferSize;

			MWDRtti& Rtti = m_pVObjectArray[i]->GetType();
			ObjectTable[i].m_ObjectPropertyTable.ResetBufferNum(Rtti.GetPropertyNum());

			ObjectTable[i].m_uiObjectPropertyNum = 0;

			for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
			{
				MWDProperty* pProperty = Rtti.GetProperty(j);

				if (pProperty->GetFlag() & MWDProperty::F_SAVE_LOAD)
				{
					//name
					m_uiBufferSize += sizeof(unsigned int);
					ObjectTable[i].m_ObjectPropertyTable[j].m_PropertyName = pProperty->GetName();
					ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID =MWDNameToID((unsigned char*)pProperty->GetName().GetBuffer(),sizeof(pProperty->GetName().GetLength()));
					//offset
					m_uiBufferSize += sizeof(unsigned int);

					ObjectTable[i].m_uiObjectPropertyNum++;
				}


			}

			ObjectTable[i].m_uiObjectPropertyTableSize = m_uiBufferSize - ObjectTable[i].m_uiOffSet;

			for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
			{
				MWDProperty* pProperty = Rtti.GetProperty(j);

				if (pProperty->GetFlag() & MWDProperty::F_SAVE_LOAD)
				{
					ObjectTable[i].m_ObjectPropertyTable[j].m_uiOffSet = m_uiBufferSize;

					m_uiArchivePropertySize = 0;

					this->Archive(*(MWDObject*)pProperty->GetValue(m_pVObjectArray[i]));

					ObjectTable[i].m_ObjectPropertyTable[j].m_uiSize = m_uiArchivePropertySize;

					m_uiBufferSize += m_uiArchivePropertySize;
				}
			}

			ObjectTable[i].m_uiObjectPropertySize = m_uiBufferSize - ObjectTable[i].m_uiObjectPropertyTableSize - ObjectTable[i].m_uiOffSet;
		}

		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			m_pVObjectArray[i]->BeforeSave();
		}
		MWDMAC_DELETEA(m_pcBuffer);
		m_pcBuffer = new unsigned char[m_uiBufferSize];
		if (!m_pcBuffer)
		{
			return 0;
		}
		m_pcCurBufPtr = m_pcBuffer;

		m_uiVerson = ms_uiCurVerson;
		
		Write(&m_uiVerson, sizeof(unsigned int));

		
		Write(&iObjectNum, sizeof(unsigned int));


		// OBJECT TABLE
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			//addr
			if (!Write(&ObjectTable[i].m_pAddr, sizeof(MWDObject*)))
			{
				MWDMAC_DELETEA(m_pcBuffer);
				return 0;
			}

			//rtti name
			if (!WriteString(ObjectTable[i].m_RttiName))
			{
				MWDMAC_DELETEA(m_pcBuffer);
				return 0;
			}

			Write(&ObjectTable[i].m_uiObjectPropertySize, sizeof(unsigned int));

			Write(&ObjectTable[i].m_uiObjectPropertyNum, sizeof(unsigned int));

		}

		//OBJECT PROPERTY 
		m_uiStreamFlag = AT_SAVE;
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{

			MWDRtti& Rtti = m_pVObjectArray[i]->GetType();
			for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
			{
				MWDProperty* pProperty = Rtti.GetProperty(j);

				if (pProperty->GetFlag() & MWDProperty::F_SAVE_LOAD)
				{

					Write(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID, sizeof(unsigned int));
					Write(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiOffSet, sizeof(unsigned int));
				}

			}

			for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
			{
				MWDProperty* pProperty = Rtti.GetProperty(j);
				if (pProperty->GetFlag() & MWDProperty::F_SAVE_LOAD)
				{
					this->Archive(*(MWDObject*)pProperty->GetValue(m_pVObjectArray[i]));
				}
			}
		}

		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			m_pVObjectArray[i]->PostSave();
		}

		MWDFile* pFile = new MWDFile();
		if (!pFile)
		{
			MWDMAC_DELETEA(m_pcBuffer);
			return 0;
		}
		if (!pFile->Open(pcFileName, *_T("w")))
		{
			MWDMAC_DELETE(pFile);
			MWDMAC_DELETEA(m_pcBuffer);
			return 0;
		}

		if (!pFile->Write(m_pcBuffer, m_uiBufferSize, 1))
		{
			MWDMAC_DELETE(pFile);
			MWDMAC_DELETEA(m_pcBuffer);
			return 0;
		}
		MWDMAC_DELETE(pFile);
		MWDMAC_DELETEA(m_pcBuffer);
		return 1;
	}
	bool MWDStream::NewLoadFromBuffer(unsigned char* pBuffer, unsigned int uiSize)
	{
		if (!pBuffer || !uiSize)
		{
			return false;
		}
		m_pVObjectArray.Clear();
		m_pmLoadMap.Clear();
		m_pcCurBufPtr = NULL;
		m_uiBufferSize = 0;

		
		m_uiBufferSize = uiSize;
		m_pcBuffer = pBuffer;

		m_pcCurBufPtr = m_pcBuffer;

		MWDArray<ObjectTable> ObjectTable;
		
		Read(&m_uiVerson, sizeof(unsigned int));
		
		unsigned int iObjectNum = 0;
		Read(&iObjectNum, sizeof(unsigned int));

		ObjectTable.ResetBufferNum(iObjectNum);
		//Object Table
		for (unsigned int i = 0; i < iObjectNum; i++)
		{
			MWDObject* pObject = 0;
			
			if (!Read(&ObjectTable[i].m_pAddr, sizeof(MWDObject*)))
			{
				return 0;
			}
			
			if (!ReadString(ObjectTable[i].m_RttiName))
			{
				return 0;
			}

			if (!Read(&ObjectTable[i].m_uiObjectPropertySize, sizeof(unsigned int)))
			{
				return 0;
			}

			if (!Read(&ObjectTable[i].m_uiObjectPropertyNum, sizeof(unsigned int)))
			{
				return 0;
			}


		}

		MWDArray<unsigned int> TableID;
		//create object
		for (unsigned int i = 0; i < iObjectNum; i++)
		{
			MWDObject* pObject = NULL;
			if (m_bLoadUseGC)
			{
				pObject = MWDObject::GetInstance(ObjectTable[i].m_RttiName);
			}
			else
			{
				pObject = MWDObject::GetNoGCInstance(ObjectTable[i].m_RttiName);
			}
			

			if (!pObject)
			{
				continue;
			}
			
			m_pmLoadMap.AddElement(ObjectTable[i].m_pAddr, pObject);


			if (RegisterObject(pObject))
			{
				TableID.AddElement(i);
			}
		}

		//load object property table
		for (unsigned int i = 0; i < iObjectNum; i++)
		{
			ObjectTable[i].m_ObjectPropertyTable.ResetBufferNum(ObjectTable[i].m_uiObjectPropertyNum);

			for (unsigned int j = 0; j < ObjectTable[i].m_uiObjectPropertyNum; j++)
			{
				Read(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID, sizeof(unsigned int));
				Read(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiOffSet, sizeof(unsigned int));

			}
			m_pcCurBufPtr += ObjectTable[i].m_uiObjectPropertySize;
		}

		m_uiStreamFlag = AT_LOAD;
		//load object property

		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			MWDRtti& Rtti = m_pVObjectArray[i]->GetType();
			unsigned int uiTableID = TableID[i];
			for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
			{
				MWDProperty* pProperty = Rtti.GetProperty(j);
				if (pProperty->GetFlag() & MWDProperty::F_SAVE_LOAD)
				{
					for (unsigned int k = 0; k < ObjectTable[uiTableID].m_ObjectPropertyTable.GetNum(); k++)
					{
						if (MWDNameToID((unsigned char*)pProperty->GetName().GetBuffer(),sizeof(pProperty->GetName().GetLength())) == ObjectTable[uiTableID].m_ObjectPropertyTable[k].m_uiNameID)
						{
							m_pcCurBufPtr = m_pcBuffer + ObjectTable[uiTableID].m_ObjectPropertyTable[k].m_uiOffSet;
							this->Archive(*(MWDObject*)pProperty->GetValue(m_pVObjectArray[i]));
							break;
						}
					}
				}
			}
		}

		
		m_uiStreamFlag = AT_LINK;
		for (int i = m_pVObjectArray.GetNum() - 1; i >= 0; i--)
		{

			MWDRtti& Rtti = m_pVObjectArray[i]->GetType();
			for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
			{
				MWDProperty* pProperty = Rtti.GetProperty(j);
				if (pProperty->GetFlag() & MWDProperty::F_SAVE_LOAD)
				{
					this->Archive(*(MWDObject*)pProperty->GetValue(m_pVObjectArray[i]));
				}
			}
		}

		m_pcBuffer = NULL;
		return 1;
	}

	bool MWDStream::ArchiveAll(MWDObject* pObject)
	{
		if (!pObject)
		{
			return false;
		}

		if (m_uiStreamFlag == AT_REGISTER)
		{
			if (RegisterObject(pObject))
			{
				MWDRtti& Rtti = pObject->GetType();
				for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
				{
					MWDProperty* pProperty = Rtti.GetProperty(j);
					if (pProperty->GetFlag() & MWDProperty::F_SAVE_LOAD)
					{
						this->Archive(*(MWDObject*)pProperty->GetValue(pObject));
					}
				}
			}
		}
		else if (m_uiStreamFlag == AT_POSTLOAD)
		{
			if (RegisterPostLoadObject(pObject))
			{

				MWDRtti& Rtti = pObject->GetType();
				for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
				{
					MWDProperty* pProperty = Rtti.GetProperty(j);
					if (pProperty->GetFlag() & MWDProperty::F_SAVE_LOAD)
					{
						this->Archive(*(MWDObject*)pProperty->GetValue(pObject));
					}
				}
				pObject->PostLoad();
			}
		}
		else if (m_uiStreamFlag == AT_OBJECT_COLLECT_GC)
		{
			if (RegisterReachableObject(pObject))
			{
				MWDRtti& Rtti = pObject->GetType();
				for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
				{
					MWDProperty* pProperty = Rtti.GetProperty(j);
					if (!(pProperty->GetFlag() & MWDProperty::F_NO_USE_GC))
					{
						this->Archive(*(MWDObject*)pProperty->GetValue(pObject));
					}
				}
			}
		}
		else if (m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC)
		{
			if (RegisterReachableObject(pObject))
			{
				MWDRtti& Rtti = pObject->GetType();
				for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
				{
					MWDProperty* pProperty = Rtti.GetProperty(j);
					this->Archive(*(MWDObject*)pProperty->GetValue(pObject));
				}
			}
		}
		else if (m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
		{
			MWDRtti& Rtti = pObject->GetType();
			for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
			{
				MWDProperty* pProperty = Rtti.GetProperty(j);
				if (!(pProperty->GetFlag() & MWDProperty::F_NO_USE_GC))
				{
					this->Archive(*(MWDObject*)pProperty->GetValue(pObject));
				}
			}
		}
		return true;
	}

	bool MWDStream::RegisterObject(MWDObject* pObject)
	{
		MWDMAC_ASSERT(pObject);
		if (!pObject)
			return 0;

		for (unsigned int i = 0; i < (unsigned int)m_pVObjectArray.GetNum(); i++)
		{
			if (m_pVObjectArray[i] == pObject)
			{
				return 0;
			}

		}
		m_pVObjectArray.AddElement((MWDObject*)pObject);

		return 1;
	}

	bool MWDStream::RegisterPostLoadObject(MWDObject* pObject)
	{
		for (unsigned int i = 0; i < (unsigned int)m_pPostLoadObject.GetNum(); i++)
		{
			if (m_pPostLoadObject[i] == pObject)
			{
				return 0;
			}

		}
		m_pPostLoadObject.AddElement((MWDObject*)pObject);
		return true;
	}

	bool MWDStream::RegisterReachableObject(MWDObject* pObject)
	{
		if (pObject->IsHasFlag(MWDObject::OF_REACH))
		{
			return false;
		}
		pObject->SetFlag(MWDObject::OF_REACH);
		pObject->ClearFlag(MWDObject::OF_UNREACH);

		return 1;
	}
	
	const MWDObject* MWDStream::GetObjectByRtti(const MWDRtti& Rtti)
	{
		MWDObject* pObject = NULL;
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			if ((m_pVObjectArray[i]->GetType()).IsSameType(Rtti))
			{
				pObject = m_pVObjectArray[i];
				break;
			}
		}
		if (pObject == NULL)
		{
			for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
			{
				if ((m_pVObjectArray[i]->GetType()).IsDerived(Rtti))
				{

					pObject = m_pVObjectArray[i];
					break;
				}
			}
		}

		if (pObject)
		{
			for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
			{
				MWDObject* p = m_pVObjectArray[i];
				MWDMAC_ASSERT(p != NULL);
				if (p)
				{
					p->ClearFlag(MWDObject::OF_REACH);
					p->SetFlag(MWDObject::OF_UNREACH);
				}
			}

			MWDStream GCCollectStream;
			GCCollectStream.SetStreamFlag(MWDStream::AT_LOAD_OBJECT_COLLECT_GC);
			GCCollectStream.ArchiveAll(pObject);


			MWDArray<MWDObject*> CanGCObject;
			for (unsigned int i = 0; i < m_pVObjectArray.GetNum();)
			{
				MWDObject* p = m_pVObjectArray[i];
				if (p->IsHasFlag(MWDObject::OF_UNREACH))
				{
					CanGCObject.AddElement(p);
					m_pVObjectArray.Erase(i);
				}
				else
				{
					i++;
				}
			}

			//MWDResourceManager::AddCanGCObject(CanGCObject);

		}
		if (pObject)
		{
			m_uiStreamFlag = AT_POSTLOAD;
			ArchiveAll(pObject);
		}
		return pObject;
	}

	bool MWDStream::GetObjectArrayByRtti(const MWDRtti& Rtti, MWDArray<MWDObject*>& ObjectArray, bool IsDerivedFrom)
	{
		ObjectArray.Clear();
		for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			if ((m_pVObjectArray[i]->GetType()).IsSameType(Rtti) || ((m_pVObjectArray[i]->GetType()).IsDerived(Rtti) && IsDerivedFrom))
			{

				ObjectArray.AddElement(m_pVObjectArray[i]);
			}
		}
		if (ObjectArray.GetNum() > 0)
		{
			for (unsigned int i = 0; i < m_pVObjectArray.GetNum(); i++)
			{
				MWDObject* p = m_pVObjectArray[i];
				MWDMAC_ASSERT(p != NULL);
				if (p)
				{
					p->ClearFlag(MWDObject::OF_REACH);
					p->SetFlag(MWDObject::OF_UNREACH);
				}
			}

			MWDStream GCCollectStream;
			GCCollectStream.SetStreamFlag(MWDStream::AT_LOAD_OBJECT_COLLECT_GC);
			for (unsigned int i = 0; i < ObjectArray.GetNum(); i++)
			{
				GCCollectStream.ArchiveAll(ObjectArray[i]);
			}

			MWDArray<MWDObject*> CanGCObject;
			for (unsigned int i = 0; i < m_pVObjectArray.GetNum();)
			{
				MWDObject* p = m_pVObjectArray[i];
				if (p->IsHasFlag(MWDObject::OF_UNREACH))
				{
					CanGCObject.AddElement(p);
					m_pVObjectArray.Erase(i);
				}
				else
				{
					i++;
				}
			}

			//MWDResourceManager::AddCanGCObject(CanGCObject);

			m_uiStreamFlag = AT_POSTLOAD;
			for (unsigned int i = 0; i < ObjectArray.GetNum(); i++)
			{
				ArchiveAll(ObjectArray[i]);
			}

			return 1;
		}
		else
		{
			return 0;
		}
	}


}
