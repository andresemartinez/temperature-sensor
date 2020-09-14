
#include <NtpTimeClient.hpp>
#include <Arduino.h>
#include <time.h>

NtpTimeClient::NtpTimeClient(UDP& udp) :
  _udp(&udp),
  _udpSetup(false),
  _readUDPAttempts(1),
  _retryLimit(100),
  _retyInterval(10),
  _udpRetryTimer(),
  _poolServerName("pool.ntp.org"),
  _port(1337),
  _updateTimer(),
  _updateInterval(60),
  _lastEpochUpdate(0),
  _currentEpoc(0)
   {}

void NtpTimeClient::begin() {
  _udp->begin(_port);
  _udpSetup = true;
  _blockingUpdate();
  _updateTimer.attach_scheduled(_updateInterval,  std::bind(&NtpTimeClient::_update, this));
}

void NtpTimeClient::_update() {

  _flushUdp();
  _sendNtpPacket();
  _checkUdpReceive();

}

void NtpTimeClient::_blockingUpdate() {
  _flushUdp();
  _sendNtpPacket();


  while (_udp->parsePacket() == 0) {
    delay ( 10 );
    if (_readUDPAttempts++ > _retryLimit) return; // timeout after 1000 ms
  }

  _lastEpochUpdate = millis() - (10 * (_readUDPAttempts + 1));

  _readNtpPacket();
}

void NtpTimeClient::end() {
  _udp->stop();
  _udpSetup = false;
  _updateTimer.detach();
  _udpRetryTimer.detach();
}

void NtpTimeClient::_flushUdp() {
  while(_udp->parsePacket() != 0) {
    _udp->flush();
  }
}

void NtpTimeClient::_checkUdpReceive() {
  if(_udp->parsePacket() == 0
    && _readUDPAttempts++ < _retryLimit) {
    _udpRetryTimer.once_ms_scheduled(_retyInterval, std::bind(&NtpTimeClient::_checkUdpReceive, this));
  } else {
    _lastEpochUpdate = millis() - (10 * (_readUDPAttempts + 1));
    _readUDPAttempts = 1;
    _readNtpPacket();
  }
}

void NtpTimeClient::_sendNtpPacket() {
  // set all bytes in the buffer to 0
  memset(_packetBuffer, 0, NTP_PACKET_SIZE);

  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  _packetBuffer[0] = 0b11100011;  // LI, Version, Mode
  _packetBuffer[1] = 0;           // Stratum, or type of clock
  _packetBuffer[2] = 6;           // Polling Interval
  _packetBuffer[3] = 0xEC;        // Peer Clock Precision

  // 8 bytes of zero for Root Delay & Root Dispersion
  _packetBuffer[12]  = 49;
  _packetBuffer[13]  = 0x4E;
  _packetBuffer[14]  = 49;
  _packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  _udp->beginPacket(_poolServerName, 123);
  _udp->write(_packetBuffer, NTP_PACKET_SIZE);
  _udp->endPacket();

}

void NtpTimeClient::_readNtpPacket() {

  _udp->read(_packetBuffer, NTP_PACKET_SIZE);

  unsigned long highWord = word(_packetBuffer[40], _packetBuffer[41]);
  unsigned long lowWord = word(_packetBuffer[42], _packetBuffer[43]);

  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;

  _currentEpoc = secsSince1900 - SEVENZYYEARS;

}

NTPTimeClientInternals::Epoch_t NtpTimeClient::epochTime() const {
  const unsigned long longLimit = 4294967295;
  unsigned long m = millis();

  return m >= _lastEpochUpdate
    ? _currentEpoc + ((m - _lastEpochUpdate) / 1000)
    : _currentEpoc + ((longLimit - _lastEpochUpdate + m) / 1000);

}

char* NtpTimeClient::isoString() const {
  return this->isoString(this->epochTime());
}

char* NtpTimeClient::isoString(NTPTimeClientInternals::Epoch_t epoch) const {
  time_t rawtime = epoch;
  struct tm * ti;
  ti = localtime (&rawtime);

  char* isoStr = (char*) malloc(sizeof(char) * 21);

  sprintf(isoStr, "%d", ti->tm_year + 1900); // 0
  isoStr[4] = '-'; // 4
  _writeZeroPaddedNumber(isoStr + 5, ti->tm_mon); // 5
  isoStr[7] = '-'; // 7
  _writeZeroPaddedNumber(isoStr + 8, ti->tm_mday); // 8
  isoStr[10] = 'T'; // 10
  _writeZeroPaddedNumber(isoStr + 11, ti->tm_hour); // 11
  isoStr[13] = ':'; // 13
  _writeZeroPaddedNumber(isoStr + 14, ti->tm_min); // 14
  isoStr[16] = ':'; // 16
  _writeZeroPaddedNumber(isoStr + 17, ti->tm_sec); // 17
  isoStr[19] = 'Z'; // 19
  isoStr[20] = '\0'; // 20

  return isoStr;

}

void NtpTimeClient::_wirteNumber(char* buffer, const int number) const {
  sprintf(buffer, "%d", number);
}

void NtpTimeClient::_writeZeroPaddedNumber(char* buffer, const int number) const {
  sprintf(buffer, "%02d", number);
}
