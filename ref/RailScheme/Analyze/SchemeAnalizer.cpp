#include "Jx3DEnginePch.h"
#include "SchemeAnalizer.h"
#include "Example/RailScheme/Model/SchemeModel.h"

using namespace jx::kernel3d::example::railscheme::model;
using namespace jx::kernel3d::engine::utils;
using namespace jx::data_type;

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme{
namespace analyze{

SchemeAnalyzer::SchemeAnalyzer(SchemeModel* m )
{
	m_pOutModel = m;
}

SchemeAnalyzer::~SchemeAnalyzer()
{

}

void SchemeAnalyzer::Analyze()
{
	SchemeInitModel& im = m_pOutModel->m_initModel;
	SchemeDesignModel& dm = m_pOutModel->m_designModel;
	SchemeAnalysModel& am = m_pOutModel->m_analysModel;
	SchemeRenderModel& rm = m_pOutModel->m_renderModel;

	// �ӳ�ʼ�����������Ʋ���
	if ((!dm.nFlagValid) || dm.nFlagChanged)
	{
		// ����������������
		this->ComputeDesignModel();
		// �޸�״̬
		dm.nFlagChanged = 1;
		dm.nFlagValid = 0xFFFF;
		dm.nFlagChanged = 0;
	}
	// ����Ʋ����������������
	if ((!am.nFlagValid) || am.nFlagChanged)
	{
		// ����������������
		this->ComputeAnalysisModel();
		// �޸�״̬
		am.nFlagChanged = 1;
		am.nFlagValid = 0xFFFF;
		am.nFlagChanged = 0;
	}
	// �ӷ��������������Ⱦ����
	if ((!rm.nFlagValid) || rm.nFlagChanged)
	{
		// �����������ӻ�����
		this->ComputeRenderModel();
		// �޸�״̬
		rm.nFlagChanged = 1; // ��SchemeView��Ⱦ������ᱻ��0
		rm.nFlagValid = 0xFFFF;
		rm.nFlagChanged = 0;
	}
}

// �����⺯���ڱ��cpp�ļ���
// ComputeDesignModel()
// ComputeAnalysisModel()
// ComputeRenderModel()

//float fStartLicheng; // ��ʼ���
//Point3 pt3DCenter; // �������ϵԭ��
//Point3fArray *vecSelectLinePoints3D;

// ��ȡ��ǰ������һ��·��,��DesignModel�����֮������á�
int SchemeAnalyzer::GetSectionIndex(float licheng)
{
	SchemeDesignModel& dm = SchemeModel::pInstance->m_designModel;

	float currLicheng = licheng; //��ǰ�����
	int secIndex = dm.vecSectionLicheng.size(); 
	for (int i = 0; i < dm.vecSectionLicheng.size(); i++)
		if (dm.vecSectionLicheng[i].y > currLicheng)
			return i;
	return dm.vecSectionLicheng.size();
}
// ��ȡ��̶�Ӧ������3D�� ��AnalysModel�����֮������á�
jx::data_type::Point3 SchemeAnalyzer::Get3DCenterPointFromLicheng( float licheng )
{
	SchemeDesignModel& dm = SchemeModel::pInstance->m_designModel;
	//if (0 == dm.nFlagValid || 0 == am.nFlagValid)
	//	return Point3(0,0,0);

	int secIndex = GetSectionIndex(licheng); // ���ڵڼ���·��
	if (secIndex >= dm.vecSectionCenterPoints3D.size()-1)
		secIndex = dm.vecSectionCenterPoints3D.size()-2;
	Point3 pt1 = dm.vecSectionCenterPoints3D[secIndex];
	Point3 pt2 = dm.vecSectionCenterPoints3D[secIndex+1];
	Point3 direction = (pt2 - pt1).normalize();
	
	Point2 sectionLicheng2D = dm.vecSectionLicheng[secIndex]; // ��ǰ·�ε���ʼ���
	Point3 ptSecStart3D = dm.vecSectionCenterPoints3D[secIndex];
	Point3 ptSecEnd3D = dm.vecSectionCenterPoints3D[secIndex+1];
	float currLicheng = licheng;
	float rate = (currLicheng - sectionLicheng2D.x) / (sectionLicheng2D.y - sectionLicheng2D.x);
	//���Բ�ֵ����·����߶�Ӧ3D�㣬�����ʵ������2D�ռ�����(0, licheng, 0)
	Point3 center3D = ptSecStart3D + (ptSecEnd3D - ptSecStart3D) * rate; 
	return center3D;
}
// ����pt2D�������߷���Ϊy��,y ֵΪ��̡�AnalysModel�����֮������á�
Point3 SchemeAnalyzer::Get3DPointFrom2D( Point3 pt2D )
{
	SchemeAnalysModel& am = SchemeModel::pInstance->m_analysModel;
	SchemeDesignModel& dm = SchemeModel::pInstance->m_designModel;
	//if (0 == dm.nFlagValid || 0 == am.nFlagValid)
	//	return Point3(0,0,0);

	Point3 center3D = Get3DCenterPointFromLicheng(pt2D.y);
	int secIndex = GetSectionIndex(pt2D.y); // ���ڵڼ���·��
	if (secIndex >= dm.vecSectionCenterPoints3D.size()-1)
		secIndex = dm.vecSectionCenterPoints3D.size() - 2;
	Point3 pt1 = dm.vecSectionCenterPoints3D[secIndex];
	Point3 pt2 = dm.vecSectionCenterPoints3D[secIndex+1];
	Point3 direction = (pt2 - pt1).normalize();
	Point3 norm = Point3(direction.y, -1*direction.x, direction.z);

	Point3 pt3D = center3D + norm * pt2D.x;
	pt3D.z = pt2D.z;
	return pt3D;
}

// ��AnalysModel�����֮������á�
void SchemeAnalyzer::Get3DPointsFrom2D( Point3Array& vec2D , Point3Array& vec3D)
{
	if (false == SchemeModel::pInstance->m_initModel.nFlagValid)
		return;

	for (int i = 0; i < vec2D.size(); i++)
	{
		Point3 pt3D = Get3DPointFrom2D(vec2D.at(i));
		vec3D.push_back(pt3D);
	}
}

//��δʵ�֡�
// ����Ƚ��ѣ����ҽ�����ܲ�Ψһ
Point3f SchemeAnalyzer::Get2DPointFrom3D( Point3f pt3D )
{
	if (false == SchemeModel::pInstance->m_initModel.nFlagValid)
		return Point3(0,0,0);
	return Point3(0,0,0);
}

//��DesignModel�����֮������á�
// ʹ��designģ�͵�DEM����
float SchemeAnalyzer::GetGroundPointAtXY( float x3D, float y3D )
{
	SchemeDesignModel& dm = SchemeModel::pInstance->m_designModel;
	if (0 == dm.nFlagValid)
		return 0;
	if (dm.vecDem.size()==0)
		return 0;
	CDem* dem = &dm.vecDem[0]; // ��ص�TIN
	return dem->GetHeightAtXY(x3D, y3D);
}


}}}}}
