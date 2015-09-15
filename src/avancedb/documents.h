#ifndef DOCUMENTS_H
#define	DOCUMENTS_H

#include <limits>
#include <vector>

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
#include "bulk_documents_result.h"

class Database;

class Documents final : public boost::enable_shared_from_this<Documents>, private boost::noncopyable {
public:
    
    using collection = rs::LazyFlatSet<document_ptr, Document::Less, Document::Equal, rs::LazyFlatSetQuickSort<document_ptr, Document::Less>, std::allocator<document_ptr>, true>;

    static documents_ptr Create(database_ptr db);
    
    document_ptr GetDocument(const char* id, bool throwOnFail = true);
    document_ptr DeleteDocument(const char* id, const char* rev);
    document_ptr SetDocument(const char* id, script_object_ptr obj);
    
    document_ptr GetLocalDocument(const char* id);
    document_ptr DeleteLocalDocument(const char* id, const char* rev);
    document_ptr SetLocalDocument(const char* id, script_object_ptr obj);
    
    document_array_ptr GetDocuments(const GetAllDocumentsOptions& options, collection::size_type& offset, collection::size_type& totalDocs, sequence_type& updateSequence);
    document_array_ptr PostDocuments(const PostAllDocumentsOptions& options, Documents::collection::size_type& totalDocs, sequence_type& updateSequence);
    
    BulkDocumentsResults PostBulkDocuments(script_array_ptr docs, bool newEdits);
    
    collection::size_type getCount();
    sequence_type getUpdateSequence();
    
private:
    
    const collection::size_type findMissedFlag = ~(std::numeric_limits<collection::size_type>::max() / 2);
    
    class DocumentsMutex {
    public:
        DocumentsMutex() {}
        DocumentsMutex(const DocumentsMutex& other) = delete;
        
        void lock() { mtx_.lock(); }
        bool try_lock() { return mtx_.try_lock(); }
        void unlock() { mtx_.unlock(); }
        
    private:
        
        boost::mutex mtx_;
        char padding_[64];
    };
    
    Documents(database_ptr db);
    
    document_array_ptr GetAllDocuments(sequence_type& sequenceNumber);    
    collection::size_type FindDocument(const document_array& docs, const std::string& id, bool descending);
    unsigned GetCollectionCount() const;
    unsigned GetDocumentCollectionIndex(const char* id) const;
    
    database_wptr db_;
    
    const unsigned collections_;
    std::vector<DocumentsMutex> docsMtx_;
    std::vector<collection> docs_;
    boost::atomic<sequence_type> updateSeq_;
    
    boost::mutex localDocsMtx_;
    collection localDocs_;
    boost::atomic<sequence_type> localUpdateSeq_;
    
    boost::mutex allDocsCacheMtx_;
    boost::atomic<sequence_type> allDocsCacheUpdateSequence_;
    document_array_ptr allDocsCacheDocs_;    

};

#endif	/* DOCUMENTS_H */
