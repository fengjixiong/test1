/*
* ��������
* fengjx, 2017.9.17
*
* ��Ƶ�˼�룺
* 1.�����ԣ������������������������ƽṹ�����Ҳ�ͬ�Ĳ���֮�����
*   �����ԣ����ϼ��������Լ����¼��������¼������Ѿ��㹻����Ӧ����ʾ��
* 2.�Զ��壬����ȫ����STL�����Զ��������������OSG������ϵ���԰���OSG��Ⱦ������
* 3.�����ԣ����в����������������֮��ĸ��¡����л������ݿ���������
*   ������ɢ�ڸ������У����������¡����л������ݿ����ʱ���кܴ�����
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
	//                               ������ʼ����
	///////////////////////////////////////////////////////////////////////////
	class SchemeInitModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		SchemeInitModel();
		~SchemeInitModel();
	public:
		int nRailLineNum; // ��˫��
		float fRailLineInteral; // ����
		float fRoadWidth; // ��·��
		float fStartLicheng; // ��ʼ���
		std::string strRoadType; // "railway" / "highway"
		jx::data_type::Point3 pt3DCenter; // GPS����ϵԭ��,һ����Բ���ֵ��������Ҫ����ά��Ʒŵ���ͼ��
		// ѡ����Ƶĵ�--���3D����
		jx::data_type::Point3fArray vecSelectLinePoints3D;
		// ��һ���ǵر������ǵز�--���3D����
		std::vector<std::string> vecTerrainNames;
		std::vector<jx::data_type::Point3fArray> matTerrainPoints2D; // ���¶�ѡһ���������3D��������Ϊ��ʷ�������⣬ֻ��2D������
		std::vector<jx::data_type::Point3fArray> matTerrainPoints3D;
	};

	///////////////////////////////////////////////////////////////////////////
	//                               ��·������Ʋ���
	///////////////////////////////////////////////////////////////////////////
	class SchemeDesignModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		SchemeDesignModel();
		~SchemeDesignModel();
	public:
		// �������
		std::vector<jx::kernel3d::engine::utils::CTin> vecTin;// TIN�߳�ģ��
		std::vector<jx::kernel3d::engine::utils::CDem> vecDem;// DEM�߳�ģ�� û��Լ�����ڵ���
		// ·�β���������3��������size()��ͬ
		std::vector<int> vecSectionType; // ʹ��RailSectionType
		std::vector<jx::data_type::Point2f> vecSectionLicheng; // Point2������������·�ε���ֹ��
		std::vector<jx::data_type::Point3f> vecSectionCenterPoints3D; // �������ϵ
		// ·������, ����6��������size()��ͬ
		std::vector<int> vecRoadbedSecIndex; // ·��������·������
		std::vector<float> vecRoadbedLicheng; // ÿ��·�������
		std::vector<jx::data_type::LuJi> vecRoadbedLuji2D; // ��������
		std::vector<jx::data_type::Point3f> vecRoadbedCenter3D; // ·������ά�еĲο���
		std::vector<jx::data_type::Point3fArray> matRoadbedLeftLine2D; // �������ߣ����е㿪ʼ���������Ϊ���
		std::vector<jx::data_type::Point3fArray> matRoadbedRightLine2D;// �Һ������
		// ����ǽ����, �ο���ͬ·��
		std::vector<jx::data_type::Point3fArray> matRetainingWallLeftLine2D;
		std::vector<jx::data_type::Point3fArray> matRetainingWallRightLine2D;
	};

	///////////////////////////////////////////////////////////////////////////
	//                               �����������
	///////////////////////////////////////////////////////////////////////////
	class SchemeAnalysModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		SchemeAnalysModel();
		~SchemeAnalysModel();
	public:
		// ��غ͵ز�Ĳ���������2��������size()��ͬ
		std::vector<jx::data_type::Point3Array> matTerrainConstraintPoints3D;
		std::vector<jx::kernel3d::engine::utils::CTin> vecTinWithConstraints; // TIN�߳�ģ��
		// ·�β���
		std::vector<jx::data_type::Point3> vecSectionDirectionPoints; // ��DesignModel��vecSectionPoints��Ŀһ��
		std::vector<jx::data_type::Point3> vecSectionNormPoints; // ·�η�������

		// ·������
		std::vector<jx::data_type::Point3Array> matRoadbedLeftInterpLine2D; // ��� Ϊ�˺���һ��·����������ӵĲ�ֵ��
		std::vector<jx::data_type::Point3Array> matRoadbedRightInterpLine2D; // �Ҳ� Ϊ�˺���һ��·����������ӵĲ�ֵ��
		//std::vector<jx::data_type::Point3fArray> vecRoadbedPositionInfo3D; //ǰ����ķ��ߣ�// ���ڵ�����RoadBed�ķ���Ӧ��һ�£���Ҫ����Section�ķ�������
	};

	///////////////////////////////////////////////////////////////////////////
	//                               ��Ⱦ����
	///////////////////////////////////////////////////////////////////////////
	class SchemeRenderModel : public jx::kernel3d::engine::scheme::model::ModelBase
	{
	public:
		SchemeRenderModel();
		~SchemeRenderModel();
	public:
		// �����͵ز�����--��DEM��HightField����
		std::vector<std::string> vecTerrainNames;
		std::vector<std::string> vecTerrainTextureFile;
		std::vector<jx::kernel3d::engine::utils::CDem> vecTerrainDem; // DEM�߳�ģ��
		// ·������--��QuadStrip����
		std::vector<std::string> vecRoadbedNames;
		std::vector<std::string> vecRoadbedTextures; // 3������0·�棬1���£�2ƽ̨
		std::string strRoadbedLeftBianpoTextureFile; // ������ļ�ͼ��
		std::string strRoadbedRightBianpoTextureFile; // �ұ��������ļ�ͼ��
		std::string strRoadbedPingtaiTextureFile;// ƽ̨����
		std::string strRoadbedLujiTextureFile;   // ·������
		std::vector<int> vecRoadbedTypeLeftTexIndex; //0����·�棬1������£�2����ƽ̨���ٶ����4������
		std::vector<int> vecRoadbedTypeRightTexIndex; //0����·�棬1������£�2����ƽ̨
		std::string strRetainingWallTexture;
		std::vector<jx::data_type::Point3fArray> matRoadbedFrontLeftLine3D;// ǰ�������ߣ������ı��λ�����������
		std::vector<jx::data_type::Point3fArray> matRoadbedFrontRightLine3D;
		std::vector<jx::data_type::Point3fArray> matRoadbedBackLeftLine3D;
		std::vector<jx::data_type::Point3fArray> matRoadbedBackRightLine3D;
		// ����ǽ����--�ö�����������
		std::vector<std::string> vecRetainingWallNames;
		std::vector<jx::data_type::Point3fArray> matRetainingWallFrontLeftLine3D;
		std::vector<jx::data_type::Point3fArray> matRetainingWallFrontRightLine3D;
		std::vector<jx::data_type::Point3fArray> matRetainingWallBackLeftLine3D;
		std::vector<jx::data_type::Point3fArray> matRetainingWallBackRightLine3D;
		// ��������--��jxModelNode����
		std::vector<std::string> vecBridgeNames;
		std::vector<jx::data_type::Point3fArray> matBridgeCenterVertices3D;
		std::vector<jx::data_type::Point3fArray> matBridgeGroundCenterVertices3D; // ������Ӧ�ĵ����
		// �������
		std::vector<std::string> vecTunnelNames;
		std::vector<jx::data_type::Point3fArray> matTunnelCenterVertices3D;
		// �������
		std::vector<std::string> vecRailtrackNames;
		std::vector<jx::data_type::Point3fArray> matRailtrackCenterVertices3D;
		// ��վ����
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