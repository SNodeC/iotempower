// toolbox.cpp

#include <Arduino.h>
#include "toolbox.h"

bool Ustring::add(const Ustring& other) {
    int ownlen = length();
    int otherlen = other.length();
    bool untruncated = true;

    if( ownlen+otherlen > ULNOIOT_MAX_STRLEN ) {
        otherlen = ULNOIOT_MAX_STRLEN - ownlen;
        untruncated = false;
    }
    strncpy(cstr+ownlen, other.cstr, otherlen);
    return untruncated;
}

bool Ustring::from(const char* _cstr) {
    int otherlen = strnlen(_cstr,ULNOIOT_MAX_STRLEN+1);
    bool untruncated = true;
    if(otherlen > ULNOIOT_MAX_STRLEN) {
        otherlen = ULNOIOT_MAX_STRLEN;
        untruncated = false;
    }
    strncpy(cstr,_cstr,ULNOIOT_MAX_STRLEN);
    return untruncated;
}

bool Ustring::from(const byte* payload, unsigned int length) {
    bool untruncated = true;
    if(length > ULNOIOT_MAX_STRLEN) {
        length = ULNOIOT_MAX_STRLEN;
        untruncated = false;
    }
    strncpy(cstr, (const char *)payload, length); // TODO: think about proper decoding
    cstr[length] = 0; // terminate properly
    return untruncated;
}

bool Ustring::remove(unsigned int from, unsigned int interval) {
    unsigned int mylen = length();
    if(from > mylen) return false;
    bool untruncated = true;
    if( from + interval > mylen ) {
        untruncated = false;
        interval = mylen - from;
    }
    memcpy(cstr + from, cstr + from + interval, mylen - from - interval);
    cstr[mylen - interval]=0;
    return untruncated;
}



void reboot() {
  // TODO: add network debugging
  Serial.println("Rebooting in 5 seconds.");
  delay(5000);
  // TODO: Consider counting in rtc and going into reconfigure after a while

  // make sure, reset is clean, gpio0 has to be high
  pinMode(0,OUTPUT);
  digitalWrite(0,1);
  yield();
  delay(10);
  yield();
  delay(500); // let things settle a bit
  ESP.reset(); // fails when serial activity TODO: solve or warn?
  //ESP.restart(); // fails when serial activity TODO: solve or warn?
}

void controlled_crash(const char * error_message) {
    log("\nCrash, cause (reboot in 5s): %s", error_message);
    reboot();
}

void log(const char *fmt, ...) {
	char buf[LOG_LINE_MAX_LEN];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, LOG_LINE_MAX_LEN, fmt, ap);
	va_end(ap);
	Serial.println(buf);
}
