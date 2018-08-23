#include "Jx3DEnginePch.h"
#include "SchemeView.h"
#include "Node/jxNode.h"
#include "Util/JxDefine.h"
#include "Example/RailScheme/Model/SchemeModel.h"
#include "Example/RailScheme/View/SchemeViewNodes.h"

using namespace jx::kernel3d::engine::node;
using namespace jx::kernel3d::example::railscheme::model;

namespace jx{
namespace kernel3d {
namespace example{
namespace railscheme {
namespace view {

SchemeView::SchemeView( SchemeRenderModel* m)
{
	m_pOutModel = m;
	m_pOsgNode = new osg::Group();
}

SchemeView::~SchemeView()
{
	// m_pOutModel是外部指针，不需要释放
	// m_pRoot和layer是智能指针，不需要释放
}

osg::Node* SchemeView::GetNode()
{
	return m_pOsgNode.get();
}

void SchemeView::Reset()
{
	m_pOsgNode->removeChildren(0, m_pOsgNode->getNumChildren());
	for (int nLayer = 0; nLayer < SchemeLayerNum; nLayer++)
		m_pNodeLayer[nLayer].clear();
}

void SchemeView::Update()
{
	SchemeRenderModel *rm = m_pOutModel;
	if (0 == rm->nFlagChanged || 0 == rm->nFlagValid)
		return;

	this->Reset(); // 删除全部节点

	// 给要渲染的元素赋参数
	for (uint i = 0; i < rm->vecTerrainNames.size(); i++)
	{
		osg::ref_ptr<Terrain> node = new Terrain();
		node->SetModel(m_pOutModel, i);
		m_pNodeLayer[(int)SchemeLayerType_Terrain].push_back(node.get());
	}
	for (uint i = 0; i < rm->vecRoadbedNames.size(); i++)
	{
		osg::ref_ptr<RoadBed> node = new RoadBed();
		node->SetModel(m_pOutModel, i);
		m_pNodeLayer[(int)SchemeLayerType_Scheme].push_back(node.get());
	}
	for (uint i = 0; i < rm->vecRetainingWallNames.size(); i++)
	{
		osg::ref_ptr<RetainingWall> node = new RetainingWall();
		node->SetModel(m_pOutModel, i);
		m_pNodeLayer[(int)SchemeLayerType_Scheme].push_back(node.get());
	}
	for (uint i = 0; i < rm->vecBridgeNames.size(); i++)
	{
		osg::ref_ptr<Bridge> node = new Bridge();
		node->SetModel(m_pOutModel, i);
		m_pNodeLayer[(int)SchemeLayerType_Scheme].push_back(node.get());
	}
	for (uint i = 0; i < rm->vecTunnelNames.size(); i++)
	{
		osg::ref_ptr<Tunnel> node = new Tunnel();
		node->SetModel(m_pOutModel, i);
		m_pNodeLayer[(int)SchemeLayerType_Scheme].push_back(node.get());
	}
	for (uint i = 0; i < rm->vecRailtrackNames.size(); i++)
	{
		osg::ref_ptr<RailTrack> node = new RailTrack();
		node->SetModel(m_pOutModel, i);
		m_pNodeLayer[(int)SchemeLayerType_Scheme].push_back(node.get());
	}
	//for (uint i = 0; i < rm->vecStationNames.size(); i++)
	//{
	//	osg::ref_ptr<Station> node = new Station();
	//	node->SetModel(m_pOutModel, i);
	//	m_pNodeLayer[(int)SchemeLayerType_Scheme].push_back(node.get());
	//}

	this->Render();
}


// 在这个函数中，jxNode的参数转化为osg::node
// 渲染完成后，把模型参数的m_flagChanged置位0
void SchemeView::Render()
{
	for (int nLayer = 0; nLayer < SchemeLayerNum; nLayer++)
	{
		for (uint i = 0; i < m_pNodeLayer[nLayer].size(); i++)
		{
			jxNode* jNode = m_pNodeLayer[nLayer].at(i);
			osg::Node* osgNode = NULL;
			if (0 != jNode->m_pModel->nFlagChanged)
			{
				osgNode = jNode->GetNode(true);
				jNode->m_pModel->nFlagChanged = 0;
			}
			else
			{
				osgNode = jNode->GetNode(false);
			}
			m_pOsgNode->addChild(osgNode);
		}
	}
	m_pOutModel->nFlagChanged = 0;
}


}}}}}