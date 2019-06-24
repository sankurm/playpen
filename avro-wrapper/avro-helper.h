#ifndef _AVRO_WRAPPER_H_
#define _AVRO_WRAPPER_H_

#include "GenericWriter.hh"
#include <utility>

class GenericWriter : avro::GenericWriter
{
	using GenericWriter(const ValidSchema& s, const EncoderPtr& encoder);
	using write(const GenericDatum& datum) const;
	//Not including static functions here - they can come later, if needed
	
	using GenericUnionDatum = std::pair<const avro::GenericUnion&, const avro::GenericDatum&>;
	void write(const GenericUnionDatum datum) const;
};

void GenericWriter::write(const GenericUnionDatum& union_datum) const {
	if (union_datum.first.isUnion()) {
		datum.selectBranch(union_datum.first.encodeUnionIndex());
	}
	return write(union_datum.second);
}

#endif  //_AVRO_WRAPPER_H_
