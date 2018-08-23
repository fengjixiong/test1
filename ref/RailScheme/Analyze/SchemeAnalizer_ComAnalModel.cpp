#include "Jx3DEnginePch.h"
#include "SchemeAnalizer.h"
#include "Example/RailScheme/Model/SchemeModel.h"
#include "Util/BasicStructures.h"

using namespace jx::data_type;
using namespace jx::kernel3d::example::railscheme::model;
using namespace jx::kernel3d::engine::utils;

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme{
namespace analyze{
	////////////////////////////// 输入参数的来源///////////////////////////
	/****************************  analysis参数列表 ***********************/
	//int nRailLineNum; // 单双轨
	//float fRailLineInteral; // 轨间距
	//float fRoadWidth; // 铁路宽
	//float fStartLicheng; // 起始里程
	//std::string strRoadType; // "railway" / "highway"
	//// 选线设计的点
	//jx::data_type::Point3fArray vecSelectLinePoints3D;
	//// 第一个是地表，其余是地层
	//std::vector<std::string> vecTerrainNames;
	//std::vector<jx::data_type::Point3fArray> matTerrainPoints3D;
	
	/****************************  analysis参数列表 ***********************/
	//// 路段参数，以下3个变量的size()相同
	//std::vector<RailSectionType> vecSectionType;
	//std::vector<jx::data_type::Point2f> vecSectionLicheng; // Point2的两个参数是路段的起止点
	//std::vector<jx::data_type::Point3f> vecSectionCenterPoints3D;
	//// 路基参数, 以下6个变量的size()相同
	//std::vector<int> vecRoadbedSecIndex; // 路基所属的路段序数
	//std::vector<jx::data_type::LuJi> vecRoadbedLuji2D; // 横断面参数
	//std::vector<jx::data_type::Point3f> vecRoadbedCenter3D; // 路基在三维中的参考点
	//std::vector<jx::data_type::Point2fArray> matRoadbedLeftLine2D; // 左横断面线，从中点开始，深度坐标为0
	//std::vector<jx::data_type::Point2fArray> matRoadbedRightLine3D;// 右横断面线
	//// 挡土墙参数, 参考点同路基
	//std::vector<jx::data_type::Point2fArray> vecRetainingWallLeftLine2D;
	//std::vector<jx::data_type::Point2fArray> vecRetainingWallRightLine2D;


	// 三角形ABC, 已知A，B，C和AB中间点AB，求AC中间点AC
	Point3f ComputeInterpPoint(Point3f &ptA, Point3f &ptAB, Point3f &ptB, Point3f &ptC)
	{
		float lamda = (ptAB.x - ptA.x) / (ptB.x - ptA.x);
		Point3f ptAC = ptA + (ptC - ptA) * lamda;
		return ptAC;
	}

	// 为了防止相邻的两个路基设计线有突变，用插值计算vecLineInterp
	void InterplateLine(Point3Array *vecLine, Point3Array *vecLineNext, Point3Array *vecLineInterp, int nLeftRight) // 把不昿平迌四边形的点换成梿形或者三角形
	{
		// 获取更长的一方和更短的
		Point3Array* longVec = NULL; int longLen = 0;
		Point3Array* shortVec = NULL; int shortLen = 0;
		if (vecLine->size() > vecLineNext->size())
		{
			longVec = vecLine; shortVec = vecLineNext;
			longLen = vecLine->size()-1; shortLen = vecLineNext->size()-1;
		}
		else if (vecLine->size() < vecLineNext->size())
		{
			longVec = vecLineNext; shortVec = vecLine;
			longLen = vecLineNext->size()-1; shortLen = vecLine->size()-1;
		}
		//std::assert(longLen>=0 && shortLen>=0);
		// 
		if (longLen != shortLen)
		{
			Point3f ptA = INDEX(longVec,longLen);
			Point3f ptB = INDEX(longVec,shortLen);
			Point3f ptC = INDEX(shortVec,shortLen);
			for (int i = shortLen+1; i < longLen; i++)
			{
				Point3f ptAB = INDEX(longVec,i);
				Point3f ptAC = ComputeInterpPoint(ptA, ptAB, ptB, ptC);
				vecLineInterp->push_back(ptAC);
				//vecConstraints->push_back(ptAC);
			}
			Point3f ptA_near = ptA + (ptC - ptA)*0.1;
			vecLineInterp->push_back(ptA_near);
		}
	}

	// 从designModel计算AnalysisModel，分析参数用于支撑Render参数
	void SchemeAnalyzer::ComputeAnalysisModel()
	{
		SchemeInitModel&   im = m_pOutModel->m_initModel;
		SchemeDesignModel& dm = m_pOutModel->m_designModel;
		SchemeAnalysModel& am = m_pOutModel->m_analysModel;

		// 路段参数--路段方向，做差分
		am.vecSectionDirectionPoints.clear();
		for (int i = 0; i < dm.vecSectionCenterPoints3D.size(); i++)
		{
			if (i == dm.vecSectionCenterPoints3D.size()-1)
			{
				am.vecSectionDirectionPoints.push_back(am.vecSectionDirectionPoints.back());
				break;
			}
			Point3 pt1 = dm.vecSectionCenterPoints3D[i];
			Point3 pt2 = dm.vecSectionCenterPoints3D[i+1];
			Point3 ptDirect = pt2 - pt1;
			ptDirect = ptDirect.normalize();
			am.vecSectionDirectionPoints.push_back(ptDirect);
			Point3 ptNorm = Point3(ptDirect.y, -1*ptDirect.x, ptDirect.z);
			am.vecSectionNormPoints.push_back(ptNorm);
		}
		/**************************** 待计算：analysis参数列表 ***********************/
		//// 路基参数
		//std::vector<jx::data_type::Point3Array> matRoadbedLeftInterpLine2D; // 左侧 为了和下一个路基设计线连接的插值线
		//std::vector<jx::data_type::Point3Array> matRoadbedRightInterpLine2D; // 右侧 为了和下一个路基设计线连接的插值线
		
		am.matRoadbedLeftInterpLine2D.clear();
		for (int i = 0; i < dm.matRoadbedLeftLine2D.size()-1; i++)
		{
			Point3fArray vecPt;
			InterplateLine(&dm.matRoadbedLeftLine2D[i], &dm.matRoadbedLeftLine2D[i+1], &vecPt, 0);
			am.matRoadbedLeftInterpLine2D.push_back(vecPt);
		}
		
		am.matRoadbedRightInterpLine2D.clear();
		for (int i = 0; i < dm.matRoadbedRightLine2D.size()-1; i++)
		{
			Point3fArray vecPt;
			InterplateLine(&dm.matRoadbedRightLine2D[i], &dm.matRoadbedRightLine2D[i+1], &vecPt, 0);
			am.matRoadbedRightInterpLine2D.push_back(vecPt);
		}
		/**************************** 待计算：analysis参数列表 ***********************/
		//// 大地和地层的参数，以下2个变量的size()相同
		//std::vector<jx::data_type::Point3Array> matTerrainConstraintPoints3D;
		//std::vector<jx::kernel3d::engine::utils::CTin> vecTin; // TIN高程模型
		// 目前只计算地面约束点，地层约束点太难算
		am.matTerrainConstraintPoints3D.clear();
		Point3Array vecGroundConstraints;
		for (int i = 0; i < am.matRoadbedLeftInterpLine2D.size(); i++)
			vecGroundConstraints.push_back(am.matRoadbedLeftInterpLine2D[i].back());
		for (int i = am.matRoadbedRightInterpLine2D.size()-1; i >=0 ; i--)
			vecGroundConstraints.push_back(am.matRoadbedRightInterpLine2D[i].back());
		am.matTerrainConstraintPoints3D.push_back(vecGroundConstraints);
		for (int i = 1; i < im.vecTerrainNames.size(); i++)
		{
			Point3Array newPoints;
			am.matTerrainConstraintPoints3D.push_back(newPoints);
		}
		// 计算Tin
		am.vecTinWithConstraints.clear();
		for (int i = 0; i < im.matTerrainPoints3D.size(); i++)
		{
			CTin tin;
			tin.AddVertices(&im.matTerrainPoints3D[i]);
			if (i == 0) // 地面
				tin.AddConstraintVertices(vecGroundConstraints, ConstraintType_PolygonConstraint);
			tin.Update();
			am.vecTinWithConstraints.push_back(tin);
		}
		
	}



}}}}}