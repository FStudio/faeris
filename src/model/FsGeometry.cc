#include "string.h"
#include "model/FsGeometry.h"

NS_FS_BEGIN

static const FsChar* s_GeometryName="GeometryObject";
static const FsChar* s_GeometryAttributeName="GeometryAttributeObject";

const FsChar* Geometry::Attribute::getName()
{
	return s_GeometryAttributeName;
}
const FsChar* Geometry::getName()
{
	return s_GeometryName;
}


static FsInt s_typesize(FsInt t)
{
	switch(t)
	{
		case FS_FLOAT:
			return sizeof(FsFloat);
		case FS_INT:
			return sizeof(FsInt);
		case FS_INT8:
			return sizeof(FsInt8);
		case FS_INT16:
			return sizeof(FsInt16);
		case FS_INT32:
			return sizeof(FsInt32);
		case FS_UINT8:
			return sizeof(FsUint8);
		case FS_UINT16:
			return sizeof(FsUint16);
		case FS_UINT32:
			return sizeof(FsUint32);
		default:
			FS_TRACE_ERROR("Unkown Type");
	}
	return 0;
}

Geometry::Attribute::Attribute(FsString*  _name,FsInt _type,FsInt _size)
{
	name=_name;
	name->addRef();
	type=_type;
	size=_size;
	count=0;
	value=0;
}

Geometry::Attribute::Attribute(FsString* _name,FsInt _type,FsInt _size,FsInt _count)
{
	name=_name;
	name->addRef();
	type=_type;
	size=_size;
	count=0;
	value=0;
	resize(_count);
}

Geometry::Attribute::Attribute(const FsChar* _name,FsInt _type,FsInt _size)
{
	FsString* fs_name=new FsString(_name);
	name=fs_name;
	type=_type;
	size=_size;
	count=0;
	value=0;
}

Geometry::Attribute::Attribute(const FsChar* _name,FsInt _type,FsInt _size,FsInt _count)
{
	FsString* fs_name=new FsString(_name);
	name=fs_name;
	type=_type;
	size=_size;
	count=0;
	value=0;
	resize(_count);
}


void Geometry::Attribute::resize(FsInt num)
{
	if(count==num) 
	{
		return;
	}
	FsInt min=count<num?count:num;

	if(num==0)
	{
		delete[] (FsChar*)value;
		value=NULL;
	}
	else 
	{

		FsInt new_length=s_typesize(type)*size*num;
		FsVoid* new_value=new FsChar[new_length];

		if(value!=NULL)  /* if we has data before, just copy it */
		{
			memcpy(new_value,value,min*s_typesize(type)*size);
			delete[] (FsChar*)value;
		}
		if(num>count) /* set the other value zero */
		{
			memset((FsChar*)new_value+count*s_typesize(type)*size,0,(num-count)*s_typesize(type)*size);
		}
		value=new_value;
	}
	count=num;
}

Geometry::Attribute::~Attribute()
{
	name->decRef();
	if(value)
	{
		delete[] (FsChar*)value;
	}
}


void Geometry::setVertexNu(FsUint num)
{
	if(m_vertexNu==num)
	{
		return;
	}

	FsDict::Iterator iter(m_attrs);
	while(!iter.done())
	{
		Geometry::Attribute* attr=(Geometry::Attribute*)iter.getValue();
		if(attr)
		{
			attr->resize(num);
			attr->decRef();
		}
		iter.next();
	}
	m_vertexNu=num;
}

void Geometry::setFaceNu(FsUint num)
{
	if(m_faceNu==num)
	{
		return ;
	}
	FsUint min=m_faceNu>num?num:m_faceNu;

	if(num==0)
	{
		delete[] m_fFaces;
		m_fFaces=NULL;
	}
	else 
	{
		Face3* face_new=new Face3[num];
		if(m_fFaces!=NULL)
		{
			memcpy(face_new,m_fFaces,sizeof(Face3)*min);
			delete[] m_fFaces;
		}
		if(num>m_faceNu)
		{
			memset(face_new+m_faceNu,0,(num-m_faceNu)*sizeof(Face3));
		}
		m_fFaces=face_new;
	}
	m_faceNu=num;
}

void Geometry::setWeight(FsUint num)
{
	if(m_weightNu==num)
	{
		return ;
	}

	FsUint min=m_weightNu<num?m_weightNu:num;
	if(num==0)
	{
		delete[] m_wWeights;
		m_wWeights=NULL;
	}
	else 
	{
		Weight* weight_new=new Weight[num];
		if(m_wWeights!=NULL)
		{
			memcpy(weight_new,m_wWeights,sizeof(Weight)*min);
			delete[] m_wWeights;
		}
		if(num>m_weightNu)
		{
			memset(weight_new+m_weightNu,0,sizeof(Weight)*(num-m_weightNu));
		}
		m_wWeights=weight_new;
	}
	m_weightNu=num;
}


void Geometry::init(FsUint vertex,FsUint face,FsUint weight)
{
	m_attrs=new FsDict();

	m_fFaces=NULL;
	m_wWeights=NULL;
	m_vertexNu=0;
	m_faceNu=0;
	m_weightNu=0;

	setVertexNu(vertex);
	setFaceNu(face);
	setWeight(weight);
}

Geometry::Geometry(FsUint vertex,FsUint face,FsUint weight)
{
	init(vertex,face,weight);
}
Geometry::Geometry(FsUint vertex,FsUint face)
{
	init(vertex,face,0);
}
Geometry::Geometry(FsUint vertex)
{
	init(vertex,0,0);
}

Geometry::~Geometry()
{
	m_attrs->decRef();
	if(m_fFaces) delete[] m_fFaces;
	if(m_wWeights) delete[] m_wWeights;
}

NS_FS_END










