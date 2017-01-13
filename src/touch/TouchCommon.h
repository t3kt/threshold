//
//  TouchCommon.h
//

#pragma once

#include <vector>
#include <stdexcept>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include "Common.h"
#include "Point.h"
#include "PointSet.h"
#include "CPlusPlus_Common.h"

using TouchPoint = ThreshPoint<ThVec3f>;
using TouchPointSource = PointSource<TouchPoint>;

class OP_CHOPInput;

class CHOPInputPointSet : public TouchPointSource {
public:
  CHOPInputPointSet(const OP_CHOPInput* inputs,
                    int xI, int yI, int zI)
  : _inputs(inputs)
  , _xInputIndex(xI), _yInputIndex(yI) , _zInputIndex(zI) { }
  
  int size() const override;
  PointT getPoint(int i) const override;
  VecT getPosition(int i) const override;
private:
  int _xInputIndex;
  int _yInputIndex;
  int _zInputIndex;
  const OP_CHOPInput* _inputs;
};

class SimpleInfoTable {
public:
  void clear() {
    _column0.clear();
    _column1.clear();
  }

  void setRowCount(int rowCount) {
    _column0.resize(rowCount);
    _column1.resize(rowCount);
  }

  void setRow(int rowIndex, const char* key, const char* value) {
    _column0.at(rowIndex) = key;
    _column1.at(rowIndex) = value;
  }

  template<typename T>
  void setRow(int rowIndex, const char* key, const T& value) {
    std::ostringstream os;
    os << value;
    setRow(rowIndex, key, os.str().c_str());
  }

  void fillInfoSize(OP_InfoDATSize* size) {
    size->rows = static_cast<int>(_column0.size());
    size->cols = 2;
  }

  void fillInfoRowEntries(int rowIndex, int count, OP_InfoDATEntries* entries) {
    if (rowIndex < 0 || rowIndex > _column0.size()) {
      return;
    }
    static char buffer0[4096];
    static char buffer1[4096];

    if (count >= 1) {
      const auto& val = _column0[0];
      if (val.empty()) {
        strcpy_s(buffer0, "");
      } else {
        strcpy_s(buffer0, val.c_str());
      }
    }
    if (count >= 2) {
      const auto& val = _column1[0];
      if (val.empty()) {
        strcpy_s(buffer1, "");
      }
      else {
        strcpy_s(buffer1, val.c_str());
      }
    }
  }
private:
  std::vector<std::string> _column0;
  std::vector<std::string> _column1;
};

