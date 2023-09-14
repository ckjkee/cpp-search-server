#pragma once
#include <iostream>
#include <vector>

template <typename Iterator>
class IteratorRange{
public:

    IteratorRange() = default;
    
    IteratorRange(Iterator begin, Iterator end)
    :begin_(begin), end_(end)
    {
    
    }
    
    Iterator begin() const {
        return begin_;
    }
    
    Iterator end()const {
        return end_;
    }
    
    size_t size()const{
        return distance(begin_, end_);
    }


private:
 Iterator begin_, end_;

};

template <typename Iterator>
class Paginator{
public:
    Paginator() = default;
    
  Paginator(Iterator begin, Iterator end, size_t page_size) {
        for(Iterator it = begin; it != end;){
        if(distance(it,end) < page_size){
           page_size =  distance(it,end);
        }
            pages_.push_back(IteratorRange(it, next(it,page_size)));
            it = next(it,page_size);
           
        }
    }
    
    auto begin() const {
        return pages_.begin();
    }
    
    auto end() const {
        return pages_.end();
    }
    
    size_t size() const{
        return pages_.size();
    }
    
private:
std::vector<IteratorRange<Iterator>> pages_;

};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}
