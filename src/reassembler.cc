#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  if (available_capacity() <=0)
  {
    return;
  }
  if (available_capacity() == 1 )
  {
    //current data not exist this nextIndex's char
    if (!containsNext(first_index, data, nextIndex))
    {
      return;
    }
  }
  
  push_or_store(first_index, data);

  //push store substring into stream
  for (uint64_t fIndex : index_)
	{
    if (containsNext(fIndex, dataMap_[fIndex], nextIndex)){
      auto pushLen = directPushStream(fIndex, nextIndex, dataMap_[fIndex]);
      pushedSize-=(pushLen > 0? dataMap_[fIndex].length() : 0);
    }
	}
  
  if (is_last_substring)
  {
    lastOne=is_last_substring;
    lastOneIndex=data.length() > 1? first_index+data.length()-1: 0;
  }

  if ((nextIndex > lastOneIndex || lastOneIndex == 0) && lastOne)
  {
    close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  return pushedSize;
}

uint64_t Reassembler::available_capacity() const
{
  return output_.writer().available_capacity() - pushedSize;
}

void Reassembler::push_or_store(uint64_t first_index, string data)
{
  if (first_index <= nextIndex)
  {
    directPushStream(first_index, nextIndex, data);
  }else
  {
    auto avail_cap = available_capacity() == 1? available_capacity() : available_capacity() - 1;
    auto dataStr = data.substr(0, min(data.length(), avail_cap));
    if (dataStr.length() >0)
    {
      index_.insert(first_index);
      dataMap_.insert({first_index, dataStr});
      pushedSize+=dataStr.length();
    }
  }
}

void Reassembler::close()
{
  output_.writer().close();
  dataMap_.clear(); 
  index_.clear();
}

uint64_t Reassembler::directPushStream(uint64_t first_index, uint64_t next, string data)
{
  if (!containsNext(first_index, data, next))
  {
    return 0;
  }

  auto subStr = data.substr(next - first_index, data.length());
  if (subStr != "")
  {
    //current substring direct write into stream
    auto sublen = min(subStr.length(), output_.writer().available_capacity());
    output_.writer().push(subStr.substr(0, sublen));
    nextIndex+=sublen;
    return sublen;
  }
  return 0;
}

bool Reassembler::containsNext(uint64_t first_index, string data, uint64_t next) const
{
  return  !data.empty() && first_index <= next && next <= first_index+data.length()-1;
}