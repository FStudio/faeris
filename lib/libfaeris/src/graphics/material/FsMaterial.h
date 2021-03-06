#ifndef _FS_BASE_MATERIAL_H_
#define _FS_BASE_MATERIAL_H_
#include "FsMacros.h"
#include "mgr/FsResource.h"
#include "FsObject.h"
#include "graphics/FsRender.h"

NS_FS_BEGIN

enum{
	FS_UNIFORM_TEXTURE0_LOC=0,

	FS_UNIFORM_OPACITY_LOC=32,
	FS_UNIFORM_COLOR_LOC,
	FS_UNIFORM_MVP_LOC,
	FS_UNIFORM_POINT_SIZE_LOC,
	FS_UNIFORM_USER_DEFINE_LOC=64,
};

enum{
	FS_ATTR_V4F_LOC=0,
	FS_ATTR_T2F_LOC,
	FS_ATTR_A1F_LOC,
	FS_ATTR_C4F_LOC,
};

#define FS_UNIFORM_TEXTURE0_NAME "u_texture0"
#define FS_UNIFORM_OPACITY_NAME "u_opacity"
#define FS_UNIFORM_COLOR_NAME "u_color"
#define FS_UNIFORM_MVP_NAME "u_mvp" 
#define FS_UNIFORM_POINT_SIZE_NAME "u_pointSize"

#define FS_ATTR_V4F_NAME "a_position"
#define FS_ATTR_T2F_NAME "a_texCoord"
#define FS_ATTR_C4F_NAME "a_color"
#define FS_ATTR_A1F_NAME "a_alpha"




/* uniform:
 * 		float 		u_opacity,
 * 		vec4 		u_color,
 * 		mat4 		u_mvp,
 */


class Program;
class Material:public FsObject
{
	public:
		Material()
			:m_blendEquation(Render::EQUATION_ADD),
			m_blendSrc(Render::FACTOR_SRC_ALPHA),
			m_blendDst(Render::FACTOR_ONE_MINUS_SRC_ALPHA),
			m_depthTest(false),
			m_color(Color4f::WHITE),
			m_opacity(1.0f)
		{}

		virtual ~Material();
		virtual const char* className();

	public:
		/* blend */
		void setBlend(int equation,int src,int dst)
		{
			m_blendEquation=equation;
			m_blendSrc=src;
			m_blendDst=dst;
		}
		void setBlend(int src,int dst)
		{
			m_blendEquation=Render::EQUATION_ADD;
			m_blendSrc=src;
			m_blendDst=dst;
		}

		int getBlendEquation()
		{
			return m_blendEquation;
		}
		int getBlendSrc()
		{
			return m_blendSrc;
		}
		int getBlendDst()
		{
			return m_blendDst;
		}


		/* depth test */
		void setDepthTest(bool enable)
		{
			m_depthTest=enable;
		}
		bool getDepthTest()
		{
			return m_depthTest;
		}

		/* color */
		void setColor(Color4f c)
		{
			m_color=c;
		}
		Color4f getColor()
		{
			return m_color;
		}


		void setOpacity(float opacity)
		{
			m_opacity=opacity;
		}

		float getOpacity()
		{
			return m_opacity;
		}


	public:
		virtual void configRender(Render* r);

	protected:
		/* alpha blend*/
		int m_blendEquation;
		int m_blendSrc;
		int m_blendDst;

		bool m_depthTest;

		Color4f m_color;
		float m_opacity;
};


NS_FS_END 
#endif /*_FS_MATTERIAL_H_*/



