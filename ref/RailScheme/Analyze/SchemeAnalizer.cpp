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

	// 从初始参数计算出设计参数
	if ((!dm.nFlagValid) || dm.nFlagChanged)
	{
		// 在这里计算分析参数
		this->ComputeDesignModel();
		// 修改状态
		dm.nFlagChanged = 1;
		dm.nFlagValid = 0xFFFF;
		dm.nFlagChanged = 0;
	}
	// 从设计参数计算出分析参数
	if ((!am.nFlagValid) || am.nFlagChanged)
	{
		// 在这里计算分析参数
		this->ComputeAnalysisModel();
		// 修改状态
		am.nFlagChanged = 1;
		am.nFlagValid = 0xFFFF;
		am.nFlagChanged = 0;
	}
	// 从分析参数计算出渲染参数
	if ((!rm.nFlagValid) || rm.nFlagChanged)
	{
		// 在这里计算可视化参数
		this->ComputeRenderModel();
		// 修改状态
		rm.nFlagChanged = 1; // 在SchemeView渲染后，这里会被置0
		rm.nFlagValid = 0xFFFF;
		rm.nFlagChanged = 0;
	}
}

// 以下这函数在别的cpp文件中
// ComputeDesignModel()
// ComputeAnalysisModel()
// ComputeRenderModel()

//float fStartLicheng; // 起始里程
//Point3 pt3DCenter; // 大地坐标系原点
//Point3fArray *vecSelectLinePoints3D;

// 获取当前点在哪一个路段,【DesignModel计算好之后可以用】
int SchemeAnalyzer::GetSectionIndex(float licheng)
{
	SchemeDesignModel& dm = SchemeModel::pInstance->m_designModel;

	float currLicheng = licheng; //当前的里程
	int secIndex = dm.vecSectionLicheng.size(); 
	for (int i = 0; i < dm.vecSectionLicheng.size(); i++)
		if (dm.vecSectionLicheng[i].y > currLicheng)
			return i;
	return dm.vecSectionLicheng.size();
}
// 获取里程对应的轴线3D点 【AnalysModel计算好之后可以用】
jx::data_type::Point3 SchemeAnalyzer::Get3DCenterPointFromLicheng( float licheng )
{
	SchemeDesignModel& dm = SchemeModel::pInstance->m_designModel;
	//if (0 == dm.nFlagValid || 0 == am.nFlagValid)
	//	return Point3(0,0,0);

	int secIndex = GetSectionIndex(licheng); // 属于第几个路段
	if (secIndex >= dm.vecSectionCenterPoints3D.size()-1)
		secIndex = dm.vecSectionCenterPoints3D.size()-2;
	Point3 pt1 = dm.vecSectionCenterPoints3D[secIndex];
	Point3 pt2 = dm.vecSectionCenterPoints3D[secIndex+1];
	Point3 direction = (pt2 - pt1).normalize();
	
	Point2 sectionLicheng2D = dm.vecSectionLicheng[secIndex]; // 当前路段的起始里程
	Point3 ptSecStart3D = dm.vecSectionCenterPoints3D[secIndex];
	Point3 ptSecEnd3D = dm.vecSectionCenterPoints3D[secIndex+1];
	float currLicheng = licheng;
	float rate = (currLicheng - sectionLicheng2D.x) / (sectionLicheng2D.y - sectionLicheng2D.x);
	//线性插值，铁路设计线对应3D点，这个点实际上在2D空间中是(0, licheng, 0)
	Point3 center3D = ptSecStart3D + (ptSecEnd3D - ptSecStart3D) * rate; 
	return center3D;
}
// 输入pt2D是以轴线方向为y轴,y 值为里程【AnalysModel计算好之后可以用】
Point3 SchemeAnalyzer::Get3DPointFrom2D( Point3 pt2D )
{
	SchemeAnalysModel& am = SchemeModel::pInstance->m_analysModel;
	SchemeDesignModel& dm = SchemeModel::pInstance->m_designModel;
	//if (0 == dm.nFlagValid || 0 == am.nFlagValid)
	//	return Point3(0,0,0);

	Point3 center3D = Get3DCenterPointFromLicheng(pt2D.y);
	int secIndex = GetSectionIndex(pt2D.y); // 属于第几个路段
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

// 【AnalysModel计算好之后可以用】
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

//【未实现】
// 这个比较难，而且结果可能不唯一
Point3f SchemeAnalyzer::Get2DPointFrom3D( Point3f pt3D )
{
	if (false == SchemeModel::pInstance->m_initModel.nFlagValid)
		return Point3(0,0,0);
	return Point3(0,0,0);
}

//【DesignModel计算好之后可以用】
// 使用design模型的DEM即可
float SchemeAnalyzer::GetGroundPointAtXY( float x3D, float y3D )
{
	SchemeDesignModel& dm = SchemeModel::pInstance->m_designModel;
	if (0 == dm.nFlagValid)
		return 0;
	if (dm.vecDem.size()==0)
		return 0;
	CDem* dem = &dm.vecDem[0]; // 大地的TIN
	return dem->GetHeightAtXY(x3D, y3D);
}


}}}}}
