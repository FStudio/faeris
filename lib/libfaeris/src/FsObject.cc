#include "FsObject.h"
#include "FsGlobal.h"
#include "extends/FsScriptEngine.h"

#include "mgr/FsObjectMgr.h"

NS_FS_BEGIN 

int FsObject::m_objectNu=0;

FsObject::~FsObject()
{
	if(m_objectMgr)
	{
		m_objectMgr->unManageObject(this);
	}

#if FS_CONFIG(FS_SCRIPT_SUPPORT)
	finalize();
	dropScriptData();
#endif 

	FsObject::m_objectNu--;
}


void FsObject::setObjectMgr(ObjectMgr* mgr)
{
	m_objectMgr=mgr;
}

ObjectMgr* FsObject::getObjectMgr()
{
	return m_objectMgr;
}


long FsObject::getHashCode() 
{
	return (long)this;
}

bool FsObject::equal(FsObject* ob)
{
	return this==ob;
}


#if FS_CONFIG(FS_SCRIPT_SUPPORT)
void FsObject::dropScriptData()
{
	if(m_scriptData!=-1)
	{
		ScriptEngine* se=Global::scriptEngine();
		if(se!=NULL)
		{
			se->releaseData(m_scriptData);
		}
		m_scriptData=-1;
	}
}

void _FsScriptExtends_Finalize(FsObject*);

void FsObject::finalize()
{
	/* NOTE: lua: implement in file FsScriptEngine.cc */
	_FsScriptExtends_Finalize(this);
}
#endif 

NS_FS_END 




