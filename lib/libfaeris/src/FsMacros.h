#ifndef _FAERIS_MACROS_H_
#define _FAERIS_MACROS_H_
#include <assert.h>
#include "FsConfig.h"

#define FS_BEGIN_NAMESPACE(name) namespace name{
#define FS_END_NAMESPACE(name) }

#define NS_FS_BEGIN FS_BEGIN_NAMESPACE(Faeris)
#define NS_FS_END FS_END_NAMESPACE(Faeris)
#define NS_FS_USE using namespace Faeris;


#if defined(WIN32)
	#define FS_OS_WIN32

#elif defined(LINUX)
	#define FS_OS_LINUX

#elif defined(ANDROID)
	#define FS_OS_ANDROID
#else
	#error "UNKOWN PLATFORM OS"
#endif


#define FS_PLATFORM_OS defined


#ifndef NULL
	#define NULL 0
#endif 


#if defined(FS_OS_WIN32)
#define snprintf _snprintf
#endif 

namespace Faeris
{
	void FsUtil_TagLog(const char* tag,const char* msg,...);
	void FsUtil_Log(const char* fmt,...);
}

#define FS_LOG_TAG Faeris::FsUtil_TagLog
#define FS_LOG Faeris::FsUtil_Log




#define FS_CONDTION_ON(tag,cnd,fmt,...) \
	do{\
		if(cnd) \
		{ \
			FS_LOG_TAG(#tag,"%s:%s:%d:%s:"#fmt,__FILE__,__FUNCTION__,__LINE__,#cnd,##__VA_ARGS__); \
		}\
	} while(0)


#define FS_MESSAGE(tag,fmt,...) \
	do{\
		FS_LOG_TAG(#tag,"%s:%s:%d:"#fmt,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__); \
    } while(0)

#define FS_TRACE_WARN_ON(cnd,fmt,...) \
	FS_CONDTION_ON(WARN_ON,cnd,fmt,##__VA_ARGS__) 

#define FS_TRACE_ERROR_ON(cnd,fmt,...) \
	FS_CONDTION_ON(ERROR_ON,cnd,fmt,##__VA_ARGS__) 

#define FS_TRACE_INFO_ON(cnd,fmt,...) \
	FS_CONDTION_ON(INFO_ON,cnd,fmt,##__VA_ARGS__) 


#define FS_TRACE_WARN(fmt,...) \
	FS_MESSAGE(WARN,fmt,##__VA_ARGS__)

#define FS_TRACE_ERROR(fmt,...) \
	FS_MESSAGE(ERROR,fmt,##__VA_ARGS__)

#define FS_TRACE_INFO(fmt,...) \
	FS_MESSAGE(INFO,fmt,##__VA_ARGS__)


#define FS_ASSERT(condtion) \
	assert(condtion)



/* copy able */
#define FS_FEATURE public

#define FS_MAKE_NO_COPYABLE(class_name) \
	private: \
    class_name& operator&(class_name& /*other*/){return *this;} \
    class_name& operator=(class_name& /*other*/){return *this;}




/* for safe add and dec object refrence */

#define FS_SAFE_ADD_REF(f) \
	do{ if(f) f->addRef(); }while(0)

#define FS_SAFE_DEC_REF(f) \
	do{ if(f) f->decRef();} while(0)


#define FS_SAFE_ASSIGN(d,s) \
	do{ \
		FS_SAFE_ADD_REF(s); \
		FS_SAFE_DEC_REF(d); \
		d=s; \
	}while(0)

#define FS_SAFE_DELETE(v) \
	do{ if(v){ delete v;} } while(0)


/* global Macros For Faeris */

/* event part*/




#define FS_MASK_LSHIFT (0x1ul<<0)
#define FS_MASK_RSHIFT (0x1ul<<1)
#define FS_MASK_SHIFT (FS_MASK_LSHIFT|FS_MASK_RSHIFT)
#define FS_MASK_LALT (0x1ul<<2)
#define FS_MASK_RALT (0x1ul<<3)
#define FS_MASK_ALT (FS_MASK_LALT|FS_MASK_RALT)
#define FS_MASK_LCTRL (0x1ul<<4)
#define FS_MASK_RCTRL (0x1ul<<5)
#define FS_MASK_CTRL (FS_MASK_LCTRL|FS_MASK_RCTRL)
#define FS_MASK_LBUTTON (0x1ul<<6) 
#define FS_MASK_MBUTTON (0x1ul<<7)
#define FS_MASK_RBUTTON (0x1ul<<8)




#define FS_LBUTTON (0x1ul<<6)
#define FS_RBUTTON (0x1ul<<7)
#define FS_MBUTTON (0x1ul<<8)

#define FS_DOWN 0x1 
#define FS_UP 0x2 
#define FS_CLICK 0x3 
#define FS_DCLICK 0x4
#define FS_MOVE 0x5


/* Frame Buffer */
#define FS_BUFFER_LEFT
#define FS_BUFFER_RIGHT
#define FS_BUFFER_FRONT
#define FS_BUFFER_BACK
#define FS_BUFFER_SINGLE 
#define FS_BUFFER_DOUBLE 
#define FS_BUFFER_STENCIL
#define FS_BUFFER_SAMPLES 
#define FS_BUFFER_DEPTH
#define FS_BUFFER_RGB 
#define FS_BUFFER_RGBA



/* EULER  OLDER */
enum 
{
	FS_EULER_XYZ =0,
	FS_EULER_XZY ,
	FS_EULER_YXZ ,
	FS_EULER_YZX ,
	FS_EULER_ZXY ,
	FS_EULER_ZYX 
};




/* file type */
#define FS_FILE_UNKOWN (0x1)
#define FS_FILE_BINARY (0x2)
#define FS_FILE_SCRIPT (0x3)


enum 
{
	FS_FLOAT=0,
	FS_INT,

	FS_INT8,
	FS_INT16,
	FS_INT32,

	FS_UINT8,
	FS_UINT16,
	FS_UINT32,

	FS_MAX_TYPE_NU,
};


/* file type */
enum 
{
	FS_FTYPE_SCRIPT,
	FS_FTYPE_BINARY
};


/* bound type */
enum 
{
	FS_BOUND_BOX,
	FS_BOUND_SPHERE
};

/* window name */
#define FS_DEFAULT_WINDOW_NAME "FaerisEngine"


/* class name */


/* stage */
#define FS_STAGE_ELEMENT_CLASS_NAME "StageElement"


/* entity */
#define FS_ENTITY_CLASS_NAME "Entity"
#define FS_COLOR_QUAD2D_CLASS_NAME "ColorQuad2D"
#define FS_QUAD2D_CLASS_NAME   "Quad2D"
#define FS_LABEL_TTF_CLASS_NAME "LabelTTF"
#define FS_SPRITE2D_ANIMATION_CLASS_NAME "Sprite2DAnimation"
#define FS_SPRITE2D_DATA_CLASS_NAME "Sprite2DData"
#define FS_SPRITE2D_CLASS_NAME "Sprite2D"
#define FS_LABEL_BITMAP_CLASS_NAME "LabelBitmap"

/* scene */
#define FS_DIRECTOR_CLASS_NAME "Director"
#define FS_SCENE_CLASS_NAME "Scene"
#define FS_LAYER_CLASS_NAME "Layer"
#define FS_COLOR_LAYER_CLASS_NAME "ColorLayer"
#define FS_LAYER_2D_CLASS_NAME "Layer2D"


/* util */
#define FS_ARRAY_CLASS_NAME "FsArray"
#define FS_SLOW_ARRAY_CLASS_NAME "FsSlowArray"
#define FS_STRING_CLASS_NAME "FsString"
#define FS_INTEGER_CLASS_NAME "FsInteger"
#define FS_DICT_CLASS_NAME "FsDict"

/* material */
#define FS_MATERIAL_CLASS_NAME "Material"
#define FS_QUAD_2D_MATERIAL_CLASS_NAME "Quad2DMaterial"
#define FS_POSITION_ALPHA_TEXTURE_MATERIAL_CLASS_NAME "Mat_V4F_T2F_A1F"
#define FS_POSITION_TEXTURE_MATERIAL_CLASS_NAME "Mat_V4F_T2F"

/* sys */
#define FS_WINDOW_CLASS_NAME "Window"

/* event */
#define FS_TOUCH_EVENT_LISTENER_CLASS_NAME "TouchEventListener"
#define FS_TOUCH_DISPATCHER_CLASS_NAME "TouchDispatcher"
#define FS_SYS_DISPATCHER_CLASS_NAME "SysDispatcher"
#define FS_SYS_EVENT_LISTENER_CLASS_NAME "SysEventListener"
#define FS_KEYPAD_EVENT_LISTENER_CLASS_NAME "KeypadEventListener"
#define FS_KEYPAD_DISPATCHER_CLASS_NAME "KeypadDispatcher"

/* scheduler */
#define FS_SCHEDULER_CLASS_NAME "Scheduler"
#define FS_SCHEDULER_TARGET_CLASS_NAME "SchedulerTarget"
#define FS_TASK_CLASS_NAME "Task"


/* graphics */
#define FS_RENDER_CLASS_NAME "Render"
#define FS_TEXTURE2D_CLASS_NAME "Texture2D"
#define FS_GLYPH_CLASS_NAME  "Glyph"
#define FS_FONT_TTF_DATA_CLASS_NAME "FontTTFData"
#define FS_FONT_TTF_CLASS_NAME "FontTTF"
#define FS_IMAGE2D_CLASS_NAME "Image2D"
#define FS_PROGRAM_CLASS_NAME "Program"
#define FS_GLYPH_BITMAP_CLASS_NAME "GlyphBitmap"
#define FS_FONT_BITMAP_CLASS_NAME "FontBitmap"

/* mgr */
#define FS_TEXTURE_MGR_CLASS_NAME "TextureMgr"
#define FS_FONT_TTF_DATA_MGR_CLASS_NAME  "FontTTFDataMgr"
#define FS_SPRITE_2D_DATA_MGR_CLASS_NAME "Sprite2DDataMgr"


/* action */
#define FS_SCALE_TO_ACTION_CLASS_NAME "ScaleToAction" 
#define FS_SCALE_BY_ACTION_CLASS_NAME "ScaleByAction"
#define FS_MOVE_TO_ACTION_CLASS_NAME "MoveToAction"
#define FS_MOVE_BY_ACTION_CLASS_NAME "MoveByAction"
#define FS_ROTATE_Z_TO_ACTION_CLASS_NAME "RotateZToAction"
#define FS_ROTATE_Z_BY_ACTION_CLASS_NAME "RotateZByAction"
#define FS_ACTION_CLASS_NAME "Action"
#define FS_PAUSE_ACTION_CLASS_NAME "PauseAction"
#define FS_SEQ_ACTION_CLASS_NAME "SeqAction"

/* io */
#define FS_FILE_CLASS_NAME "FsFile"
#define FS_MEM_FILE_CLASS_NAME "MemFile"
#define FS_SYS_FILE_CLASS_NAME "SysFile"
#define FS_PACKAGE_CLASS_NAME "Package"
#define FS_RES_PACKAGE_CLASS_NAME "ResPackage"
#define FS_ZIP_PACKAGE_CLASS_NAME "ZipPackage"
#define FS_FGZ_PACKAGE_CLASS_NAME "FgzPackage"
#define FS_VFS_NAME_FILTER_CLASS_NAME "NameFilter"
#define FS_VFS_PREFIX_NAME_FILTER_CLASS_NAME "PrefixNameFilter"
#define FS_VFS_MAP_PACKAGE_CLASS_NAME "MapPackage"
#define FS_SEG_FILE_CLASS_NAME  "SegFile"

/* support */
#define FS_ZIP_READER_CLASS_NAME "ZipReader"

/* networking */
#define FS_HTTP_ENGINE_CLASS_NAME "HttpEngine"

#include "FsTypes.h"



#endif /*_FAERIS_MACROS_H_*/



