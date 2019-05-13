#ifndef _CDR_DICTIONARY_H_
#define _CDR_DICTIONARY_H_

#include "cdr.h"

#include "avro"

#include <map>
#include <functional>
#include <string>

namespace AVRO
{
	using DataGetter = std::functional<GenericDatum(const Cdr&)>;
	using DataGetterMap = std::map<std::string, DataGetter>;

	//DataGetters
	auto diameter_app_id_getter = [](const Cdr& cdr) -> GenericDatum {
		return cdr.type == Diameter? 
			GenericDatum{cdr.detail.app_id}:
			GenericDatum{};
	};
	GenericDatum isup_caller_no_getter(const Cdr& cdr) {
		return cdr.type == Isup? 
			GenericDatum{cdr.detail.caller_no}: 
			GenericDatum{};
	}

	//The lookup of the DataGetter against the Avro field name
	DataGetterMap dataGetterMap = {
		{"timestamp"s, {[](const Cdr& cdr) { return GenericDatum{cdr.timestamp}; }}},
		{"diameter.application-id"s, {diameter_app_id_getter}},
		{"isup.caller-no"s, {isup_caller_no_getter}},
		{"isup.called-no"s, {[](const Cdr& cdr) { 
			return cdr.type == Isup? GenericDatum{cdr.detail.called_no}: GenericDatum{}; }}},
	};

	//DataGetter Dictionary Abstraction for Cdr
	class CdrDictionary
	{
		CdrDictionary(const DataGetterMap& m) : dataGetterMap(m) {}
		const DataGetter& find(const std::string& field_name) const {
			if (auto it = dataGetterMap.find(field_name); it != end(dataGetterMap)) {
				return it->second;
			}
			static DataGetter nullGetter{[](const Cdr&) { return GenericDatum{}; }};
			return nullGetter;
		}
		
		private:
		DataGetterMap dataGetterMap;
	};
}

#endif //_CDR_DICTIONARY_H_
