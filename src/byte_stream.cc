#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity )
  : capacity_( capacity )
{}

bool Writer::is_closed() const
{
  return streamClose;
}

void Writer::push( string data )
{
  if ( data == "" || available_capacity() <= 0 ) {
    return;
  }
  auto dataLen = min( data.length(), available_capacity() );
  buffer += data.substr( 0, dataLen );
  wIndex += dataLen;
  pushedNum += dataLen;
}

void Writer::close()
{
  streamClose = true;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - wIndex;
}

uint64_t Writer::bytes_pushed() const
{
  return pushedNum;
}

bool Reader::is_finished() const
{
  return writer().is_closed() && wIndex == 0;
}

uint64_t Reader::bytes_popped() const
{
  return poppedNum;
}

string_view Reader::peek() const
{
  if ( wIndex > 0 && !buffer.empty() ) {
    return std::string_view( buffer.data(), buffer.size() );
  }
  return "";
}

void Reader::pop( uint64_t len )
{
  if ( len > 0 && wIndex > 0 ) {
    auto eraseLen = min( wIndex, len );
    buffer = buffer.substr( eraseLen, wIndex - eraseLen );
    wIndex -= eraseLen;
    poppedNum += eraseLen;
  }
}

uint64_t Reader::bytes_buffered() const
{
  return wIndex;
}
