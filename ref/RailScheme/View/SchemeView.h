#ifndef _SCHEMEVIEW_H
#define _SCHEMEVIEW_H

//#include <osg/ref_ptr>
//#include <osg/Group>

#define SchemeLayerNum 3

namespace jx{
namespace kernel3d { 
namespace engine
{ 
	namespace node { class jxNode; }
}
namespace example{
namespace railscheme{
namespace model 
{ 
	//class GeometryModel;
	//enum GeometryModelType;
	class SchemeRenderModel; 
}
namespace view {

class Terrain;
class RoadBed;
class Bridge;
class Tunnel;
class Railtrack;
class Station;

class SchemeView
{
public:
	SchemeView(jx::kernel3d::example::railscheme::model::SchemeRenderModel* m);
	~SchemeView();
	osg::Node* GetNode();
	void Update();
public:
	std::vector<jx::kernel3d::engine::node::jxNode*> m_pNodeLayer[SchemeLayerNum]; // ������
private:
	void Reset(); // ���view
	void Render(); // ����jxNode����osg::Node���Ӻ�
	Terrain* BuildTerrainNode(int index);
	RoadBed* BuildRoadbedNode(int index);
	Bridge* BuildBridgenNode(int index);
	Tunnel* BuildTunnelNode(int index);
	Railtrack* BuildRailtrackNode(int index);
	Station* BuildStationNode(int index);
	jx::kernel3d::engine::node::jxNode* BuildJxNode(int index);
private:
	osg::ref_ptr<osg::Group> m_pOsgNode;
	jx::kernel3d::example::railscheme::model::SchemeRenderModel* m_pOutModel;
};

//// ��Ⱦ�߳�,��ʱû��ʱ������Ҳ��ӷ�����������Ⱦ��������Ⱦ��������Ⱦ�������ö��߳��Ż�
// fengjx 2017.9.16
//class RenderThread : public OpenThreads::Thread
//{
//public:
//	RenderThread( jx::kernel3d::engine::node::jxNode* node );
//	virtual ~RenderThread();
//	virtual void run();
//private:
//	jx::kernel3d::engine::node::jxNode* m_node;
//};


}}}}}

#endif