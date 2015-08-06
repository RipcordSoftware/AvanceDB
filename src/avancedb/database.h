#ifndef DATABASE_H
#define	DATABASE_H

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/atomic.hpp>

#include "documents.h"
#include "get_all_documents_options.h"
#include "post_all_documents_options.h"

class Database final : public boost::enable_shared_from_this<Database>, private boost::noncopyable {
public:
        
    static database_ptr Create(const char* name);
    
    unsigned long CommitedUpdateSequence() { return docs_->getUpdateSequence(); }
    unsigned long UpdateSequence() { return docs_->getUpdateSequence(); }
    unsigned long PurgeSequence() { return 0; }
    unsigned long DataSize() { return dataSize_; }
    unsigned long DiskSize() { return diskSize_; }
    unsigned long DocCount();
    unsigned long DocDelCount() { return docDelCount_; }
    unsigned long InstanceStartTime() { return instanceStartTime_; }
    
    document_ptr GetDocument(const char* id);
    document_ptr DeleteDocument(const char* id, const char* rev);
    document_ptr SetDocument(const char* id, script_object_ptr);
    
    document_array GetDocuments(const GetAllDocumentsOptions& options, Documents::collection::size_type& offset, Documents::collection::size_type& totalDocs, sequence_type& updateSequence);
    document_array PostDocuments(const PostAllDocumentsOptions& options, Documents::collection::size_type& totalDocs, sequence_type& updateSequence);
    
private:
    Database(const char*);
    
    static unsigned long Now();
    
    const std::string name_;
    
    const unsigned long instanceStartTime_;
    boost::atomic<unsigned long> docDelCount_;
    boost::atomic<unsigned long> dataSize_;
    boost::atomic<unsigned long> diskSize_;
    
    documents_ptr docs_;
};

#endif	/* DATABASE_H */

