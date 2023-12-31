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

#include "sql/executor/tuple.h"
#include "storage/common/table.h"
#include "common/log/log.h"
#include "storage/common/mydate.h"

Tuple::Tuple(const Tuple &other) {
  LOG_PANIC("Copy constructor of tuple is not supported");
  exit(1);
}

Tuple::Tuple(Tuple &&other) noexcept : values_(std::move(other.values_)) {
}

Tuple & Tuple::operator=(Tuple &&other) noexcept {
  if (&other == this) {
    return *this;
  }

  values_.clear();
  values_.swap(other.values_);
  return *this;
}

Tuple::~Tuple() {
}

// add (Value && value)
void Tuple::add(TupleValue *value) {
  values_.emplace_back(value);
}
void Tuple::add(const std::shared_ptr<TupleValue> &other) {
  values_.emplace_back(other);
}
void Tuple::add(int value) {
  add(new IntValue(value));
}

void Tuple::add(float value) {
  add(new FloatValue(value));
}

void Tuple::add(const char *s, int len) {
  add(new StringValue(s, len));
}

void Tuple::add() {
  add(new NullValue());
}

////////////////////////////////////////////////////////////////////////////////

std::string TupleField::to_string() const {
  return std::string(table_name_) + "." + field_name_ + std::to_string(type_);
}

////////////////////////////////////////////////////////////////////////////////
void TupleSchema::from_table(const Table *table, TupleSchema &schema) {
  const char *table_name = table->name();
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = 0; i < field_num; i++) {
    const FieldMeta *field_meta = table_meta.field(i);
    if (field_meta->visible()) {
      schema.add(field_meta->type(), table_name, field_meta->name());
    }
  }
}

void TupleSchema::add(AttrType type, const char *table_name, const char *field_name) {
  fields_.emplace_back(type, table_name, field_name);
}

void TupleSchema::add_if_not_exists(AttrType type, const char *table_name, const char *field_name) {
  for (const auto &field: fields_) {
    if (0 == strcmp(field.table_name(), table_name) &&
        0 == strcmp(field.field_name(), field_name)) {
      return;
    }
  }

  add(type, table_name, field_name);
}

void TupleSchema::append(const TupleSchema &other) {
  fields_.reserve(fields_.size() + other.fields_.size());
  for (const auto &field: other.fields_) {
    fields_.emplace_back(field);
  }
}

int TupleSchema::index_of_field(const char *table_name, const char *field_name) const {
  const int size = fields_.size();
  for (int i = 0; i < size; i++) {
    const TupleField &field = fields_[i];
    if (0 == strcmp(field.table_name(), table_name) && 0 == strcmp(field.field_name(), field_name)) {
      return i;
    }
  }
  return -1;
}

std::string TupleSchema::to_string(bool printTableName) const {
  std::string re;
  // std::string tableName = printTableName? 
  for (int i=0; i<fields_.size(); i++) {
    const TupleField &tupleField = fields_[i];
    std::string item = printTableName ? tupleField.table_name_str() + "." : "";
    item += tupleField.field_name_str();
    if (i != fields_.size() - 1) {
      item += " | ";
    }
    re += item;
  }
  return re;
}

void TupleSchema::print(std::ostream &os) const {
  if (fields_.empty()) {
    os << "No schema";
    return;
  }

  // 判断有多张表还是只有一张表
  std::set<std::string> table_names;
  for (const auto &field: fields_) {
    table_names.insert(field.table_name());
  }

  for (std::vector<TupleField>::const_iterator iter = fields_.begin(), end = --fields_.end();
       iter != end; ++iter) {
    if (table_names.size() > 1  && (strcmp("*", iter->table_name()) != 0)) {
      os << iter->table_name() << ".";
    }
    os << iter->field_name() << " | ";
  }

  if ((table_names.size() > 1) && (strcmp("*", fields_.back().table_name()) != 0)) {
    os << fields_.back().table_name() << ".";
  }
  os << fields_.back().field_name() << std::endl;
}

bool is_aggregation_schema(const char *attribute_name, char *&aggregation_filed, char *&field_name) {
  
  if (attribute_name == nullptr) {
    return false;
  }
  if (attribute_name[0] == 'M' || attribute_name[0] == 'm') {
    if ((attribute_name[1] == 'A' || attribute_name[1] == 'a') 
      && (attribute_name[2] == 'X' || attribute_name[2] == 'x') 
      && attribute_name[3] == '(') {
      size_t len = strlen(attribute_name) + 1 - 4;
      field_name = new char[len];
      aggregation_filed = new char[4] {'M', 'A', 'X', '\0'};
      strcpy(field_name, attribute_name+4);
      field_name[len-2] = '\0';
      return true;
    }
    if ((attribute_name[1] == 'I' || attribute_name[1] == 'i') 
      && (attribute_name[2] == 'N' || attribute_name[2] == 'n') 
      && attribute_name[3] == '(') {
      size_t len = strlen(attribute_name) + 1 - 4;
      field_name = new char[len];
      aggregation_filed = new char[4] {'M', 'I', 'N', '\0'};
      strcpy(field_name, attribute_name+4);
      field_name[len-2] = '\0';
      return true;;
    }
    return false;
  } else if ((attribute_name[0] == 'C' || attribute_name[0] == 'c') 
      && (attribute_name[1] == 'O' || attribute_name[1] == 'o') 
      && (attribute_name[2] == 'U' || attribute_name[2] == 'u')
      && (attribute_name[3] == 'N' || attribute_name[3] == 'n') 
      && (attribute_name[4] == 'T' || attribute_name[4] == 't') 
      && attribute_name[5] == '(') {
    size_t len = strlen(attribute_name) + 1 - 6;
    field_name = new char[len];
    aggregation_filed = new char[6] {'C', 'O', 'U', 'N', 'T', '\0'};
    strcpy(field_name, attribute_name+6);
    field_name[len-2] = '\0';
    return true;
  } else if ((attribute_name[0] == 'A' || attribute_name[0] == 'a') 
      && (attribute_name[1] == 'V' || attribute_name[1] == 'v')
      && (attribute_name[2] == 'G' || attribute_name[2] == 'g') 
      && attribute_name[3] == '(') {
    size_t len = strlen(attribute_name) + 1 - 4;
    field_name = new char[len];
    aggregation_filed = new char[4] {'A', 'V', 'G', '\0'};
    strcpy(field_name, attribute_name+4);
    field_name[len-2] = '\0';
    return true;
  }
  aggregation_filed = nullptr;
  field_name = nullptr;
  return false;
}

void TupleSchema::print(std::ostream &os, bool print_table_name) const {
  if (fields_.empty()) {
    os << "No schema";
    return;
  }
  char *field_name = nullptr, *aggregation_name = nullptr;

  for (std::vector<TupleField>::const_iterator iter = fields_.begin(), end = --fields_.end();
       iter != end; ++iter) {
    
    if (is_aggregation_schema(iter->field_name(), aggregation_name, field_name)) {
      os << aggregation_name << "(";
      if (print_table_name) {
        os << iter->table_name() << ".";
      } 
      os << field_name << ")" << " | ";
    } else {
      if (print_table_name && (strcmp("*", iter->table_name()) != 0)) {
        os << iter->table_name() << ".";
      }
      os << iter->field_name() << " | ";
    }
  }
  if (is_aggregation_schema(fields_.back().field_name(), aggregation_name, field_name)) {
    os << aggregation_name << "(";
    if (print_table_name) {
      os << fields_.back().table_name() << ".";
    } 
    os << field_name << ")" << std::endl;
  }
  else
  {
    if (print_table_name && (strcmp("*", fields_.back().table_name()) != 0)) {
      os << fields_.back().table_name() << ".";
    }
    os << fields_.back().field_name() << std::endl;
  }
}

/////////////////////////////////////////////////////////////////////////////
TupleSet::TupleSet(TupleSet &&other) : tuples_(std::move(other.tuples_)), schema_(other.schema_){
  other.schema_.clear();
}

TupleSet &TupleSet::operator=(TupleSet &&other) {
  if (this == &other) {
    return *this;
  }

  schema_.clear();
  schema_.append(other.schema_);
  other.schema_.clear();

  tuples_.clear();
  tuples_.swap(other.tuples_);

  // aggregation_flag = other.aggregation_flag;
  return *this;
}

void TupleSet::add(Tuple &&tuple) {
  tuples_.emplace_back(std::move(tuple));
}

void TupleSet::add_(const Tuple &tuple) {
  Tuple tuple_;
  for (int i=0; i<tuple.size(); i++) {
    tuple_.add(tuple.get_pointer(i));
  }
  tuples_.emplace_back(std::move(tuple_));
}

void TupleSet::clear() {
  tuples_.clear();
  schema_.clear();
}

// void TupleSet::set_aggregation_flag() {
//   aggregation_flag = true;
// }

void TupleSet::print(std::ostream &os) const {
  if (schema_.fields().empty()) {
    LOG_WARN("Got empty schema");
    return;
  }
  schema_.print(os);
  
  for (const Tuple &item : tuples_) {
    const std::vector<std::shared_ptr<TupleValue>> &values = item.values();
    for (std::vector<std::shared_ptr<TupleValue>>::const_iterator iter = values.begin(), end = --values.end();
          iter != end; ++iter) {
      (*iter)->to_string(os);
      os << " | ";
    }
    values.back()->to_string(os);
    os << std::endl;
  }
}

void TupleSet::print_tuple(std::ostream &os) const {
  
  for (const Tuple &item : tuples_) {
    const std::vector<std::shared_ptr<TupleValue>> &values = item.values();
    for (std::vector<std::shared_ptr<TupleValue>>::const_iterator iter = values.begin(), end = --values.end();
          iter != end; ++iter) {
      (*iter)->to_string(os);
      os << " | ";
    }
    values.back()->to_string(os);
    os << std::endl;
  }
}

void TupleSet::print(std::ostream &os, bool print_table_name) const{
  if (schema_.fields().empty()) {
    LOG_WARN("Got empty schema");
    return;
  }

  schema_.print(os, print_table_name);

  for (const Tuple &item : tuples_) {
    const std::vector<std::shared_ptr<TupleValue>> &values = item.values();
    for (std::vector<std::shared_ptr<TupleValue>>::const_iterator iter = values.begin(), end = --values.end();
          iter != end; ++iter) {
      (*iter)->to_string(os);
      os << " | ";
    }
    values.back()->to_string(os);
    os << std::endl;
  }
}

void TupleSet::set_schema(const TupleSchema &schema) {
  schema_ = schema;
}

std::string TupleSet::to_string(int index) const {
  std::string re;
  const Tuple &tuple = tuples_[index];
  if (tuple.size() == 0) {
    LOG_WARN("Got empty tuple");
    return re;
  }
  for (size_t i=0; i<tuple.size()-1; i++) {
    re += tuple.get(i).to_string() + " | ";
  }
  re += tuple.get(tuple.size()-1).to_string();
  return re;
}
std::string TupleSet::to_string(char *table_name) const {
  std::string re;
  if (schema_.fields().empty()) {
    LOG_WARN("Got empty schema");
    return re;
  }
  
  for (const Tuple &item : tuples_) {
    const std::vector<std::shared_ptr<TupleValue>> &values = item.values();
    for (std::vector<std::shared_ptr<TupleValue>>::const_iterator iter = values.begin(), end = --values.end();
          iter != end; ++iter) {
      re += (*iter)->to_string();
      re += " | ";
    }
    re += values.back()->to_string() + "\n";
    // re += std::endl;
  }
  return re;
}

std::string TupleSet::header_to_string(bool printTableName) const {
  std::string re;
  if (schema_.fields().empty()) {
    LOG_WARN("Got empty schema");
    return re;
  }
  re = schema_.to_string(printTableName);
  return re;
}

bool compare_tuple( Tuple *tuple_p1, Tuple *tuple_p2, std::vector<int> &indexs, std::vector<int> &orders) {
  for (int i = 0; i<indexs.size(); i++) {
    if (tuple_p1->get_pointer(indexs[i])->type() == IS_NULL || tuple_p2->get_pointer(indexs[i])->type() == IS_NULL) {
      return false;
    }
    int cmp = tuple_p1->get_pointer(indexs[i])->compare(*(tuple_p2->get_pointer(indexs[i])));
    if (orders[i] > 0) {
      if (cmp > 0) {
        return true;
      } else if (cmp < 0) {
        return false;
      } else {
        continue;
      }
    } else {
      if (cmp < 0) {
        return true;
      } else if (cmp > 0) {
        return false;
      } else {
        continue;
      }
    }
  }
  return false;
}

RC TupleSet::sort(const Selects &selects) {
  std::vector<int> indexs;
  std::vector<int> orders;

  for (int i = selects.condition_num - 1; i >= 0; i--)
  {
    const Condition &condition = selects.conditions[i];
    CompOp compop = condition.comp;

    if (compop == ORDER_BY_ASC) {
      /* code */
      const RelAttr &left_attr = condition.left_attr;
      int compare_index = -1;
      if (left_attr.relation_name == nullptr) {
        if (selects.relation_num != 1) {
          return RC::SCHEMA_FIELD_MISSING;
        }
        compare_index = schema_.index_of_field(selects.relations[0], left_attr.attribute_name);
      } else {
        compare_index = schema_.index_of_field(left_attr.relation_name, left_attr.attribute_name);
      }
      if (compare_index == -1) {
        return RC::SCHEMA_FIELD_MISSING;
      }
      indexs.push_back(compare_index);
      orders.push_back(1);
    } else if (compop == ORDER_BY_DESC) {
      const RelAttr &left_attr = condition.left_attr;
      int compare_index = -1;
      if (left_attr.relation_name == nullptr) {
        if (selects.relation_num != 1) {
          return RC::SCHEMA_FIELD_MISSING;
        }
        compare_index = schema_.index_of_field(selects.relations[0], left_attr.attribute_name);
      } else {
        compare_index = schema_.index_of_field(left_attr.relation_name, left_attr.attribute_name);
      }
      
      if (compare_index == -1) {
        return RC::GENERIC_ERROR;
      }
      indexs.push_back(compare_index);
      orders.push_back(-1);
    }
  }
  for (size_t j = 0; j < tuples_.size(); j++) {
    size_t min = j;
    Tuple *tuple_p1 = &tuples_[min];
    Tuple *tuple_p2 = nullptr;
    for (size_t k = j + 1; k < tuples_.size(); k++)
    {
      tuple_p2 = &tuples_[k];
      if (compare_tuple(tuple_p1, tuple_p2, indexs, orders))
      {
        min = k;
        tuple_p1 = &tuples_[min];
      }
    }
    if (min != j)
    {
      tuple_p2 = &tuples_[j];
      Tuple tmp = std::move(tuples_[min]);
      tuples_[min] = std::move(tuples_[j]);
      tuples_[j] = std::move(tmp);
    }
  }
  return RC::SUCCESS;
}

const TupleSchema &TupleSet::get_schema() const {
  return schema_;
}

bool TupleSet::is_empty() const {
  return tuples_.empty();
}

int TupleSet::size() const {
  return tuples_.size();
}

const Tuple &TupleSet::get(int index) const {
  return tuples_[index];
}

const std::vector<Tuple> &TupleSet::tuples() const {
  return tuples_;
}

/////////////////////////////////////////////////////////////////////////////
TupleRecordConverter::TupleRecordConverter(Table *table, TupleSet &tuple_set) :
      table_(table), tuple_set_(tuple_set){
}

// void TupleRecordConverter::int2date(int date, char *str) {
//   int x;
//   for (int i=9; i>=0; i--) {
//     if (i == 7 || i == 4) {
//       str[i] = '-';
//       continue;
//     }
//     x = date % 10;
//     date /= 10;
//     str[i] = x + '0';
//   }
// }

void TupleRecordConverter::add_record(const char *record) {
  const TupleSchema &schema = tuple_set_.schema();
  Tuple tuple;
  const TableMeta &table_meta = table_->table_meta();
  for (const TupleField &field : schema.fields()) {
    const FieldMeta *field_meta = table_meta.field(field.field_name());
    assert(field_meta != nullptr);
    switch (field_meta->type()) {
      case TEXT: {
        const char *s = record + field_meta->offset();
        char * out_text;
        std::string text_file_name = s; 
        std::string line;
        std::fstream fs;
        fs.open(text_file_name, std::ios_base::in | std::ios_base::binary);
        std::getline(fs, line);
        const char * out = line.c_str();
        tuple.add(out, strlen(out));
      }
      case INTS: {
        int value = *(int*)(record + field_meta->offset());
        tuple.add(value);
      }
      break;
      case INTS_NULLABLE: {
        int is_null = *(int*)(record + field_meta->offset() + field_meta->len());
        if (is_null == 1) {
          tuple.add();
        } else {
          int value = *(int*)(record + field_meta->offset());
          tuple.add(value);
        }
      }
      break;
      case FLOATS: {
        float value = *(float *)(record + field_meta->offset());
        tuple.add(value);
      }
      break;
      case FLOATS_NULLABLE: {
        int is_null = *(int*)(record + field_meta->offset() + field_meta->len());
        if (is_null == 1) {
          tuple.add();
        } else {
          int value = *(float*)(record + field_meta->offset());
          tuple.add(value);
        }
      }
      break;
      case CHARS: {
        const char *s = record + field_meta->offset();  // 现在当做Cstring来处理
        int length = field_meta->len();
        if(length < strlen(s))
          tuple.add(s, length);
        else  
          tuple.add(s, strlen(s));
      }
      break;
      case CHARS_NULLABLE: {
        int is_null = *(int*)(record + field_meta->offset() + field_meta->len());
        if (is_null == 1) {
          tuple.add();
        } else {
          const char *s = record + field_meta->offset();  // 现在当做Cstring来处理
          int length = field_meta->len();
          if(length < strlen(s))
            tuple.add(s, length);
          else  
            tuple.add(s, strlen(s));
        }
      }
      break;
      case DATES: {
        // Xing 待修改
        int date_int = *(int*)(record + field_meta->offset());
        // tuple.add(value);
        // char date_str[11] = {0, };
        // int2date(date, date_str);
        MyDate date(date_int);
        const char *date_cstr = date.toCStr();
        tuple.add(date_cstr, strlen(date_cstr));
      }
      break;
      case DATES_NULLABLE: {
        int is_null = *(int*)(record + field_meta->offset() + field_meta->len());
        if (is_null == 1) {
          tuple.add();
        } else {
          int date_int = *(int*)(record + field_meta->offset());
          // tuple.add(value);
          // char date_str[11] = {0, };
          // int2date(date, date_str);
          MyDate date(date_int);
          const char *date_cstr = date.toCStr();
          tuple.add(date_cstr, strlen(date_cstr));
        }
      }
      break;
      default: {
        LOG_PANIC("Unsupported field type. type=%d", field_meta->type());
      }
    }
  }

  // for ()

  tuple_set_.add(std::move(tuple));
}


