#include "reassembler.hh"

using namespace std;

uint64_t Reassembler::bytes_pending() const
{
  return pendingCount;
}

uint64_t Reassembler::available_capacity() const
{
  return output_.writer().available_capacity() - pendingCount;
}

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  if (available_capacity() <=0) {
    return;
  }
  if (available_capacity() == 1 && !contain_next(first_index, data, nextIndex)) {
    return;
  }
  push_rightnow_else_store(first_index, data);
  sustring_stored_merge();
  substring_stored__push_2_stream();

  if (is_last_substring) {
    lastOne=is_last_substring;
    lastOneIndex=data.length() >= 1? first_index+data.length()-1: 0;
  }
  if ((nextIndex > lastOneIndex || lastOneIndex == 0) && lastOne) {
    close();
  }
}

void Reassembler::push_rightnow_else_store(uint64_t first_index, string data)
{
  if (first_index <= nextIndex){
    push_2_stream(first_index, nextIndex, data);

  } else {
    auto avail_cap = available_capacity() == 1? available_capacity() : available_capacity() - 1;
    auto dataStr = data.substr(0, min(data.length(), avail_cap));
    if (dataStr.length() >0) {
      if (index_.contains(first_index)) {
        //replace
        if (dataMap_[first_index].length() < dataStr.length()) {
          pendingCount+=dataStr.length()-dataMap_[first_index].length();
          dataMap_[first_index]=dataStr;
        }
      } else {
        index_.insert(first_index);
        dataMap_.insert({first_index, dataStr});
        pendingCount+=dataStr.length();
      }

    }
  }
}

void Reassembler::sustring_stored_merge()
{
  vector<pair<uint64_t, string>> mergedPairs;
  vector<bool> merged(index_.size(), false);

  for ( auto it = index_.begin(); it != index_.end(); ++it) {
    if (merged[distance(index_.begin(), it)]) continue;
    uint64_t fIndex = *it;
    string fString = dataMap_[fIndex];
    for (auto it2 = next(it); it2 != index_.end(); ++it2) {
        if (merged[distance(index_.begin(), it2)]) continue;
        uint64_t sIndex = *it2;
        if (fIndex <= sIndex &&  sIndex <= (fIndex+fString.length()-1)) {
            
            string flsub = fString.substr(sIndex - fIndex);
            string lsub = flsub.length() > dataMap_[sIndex].length() ? flsub : dataMap_[sIndex];
            string fsub= fString.substr(0, sIndex - fIndex);
            
            fString = fsub + lsub;

            merged[distance(index_.begin(), it2)] = true;
        }
    }
    mergedPairs.emplace_back(fIndex, fString);
  }

  // Clear the original map and insert merged pairs
  dataMap_.clear();
  index_.clear();
  pendingCount=0;
  for (const auto &pair : mergedPairs) {      
      index_.insert(pair.first);
      dataMap_[pair.first] = pair.second;
      pendingCount+=pair.second.length();
  }
}

void Reassembler::substring_stored__push_2_stream()
{
  for ( auto it = index_.begin(); it != index_.end(); ) {
    uint64_t fIndex = *it;
    if ( fIndex <= nextIndex ) {
      if ( contain_next( fIndex, dataMap_[fIndex], nextIndex ) ) {
        auto pushLen = push_2_stream( fIndex, nextIndex, dataMap_[fIndex] );
        pendingCount -= ( ( pushLen > 0 ? dataMap_[fIndex].length() : 0 ) );
        it = index_.erase( it );
        dataMap_.erase( fIndex );
      } else {
        it = index_.erase( it );
        pendingCount -= ( pendingCount > 0 ? dataMap_[fIndex].length() : 0 );
        dataMap_.erase( fIndex );
      }
    } else {
      break;
    }
  }
}

uint64_t Reassembler::push_2_stream(uint64_t first_index, uint64_t next, string data)
{
  if (!contain_next(first_index, data, next)) {
    return 0;
  }
  auto subStr = data.substr(next - first_index, data.length());
  if (subStr != "") {
    auto sublen = min(subStr.length(), output_.writer().available_capacity());
    output_.writer().push(subStr.substr(0, sublen));
    nextIndex+=sublen;
    return sublen;
  }
  return 0;
}

bool Reassembler::contain_next(uint64_t first_index, string data, uint64_t next) const
{
  return  !data.empty() && first_index <= next && next <= first_index + data.length()-1;
}

void Reassembler::close()
{
  output_.writer().close();
  dataMap_.clear(); 
  index_.clear();
}