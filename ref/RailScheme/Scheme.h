#ifndef _SCHEME_H
#define _SCHEME_H

#include "Util/OSG_Header.h"
#include "Extern/CAD_Hdm_Data.h"
#include "Scheme/IScheme.h"

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme {

namespace analyze { class SchemeAnalyzer; }
namespace view { class SchemeView; }
namespace model { class SchemeModel;}
namespace serialize { class SchemeSerializer; }

class Scheme : public jx::kernel3d::engine::scheme::IScheme
{
public:
	// IScheme接口
	Scheme();
	~Scheme();
	virtual osg::Node* GetRenderNode(bool update);
	virtual void SetCADData(std::vector<jx::ExchangeData::ExchangeCadData*>* vecHdmData, int type);
	virtual void Load(char* filepath);
	virtual void Save(char* filepath);
public:
	model::SchemeModel*				m_pModel;		// 模型数据
	view::SchemeView*				m_pView;		// 图形显示器
	analyze::SchemeAnalyzer*		m_pAnalyzer;	// 分析计算器
	serialize::SchemeSerializer*	m_pSerialzer;	// 序列化保存器
};

}}}}

#endif