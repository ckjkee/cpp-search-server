#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server)
    : serv_(search_server), emptys_(0)
    {
        
    }

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
        
        std::vector<Document> result_ = serv_.FindTopDocuments(raw_query, status);
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

 std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
        // напишите реализацию
        std::vector<Document> result_ = serv_.FindTopDocuments(raw_query);
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

int RequestQueue::GetNoResultRequests() const {
        return emptys_;
    }