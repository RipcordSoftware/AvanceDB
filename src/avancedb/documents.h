#ifndef DOCUMENTS_H
#define	DOCUMENTS_H

#include <limits>

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>

#include "../../externals/lazyflatset/lazyflatset.hpp"

#include "types.h"
#include "document.h"
#include "get_all_documents_options.h"
#include "post_all_documents_options.h"

class Database;

class Documents final : public boost::enable_shared_from_this<Documents>, private boost::noncopyable {
public:
    
    using collection = rs::LazyFlatSet<document_ptr, Document::Less, Document::Equal, rs::LazyFlatSetQuickSort<document_ptr, Document::Less>, std::allocator<document_ptr>, true>;

    static documents_ptr Create(database_ptr db);
    
    document_ptr GetDocument(const char* id);
    document_ptr DeleteDocument(const char* id, const char* rev);
    document_ptr SetDocument(const char* id, script_object_ptr obj);
    
    document_array GetDocuments(const GetAllDocumentsOptions& options, collection::size_type& offset, collection::size_type& totalDocs, sequence_type& updateSequence);
    document_array PostDocuments(const PostAllDocumentsOptions& options, Documents::collection::size_type& totalDocs, sequence_type& updateSequence);
    
    collection::size_type getCount();
    sequence_type getUpdateSequence();
    
private:
    
    const collection::size_type findMissedFlag = ~(std::numeric_limits<collection::size_type>::max() / 2);
    
    Documents(database_ptr db);
    
    document_array GetAllDocuments(sequence_type& sequenceNumber);
    
    collection::size_type FindDocument(const document_array& docs, const std::string& id, bool descending);
    
    database_wptr db_;
    
    boost::mutex docsMtx_;
    collection docs_;

    boost::atomic<sequence_type long> updateSeq_;        
};

#endif	/* DOCUMENTS_H */
