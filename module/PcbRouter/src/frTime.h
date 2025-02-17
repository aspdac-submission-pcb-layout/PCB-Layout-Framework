/* Authors: Lutong Wang and Bangqi Xu */
/*
 * Copyright (c) 2019, blinded for review ICCAD 2020 submission 271
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FR_TIME_H_
#define _FR_TIME_H_

#include <iostream>
#include <chrono>
#include <ctime>
//#include <boost/io/ios_state.hpp>

extern size_t getPeakRSS();
extern size_t getCurrentRSS();

namespace fr {
  class frTime {
  public:
    frTime(): t0(std::chrono::high_resolution_clock::now()), t(clock()) {}
    //void begin() {
    //  t0 = std::chrono::high_resolution_clock::now();
    //}
    //void end() {
    //  t1 = std::chrono::high_resolution_clock::now();
    //}
    std::chrono::high_resolution_clock::time_point getT0() const {
      return t0;
    }
    void print();
    bool isExceed(double in) {
      auto t1        = std::chrono::high_resolution_clock::now();
      auto time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t1 - t0);
      return (time_span.count() > in);
    }
  protected:
    std::chrono::high_resolution_clock::time_point t0;
    clock_t t;
    //std::chrono::high_resolution_clock::time_point t1;
  };
}

extern std::ostream& operator<<(std::ostream& os, const fr::frTime &t);
#endif
