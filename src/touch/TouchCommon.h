//
//  TouchCommon.h
//

#pragma once

#include <array>
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

template<std::size_t RowCount, std::size_t BufferSize = 4096>
class SimpleInfoTable {
public:
  void setLabel(int rowIndex, const char* key) {
    _labels[rowIndex] = key;
  }

  void clearValues() {
    for (int rowIndex = 0; rowIndex < RowCount; ++rowIndex) {
      clearValue(rowIndex);
    }
  }

  void clearValue(int rowIndex) {
    _labels[rowIndex].clear();
  }

  void setValue(int rowIndex, const char* value) {
    _values[rowIndex] = value;
  }

  std::string& operator[](int rowIndex) {
    return _values[rowIndex];
  }

  template<typename T>
  void setValue(int rowIndex, const T& value) {
    std::ostringstream os;
    os << value;
    setValue(rowIndex, value);
  }

  void fillInfoSize(OP_InfoDATSize* size) {
    size->rows = static_cast<int>(_labels.size());
    size->cols = 2;
  }

  void fillInfoRowEntries(int rowIndex, int count, OP_InfoDATEntries* entries) {
    if (rowIndex < 0 || rowIndex > _labels.size()) {
      return;
    }

    if (count >= 1) {
      const auto& val = _labels[0];
      if (val.empty()) {
        strcpy_s(_buffer0, "");
      } else {
        strcpy_s(_buffer0, val.c_str());
      }
      entries->values[0] = _buffer0;
    }
    if (count >= 2) {
      const auto& val = _values[0];
      if (val.empty()) {
        strcpy_s(_buffer1, "");
      }
      else {
        strcpy_s(_buffer1, val.c_str());
      }
      entries->values[1] = _buffer1;
    }
  }
private:
  std::array<std::string, RowCount> _labels;
  std::array<std::string, RowCount> _values;
  char _buffer0[BufferSize];
  char _buffer1[BufferSize];
};

