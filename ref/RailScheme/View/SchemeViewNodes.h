// fengjx 20160630 路基结构
#ifndef _SCHEMEVIEWNODES_H
#define _SCHEMEVIEWNODES_H

#include "Node/jxNode.h"
#include "Node/jxModelNode.h"

namespace jx{
namespace kernel3d {
namespace example{
namespace railscheme {
namespace view {

//路基
class RoadBed : public jx::kernel3d::engine::node::jxNode
{

public:
	RoadBed();
	virtual void Update();
	virtual void UpdateProperties();
};

// 地形
class Terrain : public jx::kernel3d::engine::node::jxNode
{
public:
	Terrain();
	virtual void Update();
	virtual void UpdateProperties();
};


// 挡土墙
class RetainingWall : public jx::kernel3d::engine::node::jxNode
{
public:
	RetainingWall();
	virtual void Update();
	virtual void UpdateProperties();
};

// 桥体
class BridgeBody : public jx::kernel3d::engine::node::jxModelNode
{
public:
	BridgeBody();
};

// 桥托
class BridgeHolder : public jx::kernel3d::engine::node::jxModelNode
{
public:
	BridgeHolder();
};

// 桥墩
class BridgePier : public jx::kernel3d::engine::node::jxModelNode
{
public:
	BridgePier();
};

// 桥
class Bridge : public jx::kernel3d::engine::node::jxNode
{
public:
	Bridge();
	virtual void Update();
public:
	BridgeBody m_body;
	BridgeHolder m_holder;
	BridgePier m_pier;
};


// 隧道
class Tunnel : public jx::kernel3d::engine::node::jxModelNode
{
public:
	Tunnel();
	virtual void UpdateModel();
};

// 铁轨
class RailTrack : public jx::kernel3d::engine::node::jxModelNode
{
public:
	RailTrack();
	virtual void UpdateModel();
};
//
//
//class Tree : public jx::kernel3d::engine::node::jxModelNode
//{
//public:
//	Tree()
//	{
//		m_strName = "Tree";//"树";
//		m_strModelFile = "texture\\tree.ive";
//		m_fHeight = 5;
//		m_fLength = 1;
//		m_fWidth = 1;
//		m_fRenderInterval = -1;
//		m_pRawNode = jxSingletonModel::GetNode(Ive_Tree);
//	}
//};
//
//class FarmHouse : public jx::kernel3d::engine::node::jxModelNode
//{
//public:
//	FarmHouse()
//	{
//		m_strName = "FarmHouse";//"农舍";
//		m_strModelFile = "texture\\farm_house.ive";
//		m_fHeight = 5;
//		m_fLength = 22;
//		m_fWidth = 15;
//		m_fRenderInterval = -1;
//		m_pRawNode = jxSingletonModel::GetNode(Ive_FarmHouse);
//	}
//};
//
//class RailSideStick : public jx::kernel3d::engine::node::jxModelNode
//{
//public:
//	RailSideStick()
//	{
//		m_strName = "RailSideStick";//"电线杆";
//		m_strModelFile = "texture\\rail_right_stick_L50m.ive";
//		m_fHeight = 5;
//		m_fLength = 50;
//		m_fWidth = 0.5;
//		m_fRenderInterval = 50;
//		m_pRawNode = jxSingletonModel::GetNode(Ive_RailSideStick);
//	}
//};
//
//class RailBar : public jx::kernel3d::engine::node::jxModelNode
//{
//public:
//	RailBar()
//	{
//		m_strName = "RailBar";//"防护栏";
//		m_strModelFile = "texture\\rail_bar_L10mH2m.ive";
//		m_fHeight = 2;
//		m_fLength = 10;
//		m_fWidth = 0.1;
//		m_fRenderInterval = 10;
//		m_pRawNode = jxSingletonModel::GetNode(Ive_RailBar);
//	}
//};
//
//class Train : public jx::kernel3d::engine::node::jxModelNode
//{
//public:
//	Train()
//	{
//		m_strName = "Train";//"火车";
//		m_strModelFile = "texture\\train.ive";
//		m_fHeight = 2;
//		m_fLength = 10;
//		m_fWidth = 2;
//		m_fRenderInterval = 10;
//		m_pRawNode = jx::kernel3d::engine::node::jxSingletonModel::GetNode(Ive_Train);
//	}
//};


}}}}}

#endif