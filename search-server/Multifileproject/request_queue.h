#pragma once
#include <deque>
#include "search_server.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);
    
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
       
       std::vector<Document> result_ = serv_.FindTopDocuments(raw_query, document_predicate);
       
       requests_.push_back({raw_query, result_});
       
        if(result_.empty()){
            emptys_++;
        }
        if(requests_.size() == min_in_day_ + 1){
            emptys_--;
            requests_.pop_front();
        }
        
        return result_;
        
    }
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;
private:
    struct QueryResult {
       std::string query_;
       std::vector<Document> result_;
       
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& serv_;
    size_t emptys_;
    
    
};