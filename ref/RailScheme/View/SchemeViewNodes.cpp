#include "Jx3DEnginePch.h"
//#include "stdafx.h"
#include "SchemeViewNodes.h"

#define LEFT_SIDE 0
#define RIGHT_SIDE 1

#include "Example/RailScheme/Model/SchemeModel.h"
#include "Util/Tin2Dem/jxDem.h"
#include "Util/Utils.h"

using namespace jx::kernel3d::engine::utils;
using namespace jx::kernel3d::engine::node;
using namespace jx::kernel3d::example::railscheme::model;

namespace jx{
namespace kernel3d {
namespace example{
namespace railscheme {
namespace view {
	
RoadBed::RoadBed()
{
	m_strName = "RoadBed";
	m_index = -1;
}

void RoadBed::Update()
{
	SchemeRenderModel *rm = (SchemeRenderModel*) m_pModel;
	int index = m_index;
	// 获取参数
	Point3Array& frontLeftLine = rm->matRoadbedFrontLeftLine3D[index];
	Point3Array& backLeftLine = rm->matRoadbedBackLeftLine3D[index];
	Point3Array& frontRightLine = rm->matRoadbedFrontRightLine3D[index];
	Point3Array& backRightLine = rm->matRoadbedBackRightLine3D[index];

	m_pNode = new osg::Group;

	// 画左面的边坡
	for (int i =0; i < (int)frontLeftLine.size()-1; i++)// 
	{
		osg::Vec3Array* quadPoints = MakeQuadPoints(POINT2VEC(frontLeftLine[i]), POINT2VEC(backLeftLine[i]), 
			POINT2VEC(frontLeftLine[i+1]), POINT2VEC(backLeftLine[i+1]));
		osg::Geode* geode = MakeQuadStripGeometry(quadPoints, jxBianpoTexture::GetImage());
		// 这里的纹理应该用rm.vecRoadbedLeftTexIndex
		m_pNode->addChild(geode);
	}
	
	
	// 画右边的边坡
	for (int i =0; i < (int)frontRightLine.size()-1; i++)// 
	{
		osg::Vec3Array* quadPoints = MakeQuadPoints(POINT2VEC(frontRightLine[i]), POINT2VEC(backRightLine[i]), 
			POINT2VEC(frontRightLine[i+1]), POINT2VEC(backRightLine[i+1]));
		osg::Geode* geode = MakeQuadStripGeometry(quadPoints, jxBianpoTexture::GetImage());
		m_pNode->addChild(geode);
	}

	// 画路面
	osg::Vec3 ptLeft1 = POINT2VEC(frontLeftLine[0]);
	osg::Vec3 ptLeft2 = POINT2VEC(backLeftLine[0]);
	osg::Vec3 ptRight1 = POINT2VEC(frontRightLine[0]);
	osg::Vec3 ptRight2 = POINT2VEC(backRightLine[0]);
	osg::Vec3Array* quadPoints = MakeQuadPoints(ptLeft1, ptLeft2, ptRight1, ptRight2);
	osg::Geode* geode = MakeQuadStripGeometry(quadPoints, jxRailwaySurfaceTexture::GetImage());
	m_pNode->addChild(geode);

	// 画截面
	//osg::ref_ptr<osg::Vec3Array> newVertices = new osg::Vec3Array;
	//newVertices->insert( newVertices->begin(), m_vecRightLine3D->rbegin(), m_vecRightLine3D->rend() );
	////newVertices->push_back(osg::Vec3(0,0,0));
	//newVertices->insert( newVertices->end(), m_vecLeftLine3D->begin(), m_vecLeftLine3D->end());
	//geode = Utils::MakePolygonGeode(newVertices.get(), "");
	//m_pNode->addChild(geode);
}

void RoadBed::UpdateProperties()
{
	jxNode::UpdateProperties();
	//m_vecPropItems.push_back(new NodePropertyItem("Licheng", "Licheng", Property_Float, &m_ptCenter.y()));
	//m_vecPropItems.push_back(new NodePropertyItem("BianpoTexture-L", "texture image file of Left Bianpo", Property_File, &m_texBianpoLeft));
	//m_vecPropItems.push_back(new NodePropertyItem("BianpoTexture-R", "texture image file of Right Bianpo", Property_File, &m_texBianpoRight));
	//m_vecPropItems.push_back(new NodePropertyItem("PingtaiTexture", "texture image file of Pingtai", Property_File, &m_texPingtai));
	//m_vecPropItems.push_back(new NodePropertyItem("LujiTexture", "texture image file of Luji", Property_File, &m_texLuji));
	//AddVec3ArrayProperty("LeftLinePoints", "", m_vecLeftLine);
	//AddVec3ArrayProperty("LeftLinePoints", "", m_vecRightLine);
}

Terrain::Terrain()
{
	m_pNode = NULL;
	m_strName = "Terrain";
}

void Terrain::UpdateProperties()
{
	jxNode::UpdateProperties();

	//m_vecPropItems.push_back(new NodePropertyItem("Texture", "texture image file of Terrain", Property_File, &m_strTextureFile));
}

void Terrain::Update()
{
	SchemeRenderModel *rm = (SchemeRenderModel*) m_pModel;
	int index = m_index;

	CDem& dem = rm->vecTerrainDem[index];

	// 给地面配置纹理
	osg::Geode* geodeTerrain = new osg::Geode;
	osg::StateSet *stateset = geodeTerrain->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::Image* image = osgDB::readImageFile(rm->vecTerrainTextureFile[index]);
	if (image)
	{
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);
		texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
		texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
		stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
	}
	geodeTerrain->setStateSet(stateset);

	osg::Drawable* geom = dem.GetGeometry();
	geodeTerrain->addDrawable(geom);

	m_pNode = new osg::Group;
	m_pNode->addChild(geodeTerrain);
}

RetainingWall::RetainingWall(void)
{
	m_strName = "RetainingWall";
}

void RetainingWall::Update()
{
	m_pNode = new osg::Group;
	int index = m_index;
	SchemeRenderModel *rm = (SchemeRenderModel*) m_pModel;

	// 画左边
	Point3Array& frontLeftPoints = rm->matRetainingWallFrontLeftLine3D[index];
	Point3Array& backLeftPoints = rm->matRetainingWallBackLeftLine3D[index];
	if (frontLeftPoints.size() > 3 && backLeftPoints.size() > 3)
	{
		osg::ref_ptr<osg::Vec3Array> vecFrontPoints = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> vecBackPoints = new osg::Vec3Array();
		InsertVector(vecFrontPoints.get(), &frontLeftPoints);
		InsertVector(vecBackPoints.get(), &backLeftPoints);
		osg::Geometry* geom = CreateColumn(vecFrontPoints, vecBackPoints);
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;  
		geode->addDrawable(geom);
		SetTextureImage(geode, rm->strRetainingWallTexture);
		m_pNode->addChild(geode.get());
	}
	
	// 画右边
	Point3Array& frontRightPoints = rm->matRetainingWallFrontRightLine3D[index];
	Point3Array& backRightPoints = rm->matRetainingWallBackRightLine3D[index];
	if (frontRightPoints.size() > 3 && backRightPoints.size() > 3)
	{
		osg::ref_ptr<osg::Vec3Array> vecFrontPoints = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> vecBackPoints = new osg::Vec3Array();
		InsertVector(vecFrontPoints.get(), &frontRightPoints);
		InsertVector(vecBackPoints.get(), &backRightPoints);
		osg::Geometry* geom = CreateColumn(vecFrontPoints, vecBackPoints);
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;  
		geode->addDrawable(geom);
		SetTextureImage(geode, rm->strRetainingWallTexture);
		m_pNode->addChild(geode.get());
	}
}

void RetainingWall::UpdateProperties()
{
	jxNode::UpdateProperties();

	//m_vecPropItems.push_back(new NodePropertyItem("Texture", "texture image file", Property_File, &m_strName));
	//AddVec3ArrayProperty("FrontPoints", "Front Surface Points", m_vecFrontPoints);
	//AddVec3ArrayProperty("BackPoints", "Back Surface Points",m_vecBackPoints);
}

BridgeBody::BridgeBody()
{
	m_strName = "BridgeBody";//"桥梁";
	m_strModelFile = "texture\\bridge_body_L10mW5mH0_7m.ive";
	m_fHeight = 0.7;
	m_fLength = 10;
	m_fWidth = 5;
	m_fRenderInterval = m_fLength;
	m_pRawNode = jxBridgeBodySingletonModel::GetNode();
}

BridgeHolder::BridgeHolder()
{
	m_strName = "BridgeHolder";//"桥托";
	m_strModelFile = "texture\\bridge_holder_H0_26m_body_W5m.ive";
	m_fHeight = 0.26;
	m_fLength = 1;
	m_fWidth = 3.5;
	m_fRenderInterval = 10;
	m_pRawNode = jxBridgeHolderSingletonModel::GetNode();
}

BridgePier::BridgePier()
{
	m_strName = "BridgePier";//"桥墩";
	m_strModelFile = "texture\\bridge_pier_L1_5mW2_5mH1m_body_5m.ive";
	m_fHeight = 1;
	m_fLength = 1.5;
	m_fWidth = 2.5;
	m_fRenderInterval = 10;
	m_pRawNode = jxBridgePierSingletonModel::GetNode();
}

Bridge::Bridge()
{
	m_strName = "Bridge";//"桥";
}

void Bridge::Update()
{
	m_pNode = new osg::Group;
	int index = m_index;
	SchemeRenderModel *rm = (SchemeRenderModel*) m_pModel;

	m_strName = rm->vecBridgeNames[index];
	Point3Array& topPts = rm->matBridgeCenterVertices3D[index];
	Point3Array& bottomPts = rm->matBridgeGroundCenterVertices3D[index];

	m_body.Reset();
	m_holder.Reset();
	m_pier.Reset();

	m_body.AddCenterLineVertices(topPts);
	m_holder.AddCenterLineVertices(topPts);
	m_pier.AddBottomCenterLineVertices(bottomPts);
	m_pier.AddTopCenterLineVertices(topPts);

	m_pNode->addChild(m_body.GetNode());
	m_pNode->addChild(m_holder.GetNode());
	m_pNode->addChild(m_pier.GetNode());
}

Tunnel::Tunnel()
{
	m_strName = "Tunnel";//"隧道";
	m_strModelFile = "texture\\tunnel_L10mW7mH6m.ive";
	m_fHeight = 6;
	m_fLength = 10;
	m_fWidth = 7;
	m_fRenderInterval = m_fLength;
	m_pRawNode = jxTunnelSingletonModel::GetNode();
}

void Tunnel::UpdateModel()
{
	jxModelNode::UpdateModel();

	int index = m_index;
	SchemeRenderModel *rm = (SchemeRenderModel*) m_pModel;
	
	m_strName = rm->vecTunnelNames[index];
	AddCenterLineVertices(rm->matTunnelCenterVertices3D[index]);
}

RailTrack::RailTrack()
{
	m_strName = "RailTrack";//"铁轨";
	m_strModelFile = "texture\\RailTrack_L10m.ive";
	m_fHeight = 1;
	m_fLength = 10;
	m_fWidth = 5.6;
	m_fRenderInterval = m_fLength;
	m_pRawNode = jxRailTrackSingletonModel::GetNode();
}

void RailTrack::UpdateModel()
{
	jxModelNode::UpdateModel();

	int index = m_index;
	SchemeRenderModel *rm = (SchemeRenderModel*) m_pModel;

	m_strName = rm->vecRailtrackNames[index];
	AddCenterLineVertices(rm->matRailtrackCenterVertices3D[index]);
}


}}}}}