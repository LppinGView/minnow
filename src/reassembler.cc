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
    if (nextIndex < first_index  || nextIndex > first_index+data.length()-1)
    {
      return;
    }
  }

  //insert data and build index
  dataInsert( data);

  //write into stream
  for (uint64_t next : index_)
  {
    if (nextIndex == next)
    {
      writeData(next);
    }else
    {
      break;
    }
  }
  
  if (is_last_substring)
  {
    lastOne=is_last_substring;
    lastOneIndex=first_index;
  }

  if (nextIndex >= lastOneIndex && lastOne)
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

void Reassembler::dataInsert(string data)
{
  auto subLen = min(data.length() - nextIndex, available_capacity());
  auto dataStr = data.substr(nextIndex, subLen);
  if (dataStr.length() >0)
  {
    index_.insert(nextIndex);
    dataMap_.insert({nextIndex, data});
    pushedSize+=data.length();
  }
}

void Reassembler::writeData(uint64_t next)
{
  std::string dataSeg = dataMap_[next];
  if ( dataSeg != "" && output_.writer().available_capacity() > 0 ) {
    nextIndex+=min(output_.writer().available_capacity(), dataSeg.length());
  }
  output_.writer().push(dataSeg);
  index_.erase(next);
  dataMap_.erase(next);
  pushedSize-=dataSeg.length();
}

void Reassembler::close()
{
  output_.writer().close();
  dataMap_.clear(); 
  index_.clear();
}