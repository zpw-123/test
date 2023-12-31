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
// Created by Wangyunlai on 2021/5/7.
//

#include <stddef.h>
#include "condition_filter.h"
#include "record_manager.h"
#include "common/log/log.h"
#include "storage/common/table.h"
#include "storage/common/mydate.h"

using namespace common;

ConditionFilter::~ConditionFilter()
{}

DefaultConditionFilter::DefaultConditionFilter()
{
  left_.is_attr = false;
  left_.attr_length = 0;
  left_.attr_offset = 0;
  left_.value = nullptr;

  right_.is_attr = false;
  right_.attr_length = 0;
  right_.attr_offset = 0;
  right_.value = nullptr;
}
DefaultConditionFilter::~DefaultConditionFilter()
{}

RC DefaultConditionFilter::init(const ConDesc &left, const ConDesc &right, AttrType attr_type, CompOp comp_op, TupleSet *tuple_set, TupleSet *tuple_set_left)
{
  if (attr_type < CHARS || attr_type > NOT_NULL) {
    LOG_ERROR("Invalid condition with unsupported attribute type: %d", attr_type);
    return RC::INVALID_ARGUMENT;
  }

  if (comp_op < EQUAL_TO || comp_op >= NO_OP) {
    LOG_ERROR("Invalid condition with unsupported compare operation: %d", comp_op);
    return RC::INVALID_ARGUMENT;
  }

  left_ = left;
  right_ = right;
  attr_type_ = attr_type;
  comp_op_ = comp_op;
  tuple_set_ = tuple_set;
  tuple_set_left_ = tuple_set_left;
  return RC::SUCCESS;
}

RC DefaultConditionFilter::init(Table &table, const Condition &condition)
{
  const TableMeta &table_meta = table.table_meta();
  ConDesc left;
  ConDesc right;

  AttrType type_left = UNDEFINED;
  AttrType type_right = UNDEFINED;

  if (1 == condition.left_is_attr) {
    left.is_attr = true;
    const FieldMeta *field_left = table_meta.field(condition.left_attr.attribute_name);
    if (nullptr == field_left) {
      LOG_WARN("No such field in condition. %s.%s", table.name(), condition.left_attr.attribute_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    left.attr_length = field_left->len();
    left.attr_offset = field_left->offset();

    left.value = nullptr;

    type_left = field_left->type();
    left.type = type_left;
  } else if (condition.tuple_set_left_ == nullptr) {
    left.is_attr = false;
    left.value = condition.left_value.data;  // 校验type 或者转换类型
    type_left = condition.left_value.type;
    left.type = type_left;
    left.attr_length = 0;
    left.attr_offset = 0;
  }

  if (1 == condition.right_is_attr) {
    right.is_attr = true;
    const FieldMeta *field_right = table_meta.field(condition.right_attr.attribute_name);
    if (nullptr == field_right) {
      LOG_WARN("No such field in condition. %s.%s", table.name(), condition.right_attr.attribute_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    right.attr_length = field_right->len();
    right.attr_offset = field_right->offset();
    type_right = field_right->type();
    right.type = type_right;
    right.value = nullptr;
  } else if (condition.is_select != 1){
    right.is_attr = false;
    right.value = condition.right_value.data;
    type_right = condition.right_value.type;
    right.type = type_right;
    right.attr_length = 0;
    right.attr_offset = 0;
  }

  // 校验和转换
  //  if (!field_type_compare_compatible_table[type_left][type_right]) {
  //    // 不能比较的两个字段， 要把信息传给客户端
  //    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  //  }
  // NOTE：这里没有实现不同类型的数据比较，比如整数跟浮点数之间的对比
  // 但是选手们还是要实现。这个功能在预选赛中会出现
  // if (type_left != type_right) {
  //   if (type_left == DATES || type_right == CHARS) {
  //     MyDate date((char *)right.value);
  //     if (date.toInt() == -1) {
  //       return RC::GENERIC_ERROR;
  //     }
  //   } else {
  //     return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  //   }
  // }
  if (condition.is_select) {
    TupleSet *tuple_set = (TupleSet *)condition.tuple_set_;
    TupleSet *tuple_set_left = (TupleSet *)condition.tuple_set_left_;
    if (tuple_set->size() == 0 || (tuple_set_left_ != nullptr && tuple_set_left->size() == 0)) {
      return init(left, right, type_left, condition.comp, (TupleSet *)condition.tuple_set_, (TupleSet *)condition.tuple_set_left_);
    }
    if (tuple_set_left_ != nullptr) {
      type_left = tuple_set_left_->get(0).get(0).type();
    }
    type_right = tuple_set->get(0).get(0).type();
    if (type_left == CHARS) {
      if (type_right != CHARS && type_right != IS_NULL) {
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
    } else if (type_left == CHARS_NULLABLE) {
      if (type_right != CHARS && type_right != IS_NULL && type_right != NOT_NULL) {
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
    } else if (type_left == INTS) {
      if (type_right != INTS && type_right != IS_NULL && type_right != FLOATS) {
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
    } else if (type_left == INTS_NULLABLE) {
      if (type_right != INTS && type_right != IS_NULL && type_right != NOT_NULL && type_right != FLOATS) {
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
    } else if (type_left == FLOATS) {
      if (type_right != FLOATS && type_right != IS_NULL && type_right != INTS) {
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
    } else if (type_left == FLOATS_NULLABLE) {
      if (type_right != INTS && type_right != IS_NULL && type_right != NOT_NULL && type_right != INTS) {
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
    } else if (type_left == DATES) {
      if (type_right != CHARS && type_right != IS_NULL) {
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
      MyDate date((char *)right.value);
      if (date.toInt() == -1) {
        return RC::GENERIC_ERROR;
      }
    } else if (type_left == DATES_NULLABLE) {
      if (type_right != CHARS && type_right != IS_NULL && type_right != NOT_NULL) {
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
      if (type_right == CHARS) {
        MyDate date((char *)right.value);
        if (date.toInt() == -1) {
          return RC::GENERIC_ERROR;
        }
      }
    }
    return init(left, right, type_left, condition.comp, (TupleSet *)condition.tuple_set_, (TupleSet *)condition.tuple_set_left_);
  }



  if (type_left == CHARS) {
    if (type_right != CHARS && type_right != IS_NULL) {
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  } else if (type_left == CHARS_NULLABLE) {
    if (type_right != CHARS && type_right != IS_NULL && type_right != NOT_NULL) {
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  } else if (type_left == INTS) {
    if (type_right != INTS && type_right != IS_NULL) {
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  } else if (type_left == INTS_NULLABLE) {
    if (type_right != INTS && type_right != IS_NULL && type_right != NOT_NULL) {
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  } else if (type_left == FLOATS) {
    if (type_right != FLOATS && type_right != IS_NULL) {
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  } else if (type_left == FLOATS_NULLABLE) {
    if (type_right != INTS && type_right != IS_NULL && type_right != NOT_NULL) {
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  } else if (type_left == DATES) {
    if (type_right != CHARS && type_right != IS_NULL) {
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
    MyDate date((char *)right.value);
    if (date.toInt() == -1) {
       return RC::GENERIC_ERROR;
    }
  } else if (type_left == DATES_NULLABLE) {
    if (type_right != CHARS && type_right != IS_NULL && type_right != NOT_NULL) {
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
    if (type_right == CHARS) {
      MyDate date((char *)right.value);
      if (date.toInt() == -1) {
        return RC::GENERIC_ERROR;
      }
    }
  }


  return init(left, right, type_left, condition.comp, nullptr, nullptr);
}

bool DefaultConditionFilter::filter(const Record &rec) const
{
  char *left_value = nullptr;
  char *right_value = nullptr;

  if (left_.is_attr) {  // value
    left_value = (char *)(rec.data + left_.attr_offset);
  } else {
    left_value = (char *)left_.value;
  }

  if (right_.is_attr && tuple_set_ != nullptr) {
    right_value = (char *)(rec.data + right_.attr_offset);
  } else {
    right_value = (char *)right_.value;
  }

  int cmp_result = 0;
  switch (attr_type_) {
    case CHARS: {  // 字符串都是定长的，直接比较
      // 按照C字符串风格来定
      if (right_value == nullptr) {
        if (comp_op_ == IS_NOT) {
          return true;
        }
        return false;
      }
      if (tuple_set_ != nullptr) {
        if (tuple_set_->size() == 0) {
          if (comp_op_ == NOT_IN) {
            return true;
          }
          return false;
        }
        if (tuple_set_left_ != nullptr) {
          return false;
        }
        int left = *(int *)left_value;
        // int right = *(int *)right_value;
        const TupleValue &tuple_value = tuple_set_->get(0).get(0);
        AttrType right_type = tuple_value.type();
        if (right_type == IS_NULL) {
          switch (comp_op_) {
            case IN:
              return false;
            case NOT_IN:
              return false;
            default:
              return false;
          }
        }
        if (comp_op_ == IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            // float right = std::stof(value_str);
            // float re = left - right;
            int re = strcmp(left_value, value_str.c_str());
            if (re == 0) {
              return true;
            }
          }
          return false;
        } else if (comp_op_ == NOT_IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            int re = strcmp(left_value, value_str.c_str());
            if (re == 0) {
              return true;
            }
          }
          return true;
        }
        std::string value_str = tuple_value.to_string();
        cmp_result = strcmp(left_value, right_value);
        break;
      }
      cmp_result = strcmp(left_value, right_value);
    } break;
    case CHARS_NULLABLE: {  // 字符串都是定长的，直接比较
      // 按照C字符串风格来定
      int is_null = *(int *)(rec.data + left_.attr_offset + left_.attr_length);
      if (is_null) {
        if (comp_op_ == IS && (right_value == nullptr || left_value == nullptr)) {
          return true;
        }
        return false;
      }
      if (tuple_set_ != nullptr) {
        if (tuple_set_->size() == 0) {
          if (comp_op_ == NOT_IN) {
            return true;
          }
          return false;
        }
        if (tuple_set_left_ != nullptr) {
          return false;
        }
        int left = *(int *)left_value;
        // int right = *(int *)right_value;
        const TupleValue &tuple_value = tuple_set_->get(0).get(0);
        AttrType right_type = tuple_value.type();
        if (right_type == IS_NULL) {
          switch (comp_op_) {
            case IN:
              return false;
            case NOT_IN:
              return false;
            default:
              return false;
          }
        }
        if (comp_op_ == IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            // float right = std::stof(value_str);
            // float re = left - right;
            int re = strcmp(left_value, value_str.c_str());
            if (re == 0) {
              return true;
            }
          }
          return false;
        } else if (comp_op_ == NOT_IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            int re = strcmp(left_value, value_str.c_str());
            if (re == 0) {
              return true;
            }
          }
          return true;
        }
        std::string value_str = tuple_value.to_string();
        cmp_result = strcmp(left_value, right_value);
        break;
      }
      if (left_value == nullptr || right_value == nullptr) {
        if (comp_op_ == IS_NOT) {
          return true;
        }
        return false;
      }
      cmp_result = strcmp(left_value, right_value);
    } break;
    case DATES: { 
      // Xing 待修改
      // cmp_result = strcmp(left_value, right_value);
      // 没有考虑大小端问题
      // 对int和float，要考虑字节对齐问题,有些平台下直接转换可能会跪
      if (right_value == nullptr) {
        if (comp_op_ == IS_NOT) {
          return true;
        }
        return false;
      }
      int left = *(int *)left_value;
      MyDate date(right_value);
      int right = date.toInt();
      cmp_result = left - right;
    } break;
    case DATES_NULLABLE: {
      int is_null = *(int *)(rec.data + left_.attr_offset + left_.attr_length);
      if (is_null) {
        if (comp_op_ == IS && (right_value == nullptr || left_value == nullptr)) {
          return true;
        }
        return false;
      }
      if (left_value == nullptr || right_value == nullptr) {
        if (comp_op_ == IS_NOT) {
          return true;
        }
        return false;
      }
      int left = *(int *)left_value;
      MyDate date(right_value);
      int right = date.toInt();
      cmp_result = left - right;
    } break;
    case INTS: {
      // 没有考虑大小端问题
      // 对int和float，要考虑字节对齐问题,有些平台下直接转换可能会跪
      if (tuple_set_ != nullptr) {
        if (tuple_set_->size() == 0) {
          if (comp_op_ == NOT_IN) {
            return true;
          }
          return false;
        }
        if (tuple_set_left_ != nullptr) {
          return false;
        }
        int left = *(int *)left_value;
        // int right = *(int *)right_value;
        const TupleValue &tuple_value = tuple_set_->get(0).get(0);
        AttrType right_type = tuple_value.type();
        if (right_type == IS_NULL) {
          switch (comp_op_) {
            case IN:
              return false;
            case NOT_IN:
              return false;
            default:
              return false;
          }
        }
        if (comp_op_ == IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            float right = std::stof(value_str);
            float re = left - right;
            if (re == 0) {
              return true;
            }
          }
          return false;
        } else if (comp_op_ == NOT_IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            float right = std::stof(value_str);
            float re = left - right;
            if (re == 0) {
              return false;
            }
          }
          return true;
        }
        std::string value_str = tuple_value.to_string();
        float right = std::stof(value_str);
        float re = left - right;
        if (re > 0) {
          cmp_result = 1;
        } else if (re == 0) {
          cmp_result = 0;
        } else {
          cmp_result = -1;
        }
        break;
      }
      if (right_value == nullptr) {
        if (comp_op_ == IS_NOT) {
          return true;
        }
        return false;
      }
      
      int left = *(int *)left_value;
      int right = *(int *)right_value;
      cmp_result = left - right;
    } break;
    case INTS_NULLABLE: {
      // 没有考虑大小端问题
      // 对int和float，要考虑字节对齐问题,有些平台下直接转换可能会跪
      int is_null = *(int *)(rec.data + left_.attr_offset + left_.attr_length);
      if (is_null) {
        if (comp_op_ == IS && (right_value == nullptr || left_value == nullptr)) {
          return true;
        }
        return false;
      }
      if (tuple_set_ != nullptr) {
        if (tuple_set_->size() == 0) {
          if (comp_op_ == NOT_IN) {
            return true;
          }
          return false;
        }
        if (tuple_set_left_ != nullptr) {
          return false;
        }
        int left = *(int *)left_value;
        // int right = *(int *)right_value;
        const TupleValue &tuple_value = tuple_set_->get(0).get(0);
        AttrType right_type = tuple_value.type();
        if (right_type == IS_NULL) {
          switch (comp_op_) {
            case IN:
              return false;
            case NOT_IN:
              return false;
            default:
              return false;
          }
        }
        if (comp_op_ == IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            float right = std::stof(value_str);
            float re = left - right;
            if (re == 0) {
              return true;
            }
          }
          return false;
        } else if (comp_op_ == NOT_IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            float right = std::stof(value_str);
            float re = left - right;
            if (re == 0) {
              return false;
            }
          }
          return true;
        }
        std::string value_str = tuple_value.to_string();
        float right = std::stof(value_str);
        float re = left - right;
        if (re > 0) {
          cmp_result = 1;
        } else if (re == 0) {
          cmp_result = 0;
        } else {
          cmp_result = -1;
        }
        break;
      }
      if (left_value == nullptr || right_value == nullptr) {
        if (comp_op_ == IS_NOT) {
          return true;
        }
        return false;
      }
      
      int left = *(int *)left_value;
      int right = *(int *)right_value;
      cmp_result = left - right;
    } break;
    case FLOATS: {
      if (tuple_set_ != nullptr) {
        if (tuple_set_->size() == 0) {
          if (comp_op_ == NOT_IN) {
            return true;
          }
          return false;
        }
        if (tuple_set_left_ != nullptr) {
          return false;
        }
        float left = *(float *)left_value;
        // int right = *(int *)right_value;
        const TupleValue &tuple_value = tuple_set_->get(0).get(0);
        AttrType right_type = tuple_value.type();
        if (right_type == IS_NULL) {
          switch (comp_op_) {
            case IN:
              return false;
            case NOT_IN:
              return false;
            default:
              return false;
          }
        }
        if (comp_op_ == IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            float right = std::stof(value_str);
            float re = left - right;
            if (re == 0) {
              return true;
            }
          }
          return false;
        } else if (comp_op_ == NOT_IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            float right = std::stof(value_str);
            float re = left - right;
            if (re == 0) {
              return false;
            }
          }
          return true;
        }
        std::string value_str = tuple_value.to_string();
        float right = std::stof(value_str);
        float re = left - right;
        if (re > 0) {
          cmp_result = 1;
        } else if (re == 0) {
          cmp_result = 0;
        } else {
          cmp_result = -1;
        }
        break;
      }
      if (right_value == nullptr) {
        if (comp_op_ == IS_NOT) {
          return true;
        }
        return false;
      }
      
      float left = *(float *)left_value;
      float right = *(float *)right_value;
      float ans = left - right;
      if(ans < 0)
        cmp_result = -1;
      else if(ans > 0)
        cmp_result = 1;
      else
        cmp_result = 0;
      //cmp_result = (int)(left - right);
    } break;
    case FLOATS_NULLABLE: {
      if (tuple_set_ != nullptr) {
        if (tuple_set_->size() == 0) {
          if (comp_op_ == NOT_IN) {
            return true;
          }
          return false;
        }
        if (tuple_set_left_ != nullptr) {
          return false;
        }
        float left = *(float *)left_value;
        // int right = *(int *)right_value;
        const TupleValue &tuple_value = tuple_set_->get(0).get(0);
        AttrType right_type = tuple_value.type();
        if (right_type == IS_NULL) {
          switch (comp_op_) {
            case IN:
              return false;
            case NOT_IN:
              return false;
            default:
              return false;
          }
        }
        if (comp_op_ == IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            float right = std::stof(value_str);
            float re = left - right;
            if (re == 0) {
              return true;
            }
          }
          return false;
        } else if (comp_op_ == NOT_IN) {
          for (int i=0; i<tuple_set_->size(); i++) {
            const Tuple &tuple = tuple_set_->get(i);
            std::string value_str = tuple.get(0).to_string();
            float right = std::stof(value_str);
            float re = left - right;
            if (re == 0) {
              return false;
            }
          }
          return true;
        }
        std::string value_str = tuple_value.to_string();
        float right = std::stof(value_str);
        float re = left - right;
        if (re > 0) {
          cmp_result = 1;
        } else if (re == 0) {
          cmp_result = 0;
        } else {
          cmp_result = -1;
        }
        break;
      }
      int is_null = *(int *)(rec.data + left_.attr_offset + left_.attr_length);
      if (is_null) {
        if (comp_op_ == IS && (right_value == nullptr || left_value == nullptr)) {
          return true;
        }
        return false;
      }
      if (left_value == nullptr || right_value == nullptr) {
        if (comp_op_ == IS_NOT) {
          return true;
        }
        return false;
      }
      
      float left = *(float *)left_value;
      float right = *(float *)right_value;
      cmp_result = (int)(left - right);
    } break;
    case IS_NULL: {
      if (comp_op_ == IS && right_value == nullptr) {
        return true;
      } else if (comp_op_ == IS_NOT && right_value != nullptr) {
        return true;
      }
      return false;
    } break;
    default: {
    }
  }

  switch (comp_op_) {
    case EQUAL_TO:
      return 0 == cmp_result;
    case LESS_EQUAL:
      return cmp_result <= 0;
    case NOT_EQUAL:
      return cmp_result != 0;
    case LESS_THAN:
      return cmp_result < 0;
    case GREAT_EQUAL:
      return cmp_result >= 0;
    case GREAT_THAN:
      return cmp_result > 0;

    default:
      break;
  }

  LOG_PANIC("Never should print this.");
  return cmp_result;  // should not go here
}

bool DefaultConditionFilter::filter(const TupleSchema &schema_, const Tuple &tuple) const
{
  std::shared_ptr<TupleValue> left_value = nullptr;
  std::shared_ptr<TupleValue> right_value = nullptr;

  int left_index = schema_.index_of_field(left_.table_name, left_.attr_name);
  int right_index = schema_.index_of_field(right_.table_name, right_.attr_name);
  left_value = tuple.get_pointer(left_index);
  right_value = tuple.get_pointer(right_index);

  if (left_value->type() == IS_NULL || right_value->type() == IS_NULL) {
    return false;
  }
  int cmp_result = 0;
  cmp_result = left_value->compare(*right_value);

  switch (comp_op_) {
    case EQUAL_TO:
      return 0 == cmp_result;
    case LESS_EQUAL:
      return cmp_result <= 0;
    case NOT_EQUAL:
      return cmp_result != 0;
    case LESS_THAN:
      return cmp_result < 0;
    case GREAT_EQUAL:
      return cmp_result >= 0;
    case GREAT_THAN:
      return cmp_result > 0;

    default:
      break;
  }

  LOG_PANIC("Never should print this.");
  return cmp_result;  // should not go here
}

CompositeConditionFilter::~CompositeConditionFilter()
{
  if (memory_owner_) {
    delete[] filters_;
    filters_ = nullptr;
  }
}

RC CompositeConditionFilter::init(const ConditionFilter *filters[], int filter_num, bool own_memory)
{
  filters_ = filters;
  filter_num_ = filter_num;
  memory_owner_ = own_memory;
  return RC::SUCCESS;
}
RC CompositeConditionFilter::init(const ConditionFilter *filters[], int filter_num)
{
  return init(filters, filter_num, false);
}

RC CompositeConditionFilter::init(Table &table, const Condition *conditions, int condition_num)
{
  if (condition_num == 0) {
    return RC::SUCCESS;
  }
  if (conditions == nullptr) {
    return RC::INVALID_ARGUMENT;
  }

  RC rc = RC::SUCCESS;
  ConditionFilter **condition_filters = new ConditionFilter *[condition_num];
  for (int i = 0; i < condition_num; i++) {
    DefaultConditionFilter *default_condition_filter = new DefaultConditionFilter();
    rc = default_condition_filter->init(table, conditions[i]);
    if (rc != RC::SUCCESS) {
      delete default_condition_filter;
      for (int j = i - 1; j >= 0; j--) {
        delete condition_filters[j];
        condition_filters[j] = nullptr;
      }
      delete[] condition_filters;
      condition_filters = nullptr;
      return rc;
    }
    condition_filters[i] = default_condition_filter;
  }
  return init((const ConditionFilter **)condition_filters, condition_num, true);
}

bool CompositeConditionFilter::filter(const Record &rec) const
{
  for (int i = 0; i < filter_num_; i++) {
    if (!filters_[i]->filter(rec)) {
      return false;
    }
  }
  return true;
}

bool CompositeConditionFilter::filter(const TupleSchema &schema_, const Tuple &tuple) const 
{
  for (int i = 0; i < filter_num_; i++) {
    if (!filters_[i]->filter(schema_, tuple)) {
      return false;
    }
  }
  return true;
}