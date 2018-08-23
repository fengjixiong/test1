#include "Jx3DEnginePch.h"
#include "Util/Serializer.h"
#include "SchemeSerializer.h"
#include "Example/RailScheme/Model/SchemeModel.h"

#include "JxMQ.h"
#include "JxStructSerializer.h"
#pragma comment(lib, "JxMQ.lib")

using namespace jx::kernel3d::example::railscheme::model;
using namespace jx::kernel3d::engine::utils;

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme {
namespace serialize {

SchemeSerializer::SchemeSerializer(SchemeModel* m)
{
	m_pOutModel = m;
}

SchemeSerializer::~SchemeSerializer()
{
	// m_pModel是从外部传入的引用，不释放
}

template<typename T>
void LoadObjectFromXml(jx::message_queue::JxMessageData &data, std::stringstream &ss, char* name, T& val)
{
	if (name == NULL)
		return;
	ss.clear(); ss.str(""); 
	char* prop = data.GetProperty(name); 
	if (prop == NULL)
		return;
	ss << prop;
	ss >> val;
}

template<typename T>
void SaveObjectToXml(jx::message_queue::JxMessageData &data, std::stringstream &ss, char* name, T& val)
{
	if (name == NULL)
		return;
	ss.clear(); ss.str(""); 
	ss << val; 
	data.SetProperty(name, (char*)ss.str().c_str());
}

void SchemeSerializer::Load(char* filepath)
{
	jx::message_queue::JxMessageData data;
	data.LoadFromFile(filepath);

	std::stringstream ss;
	SchemeInitModel&   im = m_pOutModel->m_initModel;
	SchemeDesignModel& dm = m_pOutModel->m_designModel;
	SchemeAnalysModel& am = m_pOutModel->m_analysModel;
	SchemeRenderModel& rm = m_pOutModel->m_renderModel;

	// 接下来从data中获取各种参数
	LoadObjectFromXml(data, ss, "Scheme.InitModel.nRailLineNum", im.nRailLineNum);
	LoadObjectFromXml(data, ss, "Scheme.InitModel.fRailLineInteral", im.fRailLineInteral);
	LoadObjectFromXml(data, ss, "Scheme.InitModel.fRoadWidth", im.fRoadWidth);
	LoadObjectFromXml(data, ss, "Scheme.InitModel.fStartLicheng", im.fStartLicheng);
	LoadObjectFromXml(data, ss, "Scheme.InitModel.strRoadType", im.strRoadType);
	LoadObjectFromXml(data, ss, "Scheme.InitModel.pt3DCenter", im.pt3DCenter);
	LoadObjectFromXml(data, ss, "Scheme.InitModel.vecSelectLinePoints3D", im.vecSelectLinePoints3D);
	LoadObjectFromXml(data, ss, "Scheme.InitModel.vecTerrainNames", im.vecTerrainNames);
	LoadObjectFromXml(data, ss, "Scheme.InitModel.matTerrainPoints3D", im.matTerrainPoints3D);
	LoadObjectFromXml(data, ss, "Scheme.InitModel.matTerrainPoints2D", im.matTerrainPoints2D);
	LoadObjectFromXml(data, ss, "Scheme.InitModel.nFlagChanged", im.nFlagChanged);
	LoadObjectFromXml(data, ss, "Scheme.InitModel.nFlagValid", im.nFlagValid);

	LoadObjectFromXml(data, ss, "Scheme.DesignModel.vecTin", dm.vecTin);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.vecDem", dm.vecDem);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.vecSectionType", dm.vecSectionType);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.vecSectionLicheng", dm.vecSectionLicheng);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.vecSectionCenterPoints3D", dm.vecSectionCenterPoints3D);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.vecRoadbedSecIndex", dm.vecRoadbedSecIndex);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.vecRoadbedLicheng", dm.vecRoadbedLicheng);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.vecRoadbedLuji2D", dm.vecRoadbedLuji2D);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.vecRoadbedCenter3D", dm.vecRoadbedCenter3D);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.matRoadbedLeftLine2D", dm.matRoadbedLeftLine2D);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.matRoadbedRightLine2D", dm.matRoadbedRightLine2D);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.matRetainingWallLeftLine2D", dm.matRetainingWallLeftLine2D);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.matRetainingWallRightLine2D", dm.matRetainingWallRightLine2D);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.nFlagChanged", dm.nFlagChanged);
	LoadObjectFromXml(data, ss, "Scheme.DesignModel.nFlagValid", dm.nFlagValid);

	LoadObjectFromXml(data, ss, "Scheme.AnalysModel.matTerrainConstraintPoints3D", am.matTerrainConstraintPoints3D);
	LoadObjectFromXml(data, ss, "Scheme.AnalysModel.vecTinWithConstraints", am.vecTinWithConstraints);
	LoadObjectFromXml(data, ss, "Scheme.AnalysModel.vecSectionDirectionPoints", am.vecSectionDirectionPoints);
	LoadObjectFromXml(data, ss, "Scheme.AnalysModel.vecSectionNormPoints", am.vecSectionNormPoints);
	LoadObjectFromXml(data, ss, "Scheme.AnalysModel.matRoadbedLeftInterpLine2D", am.matRoadbedLeftInterpLine2D);
	LoadObjectFromXml(data, ss, "Scheme.AnalysModel.matRoadbedRightInterpLine2D", am.matRoadbedRightInterpLine2D);
	LoadObjectFromXml(data, ss, "Scheme.AnalysModel.nFlagChanged", am.nFlagChanged);
	LoadObjectFromXml(data, ss, "Scheme.AnalysModel.nFlagValid", am.nFlagValid);

	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecTerrainNames", rm.vecTerrainNames);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecTerrainTextureFile", rm.vecTerrainTextureFile);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecTerrainDem", rm.vecTerrainDem);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecRoadbedNames", rm.vecRoadbedNames);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecRoadbedTextures", rm.vecRoadbedTextures);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.strRoadbedLeftBianpoTextureFile", rm.strRoadbedLeftBianpoTextureFile);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.strRoadbedRightBianpoTextureFile", rm.strRoadbedRightBianpoTextureFile);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.strRoadbedPingtaiTextureFile", rm.strRoadbedPingtaiTextureFile);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.strRoadbedLujiTextureFile", rm.strRoadbedLujiTextureFile);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecRoadbedTypeLeftTexIndex", rm.vecRoadbedTypeLeftTexIndex);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecRoadbedTypeRightTexIndex", rm.vecRoadbedTypeRightTexIndex);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.strRetainingWallTexture", rm.strRetainingWallTexture);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matRoadbedFrontLeftLine3D", rm.matRoadbedFrontLeftLine3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matRoadbedFrontRightLine3D", rm.matRoadbedFrontRightLine3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matRoadbedBackLeftLine3D", rm.matRoadbedBackLeftLine3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matRoadbedBackRightLine3D", rm.matRoadbedBackRightLine3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecRetainingWallNames", rm.vecRetainingWallNames);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matRetainingWallFrontLeftLine3D", rm.matRetainingWallFrontLeftLine3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matRetainingWallFrontRightLine3D", rm.matRetainingWallFrontRightLine3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matRetainingWallBackLeftLine3D", rm.matRetainingWallBackLeftLine3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matRetainingWallBackRightLine3D", rm.matRetainingWallBackRightLine3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecBridgeNames", rm.vecBridgeNames);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matBridgeCenterVertices3D", rm.matBridgeCenterVertices3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matBridgeGroundCenterVertices3D", rm.matBridgeGroundCenterVertices3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecTunnelNames", rm.vecTunnelNames);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matTunnelCenterVertices3D", rm.matTunnelCenterVertices3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecRailtrackNames", rm.vecRailtrackNames);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matRailtrackCenterVertices3D", rm.matRailtrackCenterVertices3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.vecStationNames", rm.vecStationNames);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.matStationCenterVertices3D", rm.matStationCenterVertices3D);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.nFlagChanged", rm.nFlagChanged);
	LoadObjectFromXml(data, ss, "Scheme.RenderModel.nFlagValid", rm.nFlagValid);

}
void SchemeSerializer::Save(char* filepath)
{
	std::stringstream ss;
	ss.precision(10);
	jx::message_queue::JxMessageData data;

	SchemeInitModel&   im = m_pOutModel->m_initModel;
	SchemeDesignModel& dm = m_pOutModel->m_designModel;
	SchemeAnalysModel& am = m_pOutModel->m_analysModel;
	SchemeRenderModel& rm = m_pOutModel->m_renderModel;

	SaveObjectToXml(data, ss, "Scheme.InitModel.nFlagChanged", im.nFlagChanged);
	SaveObjectToXml(data, ss, "Scheme.InitModel.nFlagValid", im.nFlagValid);
	SaveObjectToXml(data, ss, "Scheme.InitModel.nRailLineNum", im.nRailLineNum);
	SaveObjectToXml(data, ss, "Scheme.InitModel.fRailLineInteral", im.fRailLineInteral);
	SaveObjectToXml(data, ss, "Scheme.InitModel.fRoadWidth", im.fRoadWidth);
	SaveObjectToXml(data, ss, "Scheme.InitModel.fStartLicheng", im.fStartLicheng);
	SaveObjectToXml(data, ss, "Scheme.InitModel.strRoadType", im.strRoadType);
	SaveObjectToXml(data, ss, "Scheme.InitModel.pt3DCenter", im.pt3DCenter);
	SaveObjectToXml(data, ss, "Scheme.InitModel.vecSelectLinePoints3D", im.vecSelectLinePoints3D);
	SaveObjectToXml(data, ss, "Scheme.InitModel.vecTerrainNames", im.vecTerrainNames);
	SaveObjectToXml(data, ss, "Scheme.InitModel.matTerrainPoints3D", im.matTerrainPoints3D);
	
	SaveObjectToXml(data, ss, "Scheme.DesignModel.nFlagChanged", dm.nFlagChanged);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.nFlagValid", dm.nFlagValid);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.vecTin", dm.vecTin);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.vecDem", dm.vecDem);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.vecSectionType", dm.vecSectionType);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.vecSectionLicheng", dm.vecSectionLicheng);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.vecSectionCenterPoints3D", dm.vecSectionCenterPoints3D);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.vecRoadbedSecIndex", dm.vecRoadbedSecIndex);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.vecRoadbedLicheng", dm.vecRoadbedLicheng);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.vecRoadbedLuji2D", dm.vecRoadbedLuji2D);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.vecRoadbedCenter3D", dm.vecRoadbedCenter3D);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.matRoadbedLeftLine2D", dm.matRoadbedLeftLine2D);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.matRoadbedRightLine2D", dm.matRoadbedRightLine2D);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.matRetainingWallLeftLine2D", dm.matRetainingWallLeftLine2D);
	SaveObjectToXml(data, ss, "Scheme.DesignModel.matRetainingWallRightLine2D", dm.matRetainingWallRightLine2D);
	
	SaveObjectToXml(data, ss, "Scheme.AnalysModel.nFlagChanged", am.nFlagChanged);
	SaveObjectToXml(data, ss, "Scheme.AnalysModel.nFlagValid", am.nFlagValid);
	SaveObjectToXml(data, ss, "Scheme.AnalysModel.matTerrainConstraintPoints3D", am.matTerrainConstraintPoints3D);
	SaveObjectToXml(data, ss, "Scheme.AnalysModel.vecTinWithConstraints", am.vecTinWithConstraints);
	SaveObjectToXml(data, ss, "Scheme.AnalysModel.vecSectionDirectionPoints", am.vecSectionDirectionPoints);
	SaveObjectToXml(data, ss, "Scheme.AnalysModel.vecSectionNormPoints", am.vecSectionNormPoints);
	SaveObjectToXml(data, ss, "Scheme.AnalysModel.matRoadbedLeftInterpLine2D", am.matRoadbedLeftInterpLine2D);
	SaveObjectToXml(data, ss, "Scheme.AnalysModel.matRoadbedRightInterpLine2D", am.matRoadbedRightInterpLine2D);
	
	SaveObjectToXml(data, ss, "Scheme.RenderModel.nFlagChanged", rm.nFlagChanged);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.nFlagValid", rm.nFlagValid);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecTerrainNames", rm.vecTerrainNames);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecTerrainTextureFile", rm.vecTerrainTextureFile);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecTerrainDem", rm.vecTerrainDem);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecRoadbedNames", rm.vecRoadbedNames);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecRoadbedTextures", rm.vecRoadbedTextures);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.strRoadbedLeftBianpoTextureFile", rm.strRoadbedLeftBianpoTextureFile);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.strRoadbedRightBianpoTextureFile", rm.strRoadbedRightBianpoTextureFile);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.strRoadbedPingtaiTextureFile", rm.strRoadbedPingtaiTextureFile);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.strRoadbedLujiTextureFile", rm.strRoadbedLujiTextureFile);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecRoadbedTypeLeftTexIndex", rm.vecRoadbedTypeLeftTexIndex);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecRoadbedTypeRightTexIndex", rm.vecRoadbedTypeRightTexIndex);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.strRetainingWallTexture", rm.strRetainingWallTexture);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matRoadbedFrontLeftLine3D", rm.matRoadbedFrontLeftLine3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matRoadbedFrontRightLine3D", rm.matRoadbedFrontRightLine3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matRoadbedBackLeftLine3D", rm.matRoadbedBackLeftLine3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matRoadbedBackRightLine3D", rm.matRoadbedBackRightLine3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecRetainingWallNames", rm.vecRetainingWallNames);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matRetainingWallFrontLeftLine3D", rm.matRetainingWallFrontLeftLine3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matRetainingWallFrontRightLine3D", rm.matRetainingWallFrontRightLine3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matRetainingWallBackLeftLine3D", rm.matRetainingWallBackLeftLine3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matRetainingWallBackRightLine3D", rm.matRetainingWallBackRightLine3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecBridgeNames", rm.vecBridgeNames);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matBridgeCenterVertices3D", rm.matBridgeCenterVertices3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matBridgeGroundCenterVertices3D", rm.matBridgeGroundCenterVertices3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecTunnelNames", rm.vecTunnelNames);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matTunnelCenterVertices3D", rm.matTunnelCenterVertices3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecRailtrackNames", rm.vecRailtrackNames);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matRailtrackCenterVertices3D", rm.matRailtrackCenterVertices3D);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.vecStationNames", rm.vecStationNames);
	SaveObjectToXml(data, ss, "Scheme.RenderModel.matStationCenterVertices3D", rm.matStationCenterVertices3D);
	

	data.SaveToFile(filepath);

}

void SchemeSerializer::LoadFromString( char* buffer )
{

}

void SchemeSerializer::SaveToString( char* buffer )
{

}

}}}}}