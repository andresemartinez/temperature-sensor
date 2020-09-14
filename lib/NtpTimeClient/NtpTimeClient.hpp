#ifndef NTP_TIME_CLIENT_HPP
#define NTP_TIME_CLIENT_HPP

#include <Ticker.h>
#include <Udp.h>

#define SEVENZYYEARS 2208988800UL
#define NTP_PACKET_SIZE 48

namespace NTPTimeClientInternals {
  typedef const unsigned long Epoch_t;
}

class NtpTimeClient {
  private:

    UDP* _udp;
    bool _udpSetup;
    char _readUDPAttempts;
    const char _retryLimit;
    const char _retyInterval; // In ms
    Ticker _udpRetryTimer;

    const char* _poolServerName;
    const unsigned int _port;

    Ticker _updateTimer;
    const float _updateInterval; // In s
    unsigned long _lastEpochUpdate;


    unsigned long _currentEpoc;   // In s

    uint8_t _packetBuffer[NTP_PACKET_SIZE];

    void _update();
    void _blockingUpdate();
    void _flushUdp();
    void _checkUdpReceive();
    void _sendNtpPacket();
    void _readNtpPacket();
    void _wirteNumber(char* buffer, const int number) const;
    void _writeZeroPaddedNumber(char* buffer, const int number) const;

  public:
    NtpTimeClient(UDP& udp);

    void begin();
    void end();
    char* isoString() const;
    char* isoString(NTPTimeClientInternals::Epoch_t epoch) const;
    NTPTimeClientInternals::Epoch_t epochTime() const;

};

#endif
