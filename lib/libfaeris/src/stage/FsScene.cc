#include "stage/FsScene.h"
#include "stage/layer/FsLayer.h"
#include "support/util/FsSlowArray.h"
NS_FS_BEGIN

Scene* Scene::create()
{
	Scene* ret=new Scene;
	return ret;
}

/* layer operation */
void Scene::push(Layer* layer)
{
	if(layer->m_scene)
	{
		FS_TRACE_WARN("layer is already owned by scene");
		return ;
	}
	m_layers->push(layer);
	layer->m_scene=this;
}

void Scene::pop()
{
	int size=m_layers->size();
	if(size==0)
	{
		FS_TRACE_WARN("No Layer to pop");
		return;
	}
	Layer* ret=(Layer*)m_layers->get(size-1);
	ret->m_scene=NULL;
	m_layers->pop();
	ret->decRef();
}

void Scene::insert(int pos,Layer* layer)
{
	if(layer->m_scene)
	{
		FS_TRACE_WARN("layer is already owned by scene");
		return ;
	}
	m_layers->insert(pos,layer);
	layer->m_scene=this;
}
void Scene::replace(int pos,Layer* layer)
{
	Layer*  ret=(Layer*)m_layers->get(pos);
	if(ret==NULL)
	{
		FS_TRACE_WARN("Index(%d) Layer Out Of Range",pos);
		return; 
	}
	ret->m_scene=NULL;
	ret->decRef();

	m_layers->set(pos,layer);
}

void Scene::remove(Layer* layer)
{
	if(layer->m_scene!=this)
	{
		FS_TRACE_WARN("Layer is not owned by scene");
		return;
	}

	layer->m_scene=NULL;
	m_layers->remove(layer);
}

Layer* Scene::top()
{
	return (Layer*)m_layers->top();
}

int Scene::layerNu()
{
	return m_layers->size();
}

Layer* Scene::getLayer(int index)
{
	return (Layer*)m_layers->get(index);
}

int Scene::getLayerIndex(Layer* layer)
{
	int layer_nu=m_layers->size();
	Layer* cur=NULL;

	for(int i=0;i<layer_nu;i++)
	{
		cur=(Layer*)m_layers->get(i);
		if(cur==layer)
		{
			cur->decRef();
			return i;
		}

		cur->decRef();
	}
	return -1;
}

void Scene::clear()
{
	int layer_nu=m_layers->size();
	for(int i=layer_nu-1;i>=0;i--)
	{
		Layer* layer=(Layer*)m_layers->get(i);
		layer->m_scene=NULL;
		layer->decRef();
	}
	m_layers->clear();
}

void Scene::drop(bool recursion) 
{
	int layer_nu=m_layers->size();
	for(int i=layer_nu-1;i>=0;i--)
	{
		Layer* layer=(Layer*)m_layers->get(i);
		if(recursion)
		{
			layer->drop(true);
		}
		layer->m_scene=NULL;
		layer->decRef();
	}
	m_layers->clear();
	FsObject::dropScriptData();
}

/* event */

void Scene::enter()
{
	/* do nothing */
}
void Scene::exit()
{
	/* do nothing */
}
void Scene::update(float dt)
{
	updateAction(dt);
	updateLayers(dt);
}


void Scene::updateLayers(float dt)
{
	m_layers->lock();
	int layer_nu=m_layers->size();
	for(int i=layer_nu-1;i>=0;i--)
	{
		Layer* layer=(Layer*)m_layers->get(i);
		if(layer->visible())
		{
			layer->update(dt);
		}
		layer->decRef();
	}
	m_layers->unlock();
	m_layers->flush();
}

void Scene::draw(Render* render)
{
	m_layers->lock();
	int layer_nu=m_layers->size();
	for(int i=0;i<layer_nu;i++)
	{
		Layer* layer=(Layer*)m_layers->get(i);
		if(layer->visible())
		{
			layer->draw(render);
		}
		layer->decRef();
	}
	m_layers->unlock();
	m_layers->flush();
}
void Scene::touchBegin(float x,float y)
{
	m_layers->lock();
	int layer_nu=m_layers->size();
	for(int i=layer_nu-1;i>=0;i--)
	{
		bool handle=false;
		Layer* layer=(Layer*)m_layers->get(i);
		if(layer->touchEnabled()&&layer->visible())
		{
			handle=layer->touchBegin(x,y);
		}
		layer->decRef();
		if(handle)
		{
			break;
		}
	}
	m_layers->unlock();
	m_layers->flush();
}
void Scene::touchMove(float x,float y)
{
	m_layers->lock();
	int layer_nu=m_layers->size();
	for(int i=layer_nu-1;i>=0;i--)
	{
		bool handle=false;
		Layer* layer=(Layer*)m_layers->get(i);
		if(layer->touchEnabled()&&layer->visible())
		{
			handle=layer->touchMove(x,y);
		}
		layer->decRef();
		if(handle)
		{
			break;
		}
	}
	m_layers->unlock();
	m_layers->flush();
}

void Scene::touchEnd(float x,float y)
{
	m_layers->lock();
	int layer_nu=m_layers->size();
	for(int i=layer_nu-1;i>=0;i--)
	{
		bool handle=false;
		Layer* layer=(Layer*)m_layers->get(i);
		if(layer->touchEnabled()&&layer->visible())
		{
			handle=layer->touchEnd(x,y);
		}
		layer->decRef();
		if(handle)
		{
			break;
		}
	}
	m_layers->unlock();
	m_layers->flush();
}

void Scene::touchesBegin(TouchEvent* event)
{
	m_layers->lock();
	int layer_nu=m_layers->size();
	for(int i=layer_nu-1;i>=0;i--)
	{
		bool handle=false;
		Layer* layer=(Layer*)m_layers->get(i);
		if(layer->touchEnabled()&&layer->visible())
		{
			handle=layer->touchesBegin(event);
		}
		layer->decRef();
		if(handle)
		{
			break;
		}
	}
	m_layers->unlock();
	m_layers->flush();
}

void Scene::touchesPointerDown(TouchEvent* event)
{
	m_layers->lock();
	int layer_nu=m_layers->size();
	for(int i=layer_nu-1;i>=0;i--)
	{
		bool handle=false;
		Layer* layer=(Layer*)m_layers->get(i);
		if(layer->touchEnabled()&&layer->visible())
		{
			handle=layer->touchesPointerDown(event);
		}
		layer->decRef();
		if(handle)
		{
			break;
		}
	}
	m_layers->unlock();
	m_layers->flush();
}
void Scene::touchesMove(TouchEvent* event)
{
	m_layers->lock();
	int layer_nu=m_layers->size();
	for(int i=layer_nu-1;i>=0;i--)
	{
		bool handle=false;
		Layer* layer=(Layer*)m_layers->get(i);
		if(layer->touchEnabled()&&layer->visible())
		{
			handle=layer->touchesMove(event);
		}
		layer->decRef();
		if(handle)
		{
			break;
		}
	}
	m_layers->unlock();
	m_layers->flush();
}

void Scene::touchesPointerUp(TouchEvent* event)
{
	m_layers->lock();
	int layer_nu=m_layers->size();
	for(int i=layer_nu-1;i>=0;i--)
	{
		bool handle=false;
		Layer* layer=(Layer*)m_layers->get(i);
		if(layer->touchEnabled()&&layer->visible())
		{
			handle=layer->touchesPointerUp(event);
		}
		layer->decRef();
		if(handle)
		{
			break;
		}
	}
	m_layers->unlock();
	m_layers->flush();
}

void Scene::touchesEnd(TouchEvent* event)
{
	m_layers->lock();
	int layer_nu=m_layers->size();
	for(int i=layer_nu-1;i>=0;i--)
	{
		bool handle=false;
		Layer* layer=(Layer*)m_layers->get(i);
		if(layer->touchEnabled()&&layer->visible())
		{
			handle=layer->touchesEnd(event);
		}
		layer->decRef();
		if(handle)
		{
			break;
		}
	}
	m_layers->unlock();
	m_layers->flush();
}



void Scene::keypadEvent(int type,int keycode)
{
}



const char* Scene::className()
{
	return FS_SCENE_CLASS_NAME;
}

Scene::Scene()
{
	init();
}
Scene::~Scene()
{
	destroy();
}

void Scene::init()
{
	m_layers=FsSlowArray::create();
}

void Scene::destroy()
{
	m_layers->decRef();
}


NS_FS_END
