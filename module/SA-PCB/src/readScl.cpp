///////////////////////////////////////////////////////////////////////////////
// Authors: blinded for review ICCAD 2020 submission 271
//          (respective ands: blinded for review ICCAD 2020 submission 271, blinded for review ICCAD 2020 submission 271, blinded for review ICCAD 2020 submission 271).
//
// BSD 3-Clause License
//
// Copyright (c) 2018, blinded for review ICCAD 2020 submission 271
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <fstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <map>
#include "readFiles.hpp"
//#include "readScl.hpp"

using namespace std;

void readSclFile() {
  fstream file;
  string buf;
  int i = 0, j = 0, Id = 1;
  vector < string > strVec;
  using boost::is_any_of;

  int coOrdinate;
  int height;
  int siteWidth;
  int siteSpacing;
  string siteOrient;
  string siteSymmetry;
  int siteRowOrigin;
  int numSites;

  file.open("ibm01.scl", ios:: in );

  while (getline(file, buf)) {
    i++;
    if (i > 8) {
      boost::algorithm::split(strVec, buf, is_any_of("\t,  "), boost::token_compress_on);
      j = i % 9;
      if (j == 1) {
        coOrdinate = atoi(strVec[3].c_str());
      } else if (j == 2) {
        height = atoi(strVec[3].c_str());
      } else if (j == 3) {
        siteWidth = atoi(strVec[3].c_str());
      } else if (j == 4) {
        siteSpacing = atoi(strVec[3].c_str());
      } else if (j == 5) {
        siteOrient = strVec[3];
      } else if (j == 6) {
        siteSymmetry = strVec[3];
      } else if (j == 7) {
        siteRowOrigin = atoi(strVec[3].c_str());
        numSites = atoi(strVec[6].c_str());
      } else if (j == 8) {
        row r;
        r.setId(Id);
        rowId.insert(pair < int, row > (Id, r));
        rowId[Id].setParameterRows(coOrdinate, height, siteWidth, siteSpacing, siteOrient, siteSymmetry, siteRowOrigin, numSites);
        Id++;
      }

    }
  }
  file.close();
}
