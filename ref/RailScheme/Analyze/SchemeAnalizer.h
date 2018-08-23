#ifndef _SCHEMEANALYZER_H
#define _SCHEMEANALYZER_H

#include "Util/BasicStructures.h"

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme{

namespace model { class SchemeModel; }

namespace analyze{

class PositionInfoInLicheng // ÿ����̵���Ϣ
{
	double licheng;
	jx::data_type::Point3 point3D; // ��ʵ������
	jx::data_type::Point3 direction; // ģ�ͷ���
};

// ���������Ĵ�����࣬���ܵ���ʵ����
class SchemeAnalyzer
{
public: // ����Ϊ��ӿڣ�������ʵ��
	SchemeAnalyzer(jx::kernel3d::example::railscheme::model::SchemeModel* m);
	~SchemeAnalyzer();
	void Analyze(); // ���Ը��¼���SchemeModel���������ö��̲߳��м���

	// ���º����ļ��㶼�ǻ���SchemeModel.InitModel�е�
	static int GetSectionIndex(float licheng);
	static jx::data_type::Point3 Get3DCenterPointFromLicheng(float licheng);
	// �������������ϵת��Ϊ���3D����ϵ
	static jx::data_type::Point3 Get3DPointFrom2D(jx::data_type::Point3 pt2D);
	static void Get3DPointsFrom2D( jx::data_type::Point3Array& vec2D , jx::data_type::Point3Array& vec3D);
	static PositionInfoInLicheng Get3DPositionInfo(double licheng);
	// �����3D����ϵת��Ϊ�����������ϵ������Ƿ���ֵPoint3��yֵ
	static jx::data_type::Point3f Get2DPointFrom3D(jx::data_type::Point3f pt3D);
	// ��ȡ����߳�
	// ����SchemeModel.AnalysisModel.Tin
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
