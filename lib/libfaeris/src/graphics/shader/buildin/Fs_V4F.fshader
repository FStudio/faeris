"VERTEX_BEGIN    													\n\
uniform mat4 u_mvp=$(R.WORLD_VIEW_PROJECTION_MAT);					\n\
uniform float u_pointSize=$(M.POINT_SIZE) ;							\n\
attribute vec4 a_position=$(VERTICES);								\n\
void main() 														\n\
{																	\n\
	gl_Position=u_mvp*a_position;									\n\
	gl_PointSize=u_pointSize;										\n\
}																	\n\
 																	\n\
																	\n\
VERTEX_END 															\n\
																	\n\
																	\n\
FRAGMENT_BEGIN 														\n\
																	\n\
#ifdef GL_ES 														\n\
precision lowp float;												\n\
#endif 																\n\
uniform vec4  u_color=$(M.COLOR);									\n\
uniform float u_opacity=$(M.OPACITY);								\n\
void main()															\n\
{																	\n\
	gl_FragColor=u_color;											\n\
	gl_FragColor.a*=u_opacity;										\n\
}																	\n\
FRAGMENT_END";
