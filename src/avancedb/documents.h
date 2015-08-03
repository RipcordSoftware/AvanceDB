#ifndef DOCUMENTS_H
#define	DOCUMENTS_H

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>

#include "../../externals/lazyflatset/lazyflatset.hpp"

#include "types.h"
#include "document.h"
#include "get_all_documents_options.h"

class Database;

class Documents final : public boost::enable_shared_from_this<Documents>, private boost::noncopyable {
public:
    
    using collection = rs::LazyFlatSet<document_ptr, Document::Less, Document::Equal, rs::LazyFlatSetQuickSort<document_ptr, Document::Less>, std::allocator<document_ptr>, true>;

    static documents_ptr Create(database_ptr db);
    
    document_ptr GetDocument(const char* id);
    document_ptr DeleteDocument(const char* id, const char* rev);
    document_ptr SetDocument(const char* id, script_object_ptr obj, sequence_type seqNum);
    
    document_array GetDocuments(const GetAllDocumentsOptions& options, collection::size_type& totalDocs);
    
    collection::size_type getCount();
    
private:
    
    Documents(database_ptr db);
    
    document_array GetAllDocuments();
    
    database_wptr db_;
    
    boost::mutex docsMtx_;
    collection docs_;

};

#endif	/* DOCUMENTS_H */
