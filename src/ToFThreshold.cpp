#include "ToFThreshold.h"

const char* ToFThreshold::printCurrentState() {
  switch (this->currentState) {
    case ToFThreshold::State::Unknown:
      return "Unknown";
    case ToFThreshold::State::Near:
      return "Near";
    case ToFThreshold::State::Far:
      return "Far";
  }

  return ""; // To keep the compiler happy!!
}

void ToFThreshold::setInitialState(int range) {
  // Has the range passed a threshold for this sensor?
  // Comapre to the near value only so there is no 'grey' area between near and far.
  Serial.printf("\n%6ld In setInitialState(). range=%d", millis(), range);
  if (range < this->valueNear) {
    this->currentState = ToFThreshold::State::Near;
  } else {
    this->currentState = ToFThreshold::State::Far;
  }
}

bool ToFThreshold::isActive() {
  if (this->currentState == State::Near) {
    return true;
  } else {
    return false;
  }
}

int ToFThreshold::check(int range) {
  int eventIndexToSend = -1;

  switch (this->currentState) {
    case State::Far:
      // Has the range moved to less than the near value?
      if (range < this->valueNear) {
        this->currentState = State::Near;
        eventIndexToSend = this->eventIndexNear;
      }
      break;
    
    case State::Near:
      // Has the range moved to more than the far value?
      if (range > this->valueFar) {
        this->currentState = State::Far;
        eventIndexToSend = this->eventIndexFar;
      }
      break;
    
    case State::Unknown:
      break;
  }

  return eventIndexToSend;
}
