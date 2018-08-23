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

	// ��InitModel����DesignModel
	// �ⲿ�ֹ��������Ѿ���ɲ��ֹ���������ֱ������2ά��ʾ
	void SchemeAnalyzer::ComputeDesignModel()
	{
		SchemeInitModel& im = m_pOutModel->m_initModel;
		SchemeDesignModel& dm = m_pOutModel->m_designModel;

		//// ·�β���������3��������size()��ͬ
		//std::vector<RailSectionType> vecSectionType;
		//std::vector<jx::data_type::Point2f> vecSectionLicheng; // Point2������������·�ε���ֹ��
		//std::vector<jx::data_type::Point3f> vecSectionCenterPoints3D;
		
		//// ·������, ����6��������size()��ͬ
		//std::vector<int> vecRoadbedSecIndex; // ·��������·������
		//std::vector<jx::data_type::LuJi> vecRoadbedLuji2D; // ��������
		//std::vector<jx::data_type::Point3f> vecRoadbedCenter3D; // ·������ά�еĲο���
		//std::vector<jx::data_type::Point2fArray> matRoadbedLeftLine2D; // �������ߣ����е㿪ʼ���������Ϊ0
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
		//std::vector<jx::data_type::Point2fArray> matRoadbedRightLine3D;// �Һ������
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
		
		//// ����ǽ����, �ο���ͬ·��
		//std::vector<jx::data_type::Point2fArray> vecRetainingWallLeftLine2D;
		//std::vector<jx::data_type::Point2fArray> vecRetainingWallRightLine2D;

		// ��������--��ʷ�������⣬ԭ����Ӧ����
		// ��������������ε�ÿ10����30���㣬̫�࣬����ÿ5������һ��19920->3984��
		if (im.matTerrainPoints3D.size()==0) // ��Ҫ����2D��������3D�����
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

		// ����·��
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
	
	// ����·������������ߡ�����ߵ�yֵ�����
	Point3fArray& GetDsignLine2D(LuJi data, float licheng, int nLeftRight)
	{
		Point3fArray vecLine;
		float	roadWidth = 0;	// �T��
		BianPo	bianpo;			// �T�A
		float	leftFactor = 1; // ���Ļ�x��Ҫ��-1

		if (nLeftRight == 0) // ���
		{
			roadWidth = data.Left_RoadWidth;
			bianpo = data.Left_BianPo;
			leftFactor = -1;
		}
		else // �Ҳ�
		{
			roadWidth = data.Right_RoadWidth;
			bianpo = data.Right_BianPo;
			leftFactor = 1;
		}
		Point3f pt1 = Point3f(0, licheng, 0);
		pt1 += Point3f(leftFactor*roadWidth, 0, 0);// OSG����ϵ��x���ң�y���z����
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