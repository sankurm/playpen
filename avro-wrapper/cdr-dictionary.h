#ifndef _CDR_DICTIONARY_H_
#define _CDR_DICTIONARY_H_

#include <map>
#include <functional>
#include <string>

using DataGetter = std::functional<GenericDatum(const Cdr&)>;
using DataGetterMap = std::map<std::string, DataGetter>;

DataGetterMap dataGetterMap = {
	{"timestamp"s, {[](const Cdr& cdr) { return GenericDatum{cdr.timestamp}; }}},
	{"diameter.application-id"s, {[](const Cdr& cdr) { 
		return cdr.type == Diameter? GenericDatum{cdr.detail.app_id}: GenericDatum{}; }}},
	{"isup.called-no"s, {[](const Cdr& cdr) { 
		return cdr.type == Isup? GenericDatum{cdr.detail.called_no}: GenericDatum{}; }}},
};

//TODO: Is this really needed? 
class CdrDictionary
{
	CdrDictionary(DataGetterMap& m) : dataGetterMap(m) {}
};

#endif //_CDR_DICTIONARY_H_
