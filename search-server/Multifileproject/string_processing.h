#pragma once
#include <iostream>
#include "paginator.h"
#include "document.h"

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, IteratorRange<Iterator> it){
    for(auto iter = it.begin(); iter < it.end(); ++iter){
        out << *iter ;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, Document doc);