#include "FsRotateAction.h"

NS_FS_BEGIN

const char* RotateAction::className()
{
	return FS_ROTATE_ACTION_CLASS_NAME;
}


RotateAction* RotateAction::createFromTo(const Vector3& from,const Vector3& to,float time)
{
	RotateAction* ret=new RotateAction();
	ret->initWithFromTo(from,to,time);
	return ret;
}

RotateAction* RotateAction::createBy(const Vector3& from,const Vector3& by,float time)
{
	RotateAction* ret=new RotateAction();
	ret->initWithBy(from,by,time);
	return ret;
}

RotateAction* RotateAction::create(Curve3* curve,float time)
{
	RotateAction* ret=new RotateAction();
	ret->initWithCurve(curve,time);
	return ret;
}


void RotateAction::initWithFromTo(const Vector3& from,const Vector3& to,float time)
{
	LinearCurve* curve=LinearCurve3::create(from,to);
	setCurve(curve);
	setTotalTime(time);
}

void RotateAction::initWithBy(const Vector3& from,const Vector3& by,float time)
{
	LinearCurve* curve=LinearCurve3::create(from,from+by);
	setCurve(curve);
	setTotalTime(time);
}

void RotateAction::InitWithCurve(Curve3* curve,float time)
{
	setCurve(curve);
	setTotalTime(time);
}


bool MoveAction::step(ActionTarget* target,float percent)
{

	Vector3 pos=getCurveValue(percent);

	if(m_markBit&CurveUsedMarkBit::USED_X)
	{
		target->setRotateX(pos.x);
	}

	if(m_markBit&CurveUsedMarkBit::USED_Y)
	{
		target->setRotateY(pos.y);
	}

	if(m_markBit&CurveUsedMarkBit::USED_Z)
	{
		target->setRotateZ(pos.z);
	}
}


NS_FS_END 

