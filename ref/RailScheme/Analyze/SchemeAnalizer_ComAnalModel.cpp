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
	////////////////////////////// �����������Դ///////////////////////////
	/****************************  analysis�����б� ***********************/
	//int nRailLineNum; // ��˫��
	//float fRailLineInteral; // ����
	//float fRoadWidth; // ��·��
	//float fStartLicheng; // ��ʼ���
	//std::string strRoadType; // "railway" / "highway"
	//// ѡ����Ƶĵ�
	//jx::data_type::Point3fArray vecSelectLinePoints3D;
	//// ��һ���ǵر������ǵز�
	//std::vector<std::string> vecTerrainNames;
	//std::vector<jx::data_type::Point3fArray> matTerrainPoints3D;
	
	/****************************  analysis�����б� ***********************/
	//// ·�β���������3��������size()��ͬ
	//std::vector<RailSectionType> vecSectionType;
	//std::vector<jx::data_type::Point2f> vecSectionLicheng; // Point2������������·�ε���ֹ��
	//std::vector<jx::data_type::Point3f> vecSectionCenterPoints3D;
	//// ·������, ����6��������size()��ͬ
	//std::vector<int> vecRoadbedSecIndex; // ·��������·������
	//std::vector<jx::data_type::LuJi> vecRoadbedLuji2D; // ��������
	//std::vector<jx::data_type::Point3f> vecRoadbedCenter3D; // ·������ά�еĲο���
	//std::vector<jx::data_type::Point2fArray> matRoadbedLeftLine2D; // �������ߣ����е㿪ʼ���������Ϊ0
	//std::vector<jx::data_type::Point2fArray> matRoadbedRightLine3D;// �Һ������
	//// ����ǽ����, �ο���ͬ·��
	//std::vector<jx::data_type::Point2fArray> vecRetainingWallLeftLine2D;
	//std::vector<jx::data_type::Point2fArray> vecRetainingWallRightLine2D;


	// ������ABC, ��֪A��B��C��AB�м��AB����AC�м��AC
	Point3f ComputeInterpPoint(Point3f &ptA, Point3f &ptAB, Point3f &ptB, Point3f &ptC)
	{
		float lamda = (ptAB.x - ptA.x) / (ptB.x - ptA.x);
		Point3f ptAC = ptA + (ptC - ptA) * lamda;
		return ptAC;
	}

	// Ϊ�˷�ֹ���ڵ�����·���������ͻ�䣬�ò�ֵ����vecLineInterp
	void InterplateLine(Point3Array *vecLine, Point3Array *vecLineNext, Point3Array *vecLineInterp, int nLeftRight) // �Ѳ��pƽރ�ı��εĵ㻻�ɗ��λ���������
	{
		// ��ȡ������һ���͸��̵�
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

	// ��designModel����AnalysisModel��������������֧��Render����
	void SchemeAnalyzer::ComputeAnalysisModel()
	{
		SchemeInitModel&   im = m_pOutModel->m_initModel;
		SchemeDesignModel& dm = m_pOutModel->m_designModel;
		SchemeAnalysModel& am = m_pOutModel->m_analysModel;

		// ·�β���--·�η��������
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
		/**************************** �����㣺analysis�����б� ***********************/
		//// ·������
		//std::vector<jx::data_type::Point3Array> matRoadbedLeftInterpLine2D; // ��� Ϊ�˺���һ��·����������ӵĲ�ֵ��
		//std::vector<jx::data_type::Point3Array> matRoadbedRightInterpLine2D; // �Ҳ� Ϊ�˺���һ��·����������ӵĲ�ֵ��
		
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
		/**************************** �����㣺analysis�����б� ***********************/
		//// ��غ͵ز�Ĳ���������2��������size()��ͬ
		//std::vector<jx::data_type::Point3Array> matTerrainConstraintPoints3D;
		//std::vector<jx::kernel3d::engine::utils::CTin> vecTin; // TIN�߳�ģ��
		// Ŀǰֻ�������Լ���㣬�ز�Լ����̫����
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
		// ����Tin
		am.vecTinWithConstraints.clear();
		for (int i = 0; i < im.matTerrainPoints3D.size(); i++)
		{
			CTin tin;
			tin.AddVertices(&im.matTerrainPoints3D[i]);
			if (i == 0) // ����
				tin.AddConstraintVertices(vecGroundConstraints, ConstraintType_PolygonConstraint);
			tin.Update();
			am.vecTinWithConstraints.push_back(tin);
		}
		
	}



}}}}}