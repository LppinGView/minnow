#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include <cstdint>

using namespace std;

void sustring_stored_merge(unordered_map<uint64_t, string>& dataMap_, set<uint64_t>& index_)
{
  vector<pair<uint64_t, string>> mergedPairs;
  vector<bool> merged(index_.size(), false);

  for ( auto it = index_.begin(); it != index_.end(); ++it) {
    if (merged[distance(index_.begin(), it)]) continue;
    uint64_t fIndex = *it;
    string fString = dataMap_[fIndex];
    cout << "fIndex: " << fIndex << endl;
    cout << "fString: " << fString << endl;
    for (auto it2 = next(it); it2 != index_.end(); ++it2) {
        if (merged[distance(index_.begin(), it2)]) continue;
        cout << "*it2: " << *it2 << endl;
        if (fIndex <= *it2 &&  *it2 <= fIndex + fString.length()) {
            cout << "fString.substr: " << fString.substr(0, *it2 - fIndex) << endl;
            string lsub = fString.substr(*it2 - fIndex, fString.length()-*it2 + 1).length() > dataMap_[*it2].length() ? fString.substr(*it2 - fIndex, fString.length()-*it2 + 1) : dataMap_[*it2];
            fString = fString.substr(0, *it2 - fIndex) + lsub;
            cout << "merged fString: " << fString << endl;
            merged[distance(index_.begin(), it2)] = true;
        }
    }
    mergedPairs.emplace_back(fIndex, fString);
  }

  // Clear the original map and insert merged pairs
  dataMap_.clear();
  index_.clear();
  for (const auto &pair : mergedPairs) {
      index_.insert(pair.first);
      dataMap_[pair.first] = pair.second;
  }
}

int main() {
    unordered_map<uint64_t, string> dataMap_ = {
        {2, "c"},
        {1, "bcd"},
        {5, "fgh"},
        {8, "ij"}
    };

    set<uint64_t> index_ = {8,5,2,1};

    sustring_stored_merge(dataMap_, index_);

    // Print the merged map
    for (const auto &pair : dataMap_) {
        cout << pair.first << ": " << pair.second << endl;
    }
    for (const auto &pair : index_) {
        cout << "index: " << pair << endl;
    }

    return 0;
}