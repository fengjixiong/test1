#include "Jx3DEnginePch.h"
#include "SchemeAnalizer.h"
#include "Util/BasicStructures.h"
#include "Example/RailScheme/Model/SchemeModel.h"

using namespace jx::kernel3d::example::railscheme::model;
using namespace jx::data_type;
using namespace jx::kernel3d::engine::utils;

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme{
namespace analyze{

	Point3fArray& GetDsignLine2D(LuJi data, float licheng, int nLeftRight);
	void DownSample(Point3fArray &ptsIn, Point3fArray &ptsOut, int nSample)
	{
		ptsOut.clear();
		if (nSample <= 0 || nSample == 1)
			return;

		for (uint i = 0; i < ptsIn.size(); i+=nSample)
			ptsOut.push_back(ptsIn[i]);
	}

	// 从InitModel计算DesignModel
	// 这部分工作黄总已经完成部分工作，可以直接用作2维显示
	void SchemeAnalyzer::ComputeDesignModel()
	{
		SchemeInitModel& im = m_pOutModel->m_initModel;
		SchemeDesignModel& dm = m_pOutModel->m_designModel;

		//// 路段参数，以下3个变量的size()相同
		//std::vector<RailSectionType> vecSectionType;
		//std::vector<jx::data_type::Point2f> vecSectionLicheng; // Point2的两个参数是路段的起止点
		//std::vector<jx::data_type::Point3f> vecSectionCenterPoints3D;
		
		//// 路基参数, 以下6个变量的size()相同
		//std::vector<int> vecRoadbedSecIndex; // 路基所属的路段序数
		//std::vector<jx::data_type::LuJi> vecRoadbedLuji2D; // 横断面参数
		//std::vector<jx::data_type::Point3f> vecRoadbedCenter3D; // 路基在三维中的参考点
		//std::vector<jx::data_type::Point2fArray> matRoadbedLeftLine2D; // 左横断面线，从中点开始，深度坐标为0
		int num = dm.vecRoadbedLicheng.size();
		if (dm.matRoadbedLeftLine2D.size() == 0)
		{
			for (int i = 0; i < num; i++)
			{
				float licheng = dm.vecRoadbedLicheng[i];
				LuJi& luji = dm.vecRoadbedLuji2D[i];
				Point3Array& vec = GetDsignLine2D(luji, licheng, 0);
				dm.matRoadbedLeftLine2D.push_back(vec);
			}
		}
		//std::vector<jx::data_type::Point2fArray> matRoadbedRightLine3D;// 右横断面线
		if (dm.matRoadbedRightLine2D.size() == 0)
		{
			for (int i = 0; i < num; i++)
			{
				float licheng = dm.vecRoadbedLicheng[i];
				LuJi& luji = dm.vecRoadbedLuji2D[i];
				Point3Array& vec = GetDsignLine2D(luji, licheng, 1);
				dm.matRoadbedRightLine2D.push_back(vec);
			}
		}
		
		//// 挡土墙参数, 参考点同路基
		//std::vector<jx::data_type::Point2fArray> vecRetainingWallLeftLine2D;
		//std::vector<jx::data_type::Point2fArray> vecRetainingWallRightLine2D;

		// 计算地面点--历史遗留问题，原本不应该有
		// 降采样，这里地形点每10米有30个点，太多，可以每5个点算一个19920->3984个
		if (im.matTerrainPoints3D.size()==0) // 需要根据2D地面点计算3D地面点
		{
			im.matTerrainPoints3D.clear();
			for (int i = 0; i < im.matTerrainPoints2D.size(); i++)
			{
				Point3Array vecPt2D;
				DownSample(im.matTerrainPoints2D[i], vecPt2D, 5);
				Point3Array vecPt3D;
				Get3DPointsFrom2D(vecPt2D, vecPt3D);
				im.matTerrainPoints3D.push_back(vecPt3D);
			}
		}

		// 计算路面
		dm.vecTin.clear();
		dm.vecDem.clear();
		for (int i = 0; i < im.matTerrainPoints3D.size(); i++)
		{
			CTin tin;
			tin.AddVertices(&im.matTerrainPoints3D[i]);
			tin.Update();
			dm.vecTin.push_back(tin);
			CDem dem;
			dem.FromTIN(&tin, 10.0, 10.0);
			dm.vecDem.push_back(dem);
		}
	}
	
	// 根据路基，计算设计线。设计线的y值是里程
	Point3fArray& GetDsignLine2D(LuJi data, float licheng, int nLeftRight)
	{
		Point3fArray vecLine;
		float	roadWidth = 0;	// 跿宿
		BianPo	bianpo;			// 跿堿
		float	leftFactor = 1; // 左侧的话x需要乘-1

		if (nLeftRight == 0) // 左侧
		{
			roadWidth = data.Left_RoadWidth;
			bianpo = data.Left_BianPo;
			leftFactor = -1;
		}
		else // 右侧
		{
			roadWidth = data.Right_RoadWidth;
			bianpo = data.Right_BianPo;
			leftFactor = 1;
		}
		Point3f pt1 = Point3f(0, licheng, 0);
		pt1 += Point3f(leftFactor*roadWidth, 0, 0);// OSG坐标系是x向右，y向里，z向上
		vecLine.push_back(pt1);

		for (uint i = 0; i < MAXBIANPO; i++)
		{
			float width = sqrt(fabs(bianpo.BianpoRate[i]*bianpo.BianpoRate[i] - bianpo.BianpoHeight[i]*bianpo.BianpoHeight[i]));
			if (width < MINFLOAT) 
				break;
			pt1 += Point3f(leftFactor*width, 0, bianpo.BianpoHeight[i]);
			vecLine.push_back(pt1);
			if (bianpo.BianpoPingtaiWidth[i] < MINFLOAT) 
				break;
			pt1 += Point3f(leftFactor*bianpo.BianpoPingtaiWidth[i], 0, 0);
			vecLine.push_back(pt1);
		}
		return vecLine;
	}

}}}}}