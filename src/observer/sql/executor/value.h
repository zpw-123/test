/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2021/5/14.
//

#ifndef __OBSERVER_SQL_EXECUTOR_VALUE_H_
#define __OBSERVER_SQL_EXECUTOR_VALUE_H_

#include <string.h>
#include <iomanip>
#include <string>
#include <ostream>
#include "sql/parser/parse_defs.h"

class TupleValue {
public:
  TupleValue() = default;
  virtual ~TupleValue() = default;

  virtual void to_string(std::ostream &os) const = 0;
  virtual std::string to_string() const = 0;
  virtual int compare(const TupleValue &other) const = 0;
  virtual void add(const TupleValue &other) = 0;
  virtual void divide(const size_t x) = 0;
  virtual AttrType type() const = 0;
private:
};

class IntValue : public TupleValue {
public:
  explicit IntValue(int value) : value_(value) {
  }

  void to_string(std::ostream &os) const override {
    os << value_;
  }

  std::string to_string() const override {
    return std::to_string(value_);
  }

  int compare(const TupleValue &other) const override {
    const IntValue & int_other = (const IntValue &)other;
    return value_ - int_other.value_;
  }

  virtual void add(const TupleValue &other) override {
    value_ += std::stoi(other.to_string());
  }

  virtual void divide(const size_t x) override {
    if (x == 0) {
      return;
    }
    value_ /= x;
  }
  
  virtual AttrType type() const override {
    return INTS;
  }
 
private:
  int value_;
};

class FloatValue : public TupleValue {
public:
  explicit FloatValue(float value) : value_(value) {
  }

  void to_string(std::ostream &os) const override {
    std::string str = this->to_string();
    size_t dot_pos = str.find('.');
    if (dot_pos == std::string::npos) {
      os << str;
      return;
    }
    std::string zhengshu = str.substr(0, dot_pos);
    // std::string xiaoshu = str.substr(dot_pos);
    // if (xiaoshu.size() <= 2) {
    //   if (xiaoshu[0] == '0' && xiaoshu[1] == '0') {
    //     os << zhengshu;
    //     return;
    //   }
    //   os << zhengshu << '.' << xiaoshu;
    //   return;
    // }

    os << std::setprecision(zhengshu.size() + 2) << value_;
  }

  std::string to_string() const override {
    return std::to_string(value_);
  }

  int compare(const TupleValue &other) const override {
    const FloatValue & float_other = (const FloatValue &)other;
    float result = value_ - float_other.value_;
    if (result > 0) { // 浮点数没有考虑精度问题
      return 1;
    }
    if (result < 0) {
      return -1;
    }
    return 0;
  }

  virtual void add(const TupleValue &other) override {
    value_ += std::stoi(other.to_string());
  }

  virtual void divide(const size_t x) override {
    if (x == 0) {
      return;
    }
    value_ /= x;
  }

  virtual AttrType type() const override {
    return FLOATS;
  }
private:
  float value_;
};

class StringValue : public TupleValue {
public:
  StringValue(const char *value, int len) : value_(value, len){
  }
  explicit StringValue(const char *value) : value_(value) {
  }

  void to_string(std::ostream &os) const override {
    os << value_;
  }

  std::string to_string() const override {
    return value_;
  }

  int compare(const TupleValue &other) const override {
    const StringValue &string_other = (const StringValue &)other;
    return strcmp(value_.c_str(), string_other.value_.c_str());
  }

  virtual void add(const TupleValue &other) override {
    value_ += std::stoi(other.to_string());
  }

  virtual void divide(const size_t x) override {
    return;
  }

  virtual AttrType type() const override {
    return CHARS;
  }
private:
  std::string value_;
};

class NullValue : public TupleValue {
public:
  explicit NullValue() {
  }

  void to_string(std::ostream &os) const override {
    os << "NULL";
  }

  std::string to_string() const override {
    return std::string("NULL");
  }

  int compare(const TupleValue &other) const override {
    return 1;
  }

  virtual void add(const TupleValue &other) override {
    
  }

  virtual void divide(const size_t x) override {
    
  }
  
  virtual AttrType type() const override {
    return IS_NULL;
  }
 
private:
  // int value_;
};
#endif //__OBSERVER_SQL_EXECUTOR_VALUE_H_
