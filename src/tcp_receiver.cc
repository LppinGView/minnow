#include "tcp_receiver.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  if (message.RST){
    RST = message.RST;
    return;
  }
  if (message.SYN || next_no.operator==(message.seqno)){
    //Incoming message   
    next_no = message.seqno.operator+(message.sequence_length());
  }
  if (message.SYN){
      zero_point = message.seqno;
      checkpoint = message.seqno.unwrap(Wrap32(0), 0);
  }  
  reassembler_.insert(message.seqno.unwrap(zero_point, checkpoint)-1, message.payload, message.FIN);
}

TCPReceiverMessage TCPReceiver::send() const
{
  TCPReceiverMessage meg;
  meg.window_size=min(writer().available_capacity(), static_cast<uint64_t>(UINT16_MAX));
  if (RST){
    return meg;
  }
  if (next_no.operator==(Wrap32(0))){
    return meg;
  }
  meg.ackno=next_no;
  return meg;
}