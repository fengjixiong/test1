#ifndef _SCHEMESERIALIZER_H
#define _SCHEMESERIALIZER_H

namespace jx{
namespace kernel3d { 
namespace example{
namespace railscheme {

namespace model { class SchemeModel; }

namespace serialize {

class SchemeSerializer
{
public:
	SchemeSerializer(jx::kernel3d::example::railscheme::model::SchemeModel* s);
	~SchemeSerializer();
	void Load(char* filepath);
	void Save(char* filepath);
	void LoadFromString(char* buffer);
	void SaveToString(char* buffer);
private:
	jx::kernel3d::example::railscheme::model::SchemeModel* m_pOutModel;
};

}}}}}

#endif

