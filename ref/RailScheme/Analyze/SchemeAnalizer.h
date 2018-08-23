#ifndef _SCHEMEANALYZER_H
#define _SCHEMEANALYZER_H

#include "Util/BasicStructures.h"

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme{

namespace model { class SchemeModel; }

namespace analyze{

class PositionInfoInLicheng // 每个里程的信息
{
	double licheng;
	jx::data_type::Point3 point3D; // 真实的坐标
	jx::data_type::Point3 direction; // 模型方向
};

// 方案参数的纯虚基类，不能单独实例化
class SchemeAnalyzer
{
public: // 以下为虚接口，供子类实现
	SchemeAnalyzer(jx::kernel3d::example::railscheme::model::SchemeModel* m);
	~SchemeAnalyzer();
	void Analyze(); // 可以更新计算SchemeModel，还可以用多线程并行计算

	// 以下函数的计算都是基于SchemeModel.InitModel中的
	static int GetSectionIndex(float licheng);
	static jx::data_type::Point3 Get3DCenterPointFromLicheng(float licheng);
	// 将轴线里程坐标系转换为大地3D坐标系
	static jx::data_type::Point3 Get3DPointFrom2D(jx::data_type::Point3 pt2D);
	static void Get3DPointsFrom2D( jx::data_type::Point3Array& vec2D , jx::data_type::Point3Array& vec3D);
	static PositionInfoInLicheng Get3DPositionInfo(double licheng);
	// 将大地3D坐标系转换为轴线里程坐标系，里程是返回值Point3的y值
	static jx::data_type::Point3f Get2DPointFrom3D(jx::data_type::Point3f pt3D);
	// 获取地面高程
	// 基于SchemeModel.AnalysisModel.Tin
	static float GetGroundPointAtXY(float x3D, float y3D);
	static float GetGroundPointAtLicheng(float licheng);
private:
	void ComputeDesignModel();
	void ComputeAnalysisModel();
	void ComputeRenderModel();
private:
	jx::kernel3d::example::railscheme::model::SchemeModel* m_pOutModel;
};


}}}}}

#endif
