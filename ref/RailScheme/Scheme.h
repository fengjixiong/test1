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
	// IScheme�ӿ�
	Scheme();
	~Scheme();
	virtual osg::Node* GetRenderNode(bool update);
	virtual void SetCADData(std::vector<jx::ExchangeData::ExchangeCadData*>* vecHdmData, int type);
	virtual void Load(char* filepath);
	virtual void Save(char* filepath);
public:
	model::SchemeModel*				m_pModel;		// ģ������
	view::SchemeView*				m_pView;		// ͼ����ʾ��
	analyze::SchemeAnalyzer*		m_pAnalyzer;	// ����������
	serialize::SchemeSerializer*	m_pSerialzer;	// ���л�������
};

}}}}

#endif