/*
* 方案参数
* fengjx, 2017.9.17
*
* 设计的思想：
* 1.独立性，这个参数类是整个方案的设计结构，而且不同的参数之间具有
*   独立性，用上级参数可以计算下级参数，下级参数已经足够做相应的显示；
* 2.自定义，变量全部用STL或者自定义变量，不采用OSG变量体系，以摆脱OSG渲染依赖；
* 3.集中性，所有参数集中在这里，便于之后的更新、序列化和数据库操作。如果
*   参数分散在各种类中，则在做更新、序列化和数据库操作时会有很大困难
*
*/

#ifndef _SCHEMEMODEL_H
#define _SCHEMEMODEL_H

#include "Util/BasicStructures.h"
#include "Util/Tin2Dem/jxDem.h"
#include "Util/Tin2Dem/jxTin.h"
#include "Scheme/Model/ModelBase.h"
//#include "Example/RailScheme/Model/Items/GeometryModel.h"
//#include "Example/RailScheme/Model/Items/ObjectModel.h"

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme {
namespace model {

	enum RailSectionType
	{
		RailSectionType_Unknown = 0,
		RailSectionType_Railway,
		RailSectionType_Tunnel,
		RailSectionType_Bridge,
		RailSectionType_Station
	};
	#define SchemeLayerNum 3
	enum SchemeLayerType
	{
		SchemeLayerType_Terrain = 0,
		SchemeLayerType_Scheme,
		SchemeLayerType_Addin,
		SchemeLayerType_Unknown
	};

	///////////////////////////////////////////////////////////////////////////
	//                               方案初始参数
	///////////////////////////////////////////////////////////////////////////
	class SchemeInitModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		SchemeInitModel();
		~SchemeInitModel();
	public:
		int nRailLineNum; // 单双轨
		float fRailLineInteral; // 轨间距
		float fRoadWidth; // 铁路宽
		float fStartLicheng; // 起始里程
		std::string strRoadType; // "railway" / "highway"
		jx::data_type::Point3 pt3DCenter; // GPS坐标系原点,一般可以不赋值，除非需要将三维设计放到地图上
		// 选线设计的点--大地3D坐标
		jx::data_type::Point3fArray vecSelectLinePoints3D;
		// 第一个是地表，其余是地层--大地3D坐标
		std::vector<std::string> vecTerrainNames;
		std::vector<jx::data_type::Point3fArray> matTerrainPoints2D; // 以下二选一，最好是有3D，但是因为历史遗留问题，只有2D的数据
		std::vector<jx::data_type::Point3fArray> matTerrainPoints3D;
	};

	///////////////////////////////////////////////////////////////////////////
	//                               道路方案设计参数
	///////////////////////////////////////////////////////////////////////////
	class SchemeDesignModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		SchemeDesignModel();
		~SchemeDesignModel();
	public:
		// 地面参数
		std::vector<jx::kernel3d::engine::utils::CTin> vecTin;// TIN高程模型
		std::vector<jx::kernel3d::engine::utils::CDem> vecDem;// DEM高程模型 没有约束（遮挡）
		// 路段参数，以下3个变量的size()相同
		std::vector<int> vecSectionType; // 使用RailSectionType
		std::vector<jx::data_type::Point2f> vecSectionLicheng; // Point2的两个参数是路段的起止点
		std::vector<jx::data_type::Point3f> vecSectionCenterPoints3D; // 大地坐标系
		// 路基参数, 以下6个变量的size()相同
		std::vector<int> vecRoadbedSecIndex; // 路基所属的路段序数
		std::vector<float> vecRoadbedLicheng; // 每个路基的里程
		std::vector<jx::data_type::LuJi> vecRoadbedLuji2D; // 横断面参数
		std::vector<jx::data_type::Point3f> vecRoadbedCenter3D; // 路基在三维中的参考点
		std::vector<jx::data_type::Point3fArray> matRoadbedLeftLine2D; // 左横断面线，从中点开始，深度坐标为里程
		std::vector<jx::data_type::Point3fArray> matRoadbedRightLine2D;// 右横断面线
		// 挡土墙参数, 参考点同路基
		std::vector<jx::data_type::Point3fArray> matRetainingWallLeftLine2D;
		std::vector<jx::data_type::Point3fArray> matRetainingWallRightLine2D;
	};

	///////////////////////////////////////////////////////////////////////////
	//                               分析结果参数
	///////////////////////////////////////////////////////////////////////////
	class SchemeAnalysModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		SchemeAnalysModel();
		~SchemeAnalysModel();
	public:
		// 大地和地层的参数，以下2个变量的size()相同
		std::vector<jx::data_type::Point3Array> matTerrainConstraintPoints3D;
		std::vector<jx::kernel3d::engine::utils::CTin> vecTinWithConstraints; // TIN高程模型
		// 路段参数
		std::vector<jx::data_type::Point3> vecSectionDirectionPoints; // 与DesignModel的vecSectionPoints数目一致
		std::vector<jx::data_type::Point3> vecSectionNormPoints; // 路段法线向量

		// 路基参数
		std::vector<jx::data_type::Point3Array> matRoadbedLeftInterpLine2D; // 左侧 为了和下一个路基设计线连接的插值线
		std::vector<jx::data_type::Point3Array> matRoadbedRightInterpLine2D; // 右侧 为了和下一个路基设计线连接的插值线
		//std::vector<jx::data_type::Point3fArray> vecRoadbedPositionInfo3D; //前截面的法线，// 相邻的两个RoadBed的法线应该一致，需要根据Section的分配来定
	};

	///////////////////////////////////////////////////////////////////////////
	//                               渲染参数
	///////////////////////////////////////////////////////////////////////////
	class SchemeRenderModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		SchemeRenderModel();
		~SchemeRenderModel();
	public:
		// 地面点和地层点参数--用DEM的HightField绘制
		std::vector<std::string> vecTerrainNames;
		std::vector<std::string> vecTerrainTextureFile;
		std::vector<jx::kernel3d::engine::utils::CDem> vecTerrainDem; // DEM高程模型
		// 路基参数--用QuadStrip绘制
		std::vector<std::string> vecRoadbedNames;
		std::vector<std::string> vecRoadbedTextures; // 3个纹理，0路面，1边坡，2平台
		std::string strRoadbedLeftBianpoTextureFile; // 左边坡文件图像
		std::string strRoadbedRightBianpoTextureFile; // 右边坡纹理文件图像
		std::string strRoadbedPingtaiTextureFile;// 平台纹理
		std::string strRoadbedLujiTextureFile;   // 路面纹理
		std::vector<int> vecRoadbedTypeLeftTexIndex; //0代表路面，1代表边坡，2代表平台，假定最高4级边坡
		std::vector<int> vecRoadbedTypeRightTexIndex; //0代表路面，1代表边坡，2代表平台
		std::string strRetainingWallTexture;
		std::vector<jx::data_type::Point3fArray> matRoadbedFrontLeftLine3D;// 前后轮廓线，绘制四边形或者三角形用
		std::vector<jx::data_type::Point3fArray> matRoadbedFrontRightLine3D;
		std::vector<jx::data_type::Point3fArray> matRoadbedBackLeftLine3D;
		std::vector<jx::data_type::Point3fArray> matRoadbedBackRightLine3D;
		// 挡土墙参数--用多边形柱面绘制
		std::vector<std::string> vecRetainingWallNames;
		std::vector<jx::data_type::Point3fArray> matRetainingWallFrontLeftLine3D;
		std::vector<jx::data_type::Point3fArray> matRetainingWallFrontRightLine3D;
		std::vector<jx::data_type::Point3fArray> matRetainingWallBackLeftLine3D;
		std::vector<jx::data_type::Point3fArray> matRetainingWallBackRightLine3D;
		// 桥梁参数--用jxModelNode绘制
		std::vector<std::string> vecBridgeNames;
		std::vector<jx::data_type::Point3fArray> matBridgeCenterVertices3D;
		std::vector<jx::data_type::Point3fArray> matBridgeGroundCenterVertices3D; // 桥梁对应的地面点
		// 隧道参数
		std::vector<std::string> vecTunnelNames;
		std::vector<jx::data_type::Point3fArray> matTunnelCenterVertices3D;
		// 铁轨参数
		std::vector<std::string> vecRailtrackNames;
		std::vector<jx::data_type::Point3fArray> matRailtrackCenterVertices3D;
		// 车站参数
		std::vector<std::string> vecStationNames;
		std::vector<jx::data_type::Point3fArray> matStationCenterVertices3D;

	};

	class SchemeModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		SchemeModel();
	public:
		static SchemeModel* pInstance;
	public:
		SchemeInitModel	  m_initModel;
		SchemeDesignModel m_designModel;
		SchemeAnalysModel m_analysModel;
		SchemeRenderModel m_renderModel;
	};

}}}}}

#endif