#include "Jx3DEnginePch.h"
//#include "stdafx.h"
#include "Scheme.h"
//#include <sstream>

#include "Util/Utils.h"
#include "Node/jxNode.h"
#include "Example/RailScheme/Model/SchemeModel.h"
#include "Example/RailScheme/View/SchemeView.h"
#include "Example/RailScheme/Serialize/SchemeSerializer.h"
#include "Example/RailScheme/Analyze/SchemeAnalizer.h"

using namespace jx::kernel3d::engine::node;
using namespace jx::kernel3d::example::railscheme::model;
using namespace jx::kernel3d::example::railscheme::analyze;
using namespace jx::kernel3d::example::railscheme::view;
using namespace jx::kernel3d::example::railscheme::serialize;

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme {
	
Scheme::Scheme()
{
	m_pModel = new SchemeModel(); // ����������ģ��
	m_pAnalyzer = new SchemeAnalyzer(m_pModel); // ������������������ģ�ͽ��з���
	m_pSerialzer = new SchemeSerializer(m_pModel); // �������л�������������ģ��
	m_pView = new view::SchemeView(&(m_pModel->m_renderModel)); // �������ӻ������Կ��ӻ�����ģ�ͽ���3Dչ��
}

Scheme::~Scheme()
{
	if (m_pModel != NULL)
	{
		delete m_pModel;
		m_pModel = NULL;
	}
	if (m_pView != NULL)
	{
		delete m_pView;
		m_pView = NULL;
	}
	if (m_pSerialzer != NULL)
	{
		delete m_pSerialzer;
		m_pSerialzer = NULL;
	}
	if (m_pAnalyzer != NULL)
	{
		delete m_pAnalyzer;
		m_pAnalyzer = NULL;
	}
}

void Scheme::SetCADData(std::vector<jx::ExchangeData::ExchangeCadData*>* vecHdmData, int type)
{
	//m_option->SetOption(vecHdmData, type);
}

void Scheme::Load(char* filepath)
{
	m_pSerialzer->Load(filepath);
	m_pAnalyzer->Analyze();
}
void Scheme::Save(char* filepath)
{
	m_pSerialzer->Save(filepath);
}

osg::Node* Scheme::GetRenderNode(bool update)
{
	if (update)
		m_pView->Update();
	osg::Node* node = m_pView->GetNode();
	return node;
}

}}}}