#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <cmath>
 
using namespace std;
 
 
const int MAX_RESULT_DOCUMENT_COUNT = 5;
 
string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}
 
int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}
 
vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            words.push_back(word);
            word = "";
        } else {
            word += c;
        }
    }
    words.push_back(word);
    
    return words;
}
    
struct Document {
    int id;
    double relevance;
};
 
class SearchServer {
    
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }
    
     void AddDocument(int document_id, const string& document) {
        document_count_++;
        vector<string> words = SplitIntoWordsNoStop(document);
         double tf = 1./words.size();
        for(const auto word : words){
            word_document_freqs[word][document_id] += tf;
        }
    }
 
    vector<Document> FindTopDocuments(const string& raw_query) const {        
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);
        
        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }
    
private:
    int document_count_ = 0;
    map<string, map<int,double>> word_document_freqs;
    set<string> stop_words_;
    
    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }
    
    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };
    
    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }
    
    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {
            text,
            is_minus,
            IsStopWord(text)
        };
    }
    
    struct Query {
        vector<string> plus_words;
        vector<string> minus_words;
    };
    
    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.push_back(query_word.data);
                } else {
                    query.plus_words.push_back(query_word.data);
                }
            }
        }
        return query;
    }
 
    double SetIDF (const string& text) const {
        string word = text;
        return log(static_cast <double> (document_count_)/static_cast <double> (word_document_freqs.at(word).size()));
    }
 
    vector<Document> FindAllDocuments(const Query& query) const {
        map<int, double> document_to_relevance;
        double IDF = 0;
        for (const string& word : query.plus_words) {
            if (word_document_freqs.count(word) == 0) {
                continue;
        }
            else{
                 IDF = SetIDF(word);
            }
            for ( auto [document_id, freq] : word_document_freqs.at(word)) {
                document_to_relevance[document_id] += IDF * freq;
            }
        }
        
        for (const string& word : query.minus_words) {
            if (word_document_freqs.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, freq] : word_document_freqs.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }
 
        vector<Document> matched_documents;
        for (auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({document_id, relevance});
        }
        return matched_documents;
    }
};
 
 
SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());
 
    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }
    
    return search_server;
}
 
 
int main() {
    const SearchServer search_server = CreateSearchServer();
 
    const string query = ReadLine();
    for (auto [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s
             << endl;
    }
 
    return 0;
}