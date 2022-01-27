#ifndef __engine_renderer_mesh_stencil__
#define __engine_renderer_mesh_stencil__


#include "engine/renderer/rendererSettings.h"




class Stencil
{
private:
	GLenum _func = GL_EQUAL;
	GLint  _ref  = 0;
	GLint  _mask = 0;

	GLenum _fail  = GL_KEEP;
	GLenum _zfail = GL_KEEP;
	GLenum _zpass = GL_KEEP;

	bool _enabled = false;



public:
	inline const GLenum func() const { return _func; }
	inline const GLint  ref () const { return _ref ; }
	inline const GLint  mask() const { return _mask; }

	inline const GLenum fail () const { return _fail ; }
	inline const GLenum zfail() const { return _zfail; }
	inline const GLenum zpass() const { return _zpass; }

	inline bool enabled() { return _enabled; }


public:
	friend class MeshData;

};


#endif // !__engine_renderer_mesh_texture__