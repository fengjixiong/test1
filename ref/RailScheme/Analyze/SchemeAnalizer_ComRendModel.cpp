#include "Jx3DEnginePch.h"
#include "SchemeAnalizer.h"
#include "Example/RailScheme/Model/SchemeModel.h"
#include "Util/Tin2Dem/jxTin.h"
#include "Util/Tin2Dem/jxDem.h"

using namespace jx::kernel3d::example::railscheme::model;
using namespace jx::kernel3d::engine::utils;
using namespace jx::data_type;

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme{
namespace analyze{

// 这些本地临时函数在文件后面定义
void ComputeTerrain(SchemeInitModel& im, SchemeAnalysModel& am, SchemeRenderModel& rm);
void ComputeRoadbed(SchemeAnalysModel& am, SchemeRenderModel& rm);
void ComputeRetainingWall(SchemeDesignModel& dm, SchemeRenderModel& rm);
void ComputeBridgeTunnel(SchemeDesignModel& dm, SchemeRenderModel& rm);

// 实际上用(initModel + designModel + AnalysisModel)来计算renderModel
void SchemeAnalyzer::ComputeRenderModel()
{
	SchemeInitModel&   im = m_pOutModel->m_initModel;
	SchemeDesignModel& dm = m_pOutModel->m_designModel;
	SchemeAnalysModel& am = m_pOutModel->m_analysModel;
	SchemeRenderModel& rm = m_pOutModel->m_renderModel;

	ComputeTerrain(im, am, rm);
	ComputeRoadbed(am, rm);
	ComputeRetainingWall(dm, rm);
	ComputeBridgeTunnel(dm, rm);
}

void ComputeTerrain(SchemeInitModel& im, SchemeAnalysModel& am, SchemeRenderModel& rm)
{
	/**************************** 待计算：Render参数列表 ***********************/
	//// 地面点和地层点参数
	//std::vector<std::string> vecTerrainNames;
	//std::vector<std::string> vecTerrainTextureFile;
	//std::vector<jx::kernel3d::engine::utils::CDem> vecTerrainDem; // DEM高程模型
	rm.vecTerrainNames.clear();
	rm.vecTerrainTextureFile.clear();
	rm.vecTerrainDem.clear();
	for (int i = 0; i < im.vecTerrainNames.size(); i++)
	{
		rm.vecTerrainNames.push_back(im.vecTerrainNames[i]);
		if (i == 0)
			rm.vecTerrainTextureFile.push_back("texture/ground.jpg");
		else
			rm.vecTerrainTextureFile.push_back("texture/rocklayer.jpg");
		CDem dem;
		dem.FromTIN(&am.vecTinWithConstraints[i], 5, 5);
		rm.vecTerrainDem.push_back(dem);
	}
}

void ComputeRoadbed(SchemeAnalysModel& am, SchemeRenderModel& rm)
{
	/**************************** 待计算：Render参数列表 ***********************/
	//// 路基参数
	//std::vector<std::string> vecRoadbedTextures; // 3个纹理，0路面，1边坡，2平台
	//std::string strRoadbedLeftBianpoTextureFile; // 路面纹理文件图像
	//std::string strRoadbedRightBianpoTextureFile; // 路面纹理文件图像
	//std::string strRoadbedPingtaiTextureFile;// 平台纹理
	//std::string strRoadbedLujiTextureFile;   // 边坡纹理	
	rm.strRoadbedLujiTextureFile = "texture/daozha.bmp";
	rm.strRoadbedLeftBianpoTextureFile = "texture/hupoLeft.bmp";
	rm.strRoadbedRightBianpoTextureFile = "texture/hupoRight.bmp";
	rm.strRoadbedPingtaiTextureFile = "texture/pingtai.bmp";
	rm.vecRoadbedTextures.clear();
	rm.vecRoadbedTextures.push_back(rm.strRoadbedLujiTextureFile);
	rm.vecRoadbedTextures.push_back(rm.strRoadbedLeftBianpoTextureFile);
	rm.vecRoadbedTextures.push_back(rm.strRoadbedRightBianpoTextureFile);
	rm.vecRoadbedTextures.push_back(rm.strRoadbedPingtaiTextureFile);
	/**************************** 待计算：Render参数列表 ***********************/
	//// 路基参数
	//std::vector<RoadBedTextureMap> vecRoadbedTypeLeftTexIndex[9]; //0代表路面，1代表边坡，2代表平台，假定最高4级边坡
	//std::vector<RoadBedTextureMap> vecRoadbedTypeRightTexIndex[9]; //0代表路面，1代表边坡，2代表平台
	int type1[9] = {0, 2, 1, 2, 1, 2, 1, 2, 1 };
	for (uint i = 0; i < 9; i++)
		rm.vecRoadbedTypeLeftTexIndex[i] = type1[i];
	int type2[9] = {0, 3, 1, 3, 1, 3, 1, 3, 1 };
	for (uint i = 0; i < 9; i++)
		rm.vecRoadbedTypeRightTexIndex[i] = type2[i];
	/**************************** 待计算：Render参数列表 ***********************/
	//// 路基参数
	//std::vector<std::string> vecRoadbedNames;
	//std::vector<jx::data_type::Point3fArray> matRoadbedFrontLeftLine3D;// 前后轮廓线，绘制四边形或者三角形用
	//std::vector<jx::data_type::Point3fArray> matRoadbedFrontRightLine3D;
	//std::vector<jx::data_type::Point3fArray> matRoadbedBackLeftLine3D;
	//std::vector<jx::data_type::Point3fArray> matRoadbedBackRightLine3D;
	rm.vecRoadbedNames.clear();
	rm.matRoadbedFrontLeftLine3D.clear();
	rm.matRoadbedFrontRightLine3D.clear();
	rm.matRoadbedBackLeftLine3D.clear();
	rm.matRoadbedBackRightLine3D.clear();
	for (int i = 0; i < am.matRoadbedLeftInterpLine2D.size()-1; i++)
	{
		std::string name = "Roadbed" + std::to_string((long long)i);
		Point3Array& vecLeftFront2D = am.matRoadbedLeftInterpLine2D[i];
		Point3Array& vecRightFront2D = am.matRoadbedRightInterpLine2D[i];
		Point3Array& vecLeftBack2D = am.matRoadbedLeftInterpLine2D[i+1];
		Point3Array& vecRightBack2D = am.matRoadbedRightInterpLine2D[i+1];
		// 将轴线里程坐标系转换为大地3D坐标系
		Point3Array vecLeftFront3D;
		SchemeAnalyzer::Get3DPointsFrom2D(vecLeftFront2D, vecLeftFront3D);
		Point3Array vecRightFront3D;
		SchemeAnalyzer::Get3DPointsFrom2D(vecRightFront2D, vecRightFront3D);
		Point3Array vecLeftBack3D;
		SchemeAnalyzer::Get3DPointsFrom2D(vecLeftBack2D, vecLeftBack3D);
		Point3Array vecRightBack3D;
		SchemeAnalyzer::Get3DPointsFrom2D(vecRightBack2D, vecRightBack3D);
		rm.vecRoadbedNames.push_back(name);
		rm.matRoadbedFrontLeftLine3D.push_back(vecLeftFront3D);
		rm.matRoadbedFrontLeftLine3D.push_back(vecLeftFront3D);
		rm.matRoadbedFrontLeftLine3D.push_back(vecLeftFront3D);
		rm.matRoadbedFrontLeftLine3D.push_back(vecLeftFront3D);
	}
}

void ComputeRetainingWall(SchemeDesignModel& dm, SchemeRenderModel& rm)
{
	/**************************** 待计算：Render参数列表 ***********************/
	//// 挡土墙参数
	//std::vector<std::string> vecRetainingWallNames;
	//std::vector<jx::data_type::Point3fArray> vecRetainingWallFrontLeftLine3D;
	//std::vector<jx::data_type::Point3fArray> vecRetainingWallFrontRightLine3D;
	//std::vector<jx::data_type::Point3fArray> vecRetainingWallBackLeftLine3D;
	//std::vector<jx::data_type::Point3fArray> vecRetainingWallBackRightLine3D;
	rm.vecRetainingWallNames.clear();
	rm.matRetainingWallFrontLeftLine3D.clear();
	rm.matRetainingWallFrontRightLine3D.clear();
	rm.matRetainingWallBackLeftLine3D.clear();
	rm.matRetainingWallBackRightLine3D.clear();
	for (int i = 0; i < dm.matRetainingWallLeftLine2D.size()-1; i++)
	{
		std::string name = "RetainingWall" + std::to_string((long long)i);
		Point3Array& vecLeftFront2D = dm.matRetainingWallLeftLine2D[i];
		Point3Array& vecRightFront2D = dm.matRetainingWallRightLine2D[i];
		Point3Array& vecLeftBack2D = dm.matRetainingWallLeftLine2D[i+1];
		Point3Array& vecRightBack2D = dm.matRetainingWallRightLine2D[i+1];
		// 将轴线里程坐标系转换为大地3D坐标系
		Point3Array vecLeftFront3D;
		SchemeAnalyzer::Get3DPointsFrom2D(vecLeftFront2D, vecLeftFront3D);
		Point3Array vecRightFront3D;
		SchemeAnalyzer::Get3DPointsFrom2D(vecRightFront2D, vecRightFront3D);
		Point3Array vecLeftBack3D;
		SchemeAnalyzer::Get3DPointsFrom2D(vecLeftBack2D, vecLeftBack3D);
		Point3Array vecRightBack3D;
		SchemeAnalyzer::Get3DPointsFrom2D(vecRightBack2D, vecRightBack3D);
		rm.vecRetainingWallNames.push_back(name);
		rm.matRetainingWallFrontLeftLine3D.push_back(vecLeftFront3D);
		rm.matRetainingWallFrontLeftLine3D.push_back(vecLeftFront3D);
		rm.matRetainingWallFrontLeftLine3D.push_back(vecLeftFront3D);
		rm.matRetainingWallFrontLeftLine3D.push_back(vecLeftFront3D);
	}
}

void ComputeBridgeTunnel(SchemeDesignModel& dm, SchemeRenderModel& rm)
{
	/**************************** 待计算：Render参数列表 ***********************/
	//// 桥梁参数
	//std::vector<std::string> vecBridgeNames;
	//std::vector<jx::data_type::Point3fArray> matBridgeCenterVertices3D; // 桥梁对应的地面点
	//// 隧道参数
	//std::vector<std::string> vecTunnelNames;
	//std::vector<jx::data_type::Point3fArray> matTunnelCenterVertices3D;
	//// 铁轨参数
	//std::vector<std::string> vecRailtrackName;
	//std::vector<jx::data_type::Point3fArray> matRailtrackCenterVertices3D;
	//// 车站参数
	//std::vector<std::string> vecStationName;
	//std::vector<jx::data_type::Point3fArray> matStationCenterVertices3D;
	rm.vecBridgeNames.clear();
	rm.matBridgeCenterVertices3D.clear();
	rm.matBridgeGroundCenterVertices3D.clear();
	for (int i = 0; i < dm.vecSectionType.size(); i++)
	{
		if (i == dm.vecSectionType.size()-1)
			break;
		Point3Array vecPoints;
		vecPoints.push_back(dm.vecSectionCenterPoints3D[i]);
		vecPoints.push_back(dm.vecSectionCenterPoints3D[i+1]);
		switch (dm.vecSectionType[i])
		{
		case RailSectionType_Bridge:
			{
				std::string name = "Bridge" + std::to_string((long long)i);
				rm.matBridgeCenterVertices3D.push_back(vecPoints);
				break;
			}
		case RailSectionType_Tunnel:
			{
				std::string name = "Tunnel" + std::to_string((long long)i);
				rm.matTunnelCenterVertices3D.push_back(vecPoints);
				break;
			}
		case RailSectionType_Railway:
			{
				std::string name = "RailTrack" + std::to_string((long long)i);
				rm.matRailtrackCenterVertices3D.push_back(vecPoints);
				break;
			}
		case RailSectionType_Station:
			{
				std::string name = "Station" + std::to_string((long long)i);
				rm.matStationCenterVertices3D.push_back(vecPoints);
				break;
			}
		default:break;
		}
	}
}


}}}}}