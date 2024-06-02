#pragma once

#include "byte_stream.hh"
#include <set>
#include <map>
#include <iostream>

class Reassembler
{
public:
  // Construct Reassembler to write into given ByteStream.
  explicit Reassembler( ByteStream&& output )
    : output_( std::move( output ) ), index_(), dataMap_()
  {}

  //当前需要有一个数据结构，用于存储到达的 字串，并排序。
  //当一个substring 之前没有其他字串了，就可以将 substring写入到 byteStream。但是这个怎么确定呢？
  //这里可以多线程处理，但是需要处理共享 字串列。可以复制一个子串列出来，不过内存是个问题；

  /*
   * Insert a new substring to be reassembled into a ByteStream.
   *   `first_index`: the index of the first byte of the substring
   *   `data`: the substring itself
   *   `is_last_substring`: this substring represents the end of the stream
   *   `output`: a mutable reference to the Writer
   *
   * The Reassembler's job is to reassemble the indexed substrings (possibly out-of-order
   * and possibly overlapping) back into the original ByteStream. As soon as the Reassembler
   * learns the next byte in the stream, it should write it to the output.
   *
   * If the Reassembler learns about bytes that fit within the stream's available capacity
   * but can't yet be written (because earlier bytes remain unknown), it should store them
   * internally until the gaps are filled in.
   *
   * The Reassembler should discard any bytes that lie beyond the stream's available capacity
   * (i.e., bytes that couldn't be written even if earlier gaps get filled in).
   *
   * The Reassembler should close the stream after writing the last byte.
   */
  // data 可能是重叠字串  字串格式【】，相同字串只存储一次
  void insert( uint64_t first_index, std::string data, bool is_last_substring );

  // How many bytes are stored in the Reassembler itself?
  uint64_t bytes_pending() const;

  // for the external customer
  // Access output stream reader
  Reader& reader() { return output_.reader(); }
  const Reader& reader() const { return output_.reader(); }

  // Access output stream writer, but const-only (can't write from outside)
  const Writer& writer() const { return output_.writer(); }

private:
  ByteStream output_; // the Reassembler writes to this ByteStream
  
  uint64_t capacity__= 1500;
  uint64_t pushedSize=0;
  uint64_t nextIndex=0;
  bool lastOne=false;
  uint64_t lastOneIndex=0;
  std::set<uint64_t> index_;
  std::map<uint64_t, std::string> dataMap_;

  uint64_t available_capacity() const; // How many bytes can be pushed to the reassembler right now?
  void close();
  
  //insert data, build index, those can abstruct to a class called Index
  void dataInsert( std::string data);
  void writeData(uint64_t index);
};
