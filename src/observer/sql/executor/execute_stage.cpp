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
// Created by Longda on 2021/4/13.
//

#include <string>
#include <sstream>
#include <unordered_map>

#include "execute_stage.h"

#include "common/io/io.h"
#include "common/log/log.h"
#include "common/seda/timer_stage.h"
#include "common/lang/string.h"
#include "session/session.h"
#include "event/storage_event.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "event/execution_plan_event.h"
#include "sql/executor/execution_node.h"
#include "sql/executor/tuple.h"
#include "storage/common/table.h"
#include "storage/default/default_handler.h"
#include "storage/common/condition_filter.h"
#include "storage/trx/trx.h"

using namespace common;

RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node);
RC create_join_selection_executor(Trx *trx, const Selects &selects, const char *db, std::vector<TupleSet> *tuple_sets, std::unordered_map<std::string,int> *table_map, JoinSelectExeNode &join_select_node);
//! Constructor
ExecuteStage::ExecuteStage(const char *tag) : Stage(tag) {}

//! Destructor
ExecuteStage::~ExecuteStage() {}

//! Parse properties, instantiate a stage object
Stage *ExecuteStage::make_stage(const std::string &tag) {
  ExecuteStage *stage = new (std::nothrow) ExecuteStage(tag.c_str());
  if (stage == nullptr) {
    LOG_ERROR("new ExecuteStage failed");
    return nullptr;
  }
  stage->set_properties();
  return stage;
}

//! Set properties for this object set in stage specific properties
bool ExecuteStage::set_properties() {
  //  std::string stageNameStr(stageName);
  //  std::map<std::string, std::string> section = theGlobalProperties()->get(
  //    stageNameStr);
  //
  //  std::map<std::string, std::string>::iterator it;
  //
  //  std::string key;

  return true;
}

//! Initialize stage params and validate outputs
bool ExecuteStage::initialize() {
  LOG_TRACE("Enter");

  std::list<Stage *>::iterator stgp = next_stage_list_.begin();
  default_storage_stage_ = *(stgp++);
  mem_storage_stage_ = *(stgp++);

  LOG_TRACE("Exit");
  return true;
}

//! Cleanup after disconnection
void ExecuteStage::cleanup() {
  LOG_TRACE("Enter");

  LOG_TRACE("Exit");
}

void ExecuteStage::handle_event(StageEvent *event) {
  LOG_TRACE("Enter\n");

  handle_request(event);

  LOG_TRACE("Exit\n");
  return;
}

void ExecuteStage::callback_event(StageEvent *event, CallbackContext *context) {
  LOG_TRACE("Enter\n");

  // here finish read all data from disk or network, but do nothing here.
  ExecutionPlanEvent *exe_event = static_cast<ExecutionPlanEvent *>(event);
  SQLStageEvent *sql_event = exe_event->sql_event();
  sql_event->done_immediate();

  LOG_TRACE("Exit\n");
  return;
}

void ExecuteStage::handle_request(common::StageEvent *event) {
  ExecutionPlanEvent *exe_event = static_cast<ExecutionPlanEvent *>(event);
  SessionEvent *session_event = exe_event->sql_event()->session_event();
  Query *sql = exe_event->sqls();
  const char *current_db = session_event->get_client()->session->get_current_db().c_str();

  CompletionCallback *cb = new (std::nothrow) CompletionCallback(this, nullptr);
  if (cb == nullptr) {
    LOG_ERROR("Failed to new callback for ExecutionPlanEvent");
    exe_event->done_immediate();
    return;
  }
  exe_event->push_callback(cb);

  switch (sql->flag) {
    case SCF_SELECT: { // select
      do_select(current_db, sql, exe_event->sql_event()->session_event());
      exe_event->done_immediate();
    }
    break;

    case SCF_INSERT:
    case SCF_UPDATE:
    case SCF_DELETE:
    case SCF_CREATE_TABLE:
    case SCF_SHOW_TABLES:
    case SCF_DESC_TABLE:
    case SCF_DROP_TABLE:
    case SCF_CREATE_INDEX:
    case SCF_DROP_INDEX: 
    case SCF_LOAD_DATA: {
      StorageEvent *storage_event = new (std::nothrow) StorageEvent(exe_event);
      if (storage_event == nullptr) {
        LOG_ERROR("Failed to new StorageEvent");
        event->done_immediate();
        return;
      }

      default_storage_stage_->handle_event(storage_event);
    }
    break;
    case SCF_SYNC: {
      RC rc = DefaultHandler::get_default().sync();
      session_event->set_response(strrc(rc));
      exe_event->done_immediate();
    }
    break;
    case SCF_BEGIN: {
      session_event->get_client()->session->set_trx_multi_operation_mode(true);
      session_event->set_response(strrc(RC::SUCCESS));
      exe_event->done_immediate();
    }
    break;
    case SCF_COMMIT: {
      Trx *trx = session_event->get_client()->session->current_trx();
      RC rc = trx->commit();
      session_event->get_client()->session->set_trx_multi_operation_mode(false);
      session_event->set_response(strrc(rc));
      exe_event->done_immediate();
    }
    break;
    case SCF_ROLLBACK: {
      Trx *trx = session_event->get_client()->session->current_trx();
      RC rc = trx->rollback();
      session_event->get_client()->session->set_trx_multi_operation_mode(false);
      session_event->set_response(strrc(rc));
      exe_event->done_immediate();
    }
    break;
    case SCF_HELP: {
      const char *response = "show tables;\n"
          "desc `table name`;\n"
          "create table `table name` (`column name` `column type`, ...);\n"
          "create index `index name` on `table` (`column`);\n"
          "insert into `table` values(`value1`,`value2`);\n"
          "update `table` set column=value [where `column`=`value`];\n"
          "delete from `table` [where `column`=`value`];\n"
          "select [ * | `columns` ] from `table`;\n";
      session_event->set_response(response);
      exe_event->done_immediate();
    }
    break;
    case SCF_EXIT: {
      // do nothing
      const char *response = "Unsupported\n";
      session_event->set_response(response);
      exe_event->done_immediate();
    }
    break;
    default: {
      exe_event->done_immediate();
      LOG_ERROR("Unsupported command=%d\n", sql->flag);
    }
  }
}

void end_trx_if_need(Session *session, Trx *trx, bool all_right) {
  if (!session->is_trx_multi_operation_mode()) {
    if (all_right) {
      trx->commit();
    } else {
      trx->rollback();
    }
  }
}

// 这里没有对输入的某些信息做合法性校验，比如查询的列名、where条件中的列名等，没有做必要的合法性校验
// 需要补充上这一部分. 校验部分也可以放在resolve，不过跟execution放一起也没有关系
RC ExecuteStage::do_select2(const char *db, Query *sql, SessionEvent *session_event) {

  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();
  const Selects &selects = sql->sstr.selection;
  // 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
  std::vector<SelectExeNode *> select_nodes;
  for (size_t i = 0; i < selects.relation_num; i++) {
    const char *table_name = selects.relations[i];
    SelectExeNode *select_node = new SelectExeNode;
    rc = create_selection_executor(trx, selects, db, table_name, *select_node);
    if (rc != RC::SUCCESS) {
      delete select_node;
      for (SelectExeNode *& tmp_node: select_nodes) {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      char response[256];
      snprintf(response, sizeof(response), "%s\n", rc == RC::SUCCESS ? "SUCCESS" : "FAILURE");
      session_event->set_response(response);
      return rc;
    }
    select_nodes.push_back(select_node);
  }

  if (select_nodes.empty()) {
    LOG_ERROR("No table given");
    end_trx_if_need(session, trx, false);
    return RC::SQL_SYNTAX;
  }

  std::vector<TupleSet> tuple_sets;
  for (SelectExeNode *&node: select_nodes) {
    TupleSet tuple_set;
    rc = node->execute(tuple_set);
    if (rc != RC::SUCCESS) {
      for (SelectExeNode *& tmp_node: select_nodes) {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      return rc;
    } else {
      tuple_sets.push_back(std::move(tuple_set));
    }
  }

  std::stringstream ss;
  if (tuple_sets.size() > 1) {
    // 本次查询了多张表，需要做join操作
  } else {
    // 当前只查询一张表，直接返回结果即可
    tuple_sets.front().print(ss);
  }

  for (SelectExeNode *& tmp_node: select_nodes) {
    delete tmp_node;
  }
  session_event->set_response(ss.str());
  end_trx_if_need(session, trx, true);
  return rc;
}

void dfs_helper(std::vector<TupleSet> &tuple_sets, size_t depth, std::string str, std::vector<std::string> &result) {
  if (depth == tuple_sets.size()) {
    
    result.push_back(str);
    return;
  }
  if (depth != 0) {
    str += " | ";
  }
  TupleSet &tuple_set = tuple_sets[depth];
  for (size_t i=0; i<tuple_set.size(); i++) {
    std::string tuple_str = str + tuple_set.to_string(i);
    dfs_helper(tuple_sets, depth+1, tuple_str, result);
  }
}


void dfs(std::vector<TupleSet> &tuple_sets, std::vector<std::string> &result) {
  dfs_helper(tuple_sets, 0, "", result);
}


void dfs_helper(std::vector<TupleSet> &tuple_sets, size_t depth, Tuple &tuple, TupleSet &join_tuple_set) {
  if (depth == tuple_sets.size()) {
    join_tuple_set.add(std::move(tuple));
    return;
  }
  TupleSet &tuple_set = tuple_sets[depth];
  for (size_t i=0; i<tuple_set.size(); i++) {
    Tuple tuple_(std::move(tuple));
    const Tuple &tuple_tmp = tuple_set.get(i);
    for (size_t j=0; j<tuple_tmp.size(); j++) {
      tuple_.add(tuple_tmp.get_pointer(j));
    }
    dfs_helper(tuple_sets, depth+1, tuple_, join_tuple_set);
  }
}

void dfs(std::vector<TupleSet> &tuple_sets, TupleSet &join_tuple_set) {
  TupleSchema schema;
  for (TupleSet &tuple_set: tuple_sets) {
    schema.append(tuple_set.get_schema());
  }
  join_tuple_set.set_schema(schema);
  Tuple tuple;
  dfs_helper(tuple_sets, 0, tuple, join_tuple_set);
}

void dfs_helper(std::vector<TupleSet> &tuple_sets, size_t depth, std::vector<std::shared_ptr<TupleValue>> &values, TupleSet &join_tuple_set) {
  if (depth == tuple_sets.size()) {
    Tuple tuple;
    for (std::shared_ptr<TupleValue> tuple_value: values) {
      tuple.add(tuple_value);
    }
    join_tuple_set.add(std::move(tuple));
    return;
  }
  TupleSet &tuple_set = tuple_sets[depth];
  for (size_t i=0; i<tuple_set.size(); i++) {
    // Tuple tuple_(std::move(tuple));
    const Tuple &tuple_tmp = tuple_set.get(i);
    for (size_t j=0; j<tuple_tmp.size(); j++) {
      // tuple_.add(tuple_tmp.get_pointer(j));
      values.push_back(tuple_tmp.get_pointer(j));
    }
    dfs_helper(tuple_sets, depth+1, values, join_tuple_set);
    for (size_t j=0; j<tuple_tmp.size(); j++) {
      // tuple_.add(tuple_tmp.get_pointer(j));
      values.pop_back();
    }
  }
}

void dfs2(std::vector<TupleSet> &tuple_sets, TupleSet &join_tuple_set) {
  TupleSchema schema;
  for (TupleSet &tuple_set: tuple_sets) {
    schema.append(tuple_set.get_schema());
  }
  join_tuple_set.set_schema(schema);
  std::vector<std::shared_ptr<TupleValue>>  values;
  dfs_helper(tuple_sets, 0, values, join_tuple_set);
}

std::string create_header(std::vector<TupleSet> tuple_sets, bool printTableName) {
  std::string header;

  for (const TupleSet &tuple_set: tuple_sets) {
    header += tuple_set.header_to_string(printTableName);
  }
}

enum AGGREGATION_TYPE {
  MAX, MIN, COUNT, AVG, NOT_KNOWN
};

AGGREGATION_TYPE is_aggregation_select(const char * attribute_name, char * &real_attribute_name) {
  if (attribute_name == nullptr) {
    return NOT_KNOWN;
  }
  if (attribute_name[0] == 'M' || attribute_name[0] == 'm') {
    if ((attribute_name[1] == 'A' || attribute_name[1] == 'a') 
      && (attribute_name[2] == 'X' || attribute_name[2] == 'x') 
      && attribute_name[3] == '(') {
      size_t len = strlen(attribute_name) + 1 - 4;
      real_attribute_name = new char[len];
      strcpy(real_attribute_name, attribute_name+4);
      real_attribute_name[len-2] = '\0';
      return MAX;
    }
    if ((attribute_name[1] == 'I' || attribute_name[1] == 'i') 
      && (attribute_name[2] == 'N' || attribute_name[2] == 'n') 
      && attribute_name[3] == '(') {
      size_t len = strlen(attribute_name) + 1 - 4;
      real_attribute_name = new char[len];
      strcpy(real_attribute_name, attribute_name+4);
      real_attribute_name[len-2] = '\0';
      return MIN;
    }
    return NOT_KNOWN;
  } else if ((attribute_name[0] == 'C' || attribute_name[0] == 'c') 
      && (attribute_name[1] == 'O' || attribute_name[1] == 'o') 
      && (attribute_name[2] == 'U' || attribute_name[2] == 'u')
      && (attribute_name[3] == 'N' || attribute_name[3] == 'n') 
      && (attribute_name[4] == 'T' || attribute_name[4] == 't') 
      && attribute_name[5] == '(') {
    size_t len = strlen(attribute_name) + 1 - 6;
    real_attribute_name = new char[len];
    strcpy(real_attribute_name, attribute_name+6);
    real_attribute_name[len-2] = '\0';
    return COUNT;
  } else if ((attribute_name[0] == 'A' || attribute_name[0] == 'a') 
      && (attribute_name[1] == 'V' || attribute_name[1] == 'v')
      && (attribute_name[2] == 'G' || attribute_name[2] == 'g') 
      && attribute_name[3] == '(') {
    size_t len = strlen(attribute_name) + 1 - 4;
    real_attribute_name = new char[len];
    strcpy(real_attribute_name, attribute_name+4);
    real_attribute_name[len-2] = '\0';
    return AVG;
  }
  return NOT_KNOWN;
}

void get_tuple_value(TupleSet &tuple_set, Tuple &tuple, AGGREGATION_TYPE type, int index, std::vector<int> &group_by_list, bool add_group_by_value) {
  // const TupleValue &value_
  // std::ostringstream os;
  // tuple_set.print_tuple(os);
  // os << std::endl;
  // tuple_set.print_tuple(os);

  // std::cout << os.str() << std::endl;
  if (type == MAX) {
    const std::shared_ptr<TupleValue> *p_value = nullptr;
    bool has_null = false;
    for (size_t i=0; i<tuple_set.size(); i++) {
      const Tuple &tuple_ = tuple_set.get(i);
      const std::shared_ptr<TupleValue> &value_ = tuple_.get_pointer(index);
      if (value_->type() == IS_NULL) {
        has_null = true;
        continue;
      }
      if (p_value == nullptr) {
        p_value = &value_;
        continue;
      }
      int cmp = (*p_value)->compare(*value_);
      if (cmp < 0) {
        p_value = &value_;
      }
    }
    if (p_value == nullptr) {
      // todo: 使用null类型
      tuple.add();
    } else {
      tuple.add(*p_value);
    }
  } else if (type == MIN) {
    bool has_null = false;
    const std::shared_ptr<TupleValue> *p_value = nullptr;
    for (size_t i=0; i<tuple_set.size(); i++) {
      const Tuple &tuple_ = tuple_set.get(i);
      const std::shared_ptr<TupleValue> &value_ = tuple_.get_pointer(index);
      if (value_->type() == IS_NULL) {
        has_null = true;
        // break;
        continue;
      }
      if (p_value == nullptr) {
        
        p_value = &value_;
        continue;
      }
      int cmp = (*p_value)->compare(*value_);
      if (cmp > 0) {
        p_value = &value_;
      }
    }
    if (p_value == nullptr) {
      tuple.add();
      // todo: 使用null类型
    } else {
      tuple.add(*p_value);
    }
  } else if (type == COUNT) {
    const std::shared_ptr<TupleValue> *p_value = nullptr;
    std::unordered_map<std::string, bool> value_exist_map;
    int count = 0;
    if (index == -1) {
      count = tuple_set.size();
    } else {
      for (size_t i=0; i<tuple_set.size(); i++) {
        const Tuple &tuple_ = tuple_set.get(i);
        const std::shared_ptr<TupleValue> &value_ = tuple_.get_pointer(index);
        if (value_->type() == IS_NULL) {
          continue;
        }
        std::string str = value_->to_string();
        count++;
        value_exist_map[str] = true;
      }
    }
    tuple.add(new IntValue(count));
  } else if (type == AVG) {
    if (tuple_set.size() == 0) {
      // todo: 使用null类型
      return;
    }
    bool all_null = false;
    bool not_all_null = false;
    const std::shared_ptr<TupleValue> *p_value = &(tuple_set.get(0).get_pointer(index));
    AttrType type = (*p_value)->type();
    float avg = 0;
    size_t div_count = 0;
    for (size_t i=0; i<tuple_set.size(); i++) {
      const Tuple &tuple_ = tuple_set.get(i);
      const std::shared_ptr<TupleValue> &value_ = tuple_.get_pointer(index);
      // std::cout << value_->to_string() << std::endl;
      if (value_->type() == IS_NULL) {
        if (not_all_null) {
          all_null = false;
        } else {
          all_null = true;
        }
        continue;
        // break;
      }
      not_all_null = true;
      avg += std::stof(value_->to_string());
      div_count++;
    }
    if (all_null) {
      tuple.add();
    } else {
      if (div_count != 0) {
      avg /= div_count;
      }
      tuple.add(new FloatValue(avg));
    }
    
  }
  if (group_by_list.size() > 0 && tuple_set.size() > 0 && add_group_by_value) {
    const Tuple &tuple_ = tuple_set.get(0);
    // tuple.add(tuple_.get_pointer)
    for (int i=0; i<group_by_list.size(); i++) {
      tuple.add(tuple_.get_pointer(group_by_list[i]));
    }
  }
}

bool same_tuple(const Tuple &tuple_1, const Tuple &tuple_2, std::vector<int> &indexs) {
  if (indexs.size() == 0) {
    return true;
  }
  for (int i = 0; i<indexs.size(); i++) {
    int cmp = tuple_1.get_pointer(indexs[i])->compare(*(tuple_2.get_pointer(indexs[i])));
    if (cmp != 0) {
      return false;
    }
  }
  return true;
}

void split_tuple_set(TupleSet &tuple_set, std::vector<int> &group_by_list, std::vector<TupleSet> &split_tuple_sets) {
  const std::vector<Tuple> &tuples = tuple_set.tuples();
  for (size_t i=0; i<tuple_set.size(); i++) {
    // const Tuple *tuple = &(tuples[i]);
    const Tuple &tuple = tuple_set.get(i);
    bool flag = true;
    for (size_t j=0; j<split_tuple_sets.size(); j++) {
      TupleSet &tuple_set_ = split_tuple_sets[j];
      const Tuple &tuple_ = tuple_set_.get(0);
      if (same_tuple(tuple, tuple_, group_by_list) == true) {
        tuple_set_.add_(tuple);
        flag = false;
      }
    }
    if (flag) {
      TupleSet tuple_set_;
      tuple_set_.add_(tuple);
      split_tuple_sets.push_back(std::move(tuple_set_));
    }
  }
}

bool has_aggregation_select(const Selects &selects) {
  for (int i = selects.attr_num - 1; i >= 0; i--) {
    const RelAttr &attr = selects.attributes[i];
    char *real_attribute_name;
    AGGREGATION_TYPE aggreation = is_aggregation_select(attr.attribute_name, real_attribute_name);
    if (aggreation != NOT_KNOWN) {
      return true;
    }
  }
  return false;
}

const char *get_attribute_name_from_aggregation(const char *aggregation_str) {
  std::string str(aggregation_str);
  size_t pos = str.find('(');
  if (pos == std::string::npos) {
    return nullptr;
  }
  str = str.substr(pos, str.length()-pos-1);
  return str.c_str();
}

RC projection(const char *db, TupleSet &tuple_set, const Selects &selects, TupleSet &re_tuple_set) {
  re_tuple_set.clear();
  TupleSchema schema;
  const TupleSchema &schema_all = tuple_set.get_schema();
  std::map<std::string, bool> skip_flag;
  std::vector <size_t> indexs; // 需要取出的属性下标集合
  bool aggregation_flag = false;
  std::vector <AGGREGATION_TYPE> aggreations;
  if (has_aggregation_select(selects)) {
    aggregation_flag = true;
    for (int i = selects.attr_num - 1; i >= 0; i--) {
      // char *table_name = selects.relations[0];
      // Table * table = DefaultHandler::get_default().find_table(db, table_name);
      const RelAttr &attr = selects.attributes[i];
      // 多表中COUNT(ID)这种聚合语句应该报错
      if (attr.relation_name == nullptr && selects.relation_num != 1 && 0 != strcmp(attr.attribute_name, "COUNT(*)")) {
        return RC::GENERIC_ERROR;
      }
      Table * table = nullptr;
      if (attr.relation_name != nullptr) {
        table = DefaultHandler::get_default().find_table(db, attr.relation_name);
        if (table == nullptr) {
          return RC::GENERIC_ERROR;
        }
      } else {
        table = DefaultHandler::get_default().find_table(db, selects.relations[0]);
        if (table == nullptr) {
          return RC::GENERIC_ERROR;
        }
      }
      // if (table == nullptr && )
      char *real_attribute_name;
      AGGREGATION_TYPE aggreation = is_aggregation_select(attr.attribute_name, real_attribute_name);
      if (aggreation != NOT_KNOWN) {
        
        aggreations.push_back(aggreation);
        if (strcmp(real_attribute_name, "*") == 0) {
          indexs.push_back(-1);
          schema.add_if_not_exists(INTS, "*", attr.attribute_name);
        } else {
          const FieldMeta *field_meta = table->table_meta().field(real_attribute_name);
          if (nullptr == field_meta) {
            LOG_WARN("No such field. %s.%s", table->name(), real_attribute_name);
            return RC::SCHEMA_FIELD_MISSING;
          }
          indexs.push_back(schema_all.index_of_field(table->name(), real_attribute_name));
          schema.add_if_not_exists(field_meta->type(), table->name(), attr.attribute_name);
        }
      }
      
    }
  } else if (selects.relation_num == 1) {
    char *table_name = selects.relations[0];
    Table * table = DefaultHandler::get_default().find_table(db, table_name);
    for (int i = selects.attr_num - 1; i >= 0; i--) {
      const RelAttr &attr = selects.attributes[i];
      if (attr.relation_name != nullptr && 0 != strcmp(attr.relation_name, table_name)) {
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
      
      auto search = skip_flag.find(table_name);
      if (search != skip_flag.end() && search->second == true) {
        continue;
      }
      char *real_attribute_name;
      AGGREGATION_TYPE aggreation = is_aggregation_select(attr.attribute_name, real_attribute_name);
      if (aggreation != NOT_KNOWN) {
        aggregation_flag = true;
        aggreations.push_back(aggreation);
        if (strcmp(real_attribute_name, "*") == 0) {
          indexs.push_back(-1);
          schema.add_if_not_exists(INTS, "*", attr.attribute_name);
        } else {
          const FieldMeta *field_meta = table->table_meta().field(real_attribute_name);
          if (nullptr == field_meta) {
            LOG_WARN("No such field. %s.%s", table->name(), real_attribute_name);
            return RC::SCHEMA_FIELD_MISSING;
          }
          indexs.push_back(schema_all.index_of_field(table_name, real_attribute_name));
          schema.add_if_not_exists(field_meta->type(), table_name, attr.attribute_name);
        }
        
        continue;
      }
      if (strcmp(attr.attribute_name, "*") == 0) {
        skip_flag[table_name] = true;
        const TableMeta &table_meta = table->table_meta();
        for (int i=table_meta.sys_field_num(); i<table_meta.field_num(); i++) {
          const FieldMeta * field_meta = table_meta.field(i);
          indexs.push_back(schema_all.index_of_field(table->name(), field_meta->name()));
          schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name());
        }
      } else {
        const FieldMeta *field_meta = table->table_meta().field(attr.attribute_name);
        if (nullptr == field_meta) {
          LOG_WARN("No such field. %s.%s", table->name(), attr.attribute_name);
          return RC::SCHEMA_FIELD_MISSING;
        }
        indexs.push_back(schema_all.index_of_field(table_name, attr.attribute_name));
        schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name());
      }
      
    }
  } else  {
    for (int i = selects.attr_num - 1; i >= 0; i--) {
      const RelAttr &attr = selects.attributes[i];
      if (attr.relation_name == nullptr) {
        if (0 != strcmp(attr.attribute_name, "*")) {
          return RC::GENERIC_ERROR;
        }
        schema.clear();
        schema = tuple_set.get_schema();
        for (size_t i=0; i<schema.fields().size(); i++) {
          indexs.push_back(i);
        }
        // re_tuple_set.set_schema(tuple_set.get_schema());
        break;
      }
      Table * table = DefaultHandler::get_default().find_table(db, attr.relation_name);
      auto search = skip_flag.find(attr.relation_name);
      if (search != skip_flag.end() && search->second == true) {
        continue;
      }
      if (strcmp(attr.attribute_name, "*") == 0) {
        skip_flag[attr.relation_name] = true;
        const TableMeta &table_meta = table->table_meta();
        for (int i=0; i<table_meta.field_num()-table_meta.sys_field_num(); i++) {
          const FieldMeta * field_meta = table_meta.field(i);
          indexs.push_back(schema_all.index_of_field(table->name(), field_meta->name()));
          schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name());
        }
      } else {
        const FieldMeta *field_meta = table->table_meta().field(attr.attribute_name);
        if (nullptr == field_meta) {
          LOG_WARN("No such field. %s.%s", table->name(), attr.attribute_name);
          return RC::SCHEMA_FIELD_MISSING;
        }
        indexs.push_back(schema_all.index_of_field(attr.relation_name, attr.attribute_name));
        schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name());
      }
      
    }
  }
  if (aggregation_flag) {
    std::vector<int> group_by_list;
    std::vector<int> group_by_list_pos;
    for (int i=selects.condition_num-1; i>=0; i--) {
      const Condition &condition = selects.conditions[i];
      CompOp compop = condition.comp;
      if (compop == GROUP_BY) {
        const RelAttr &left_attr = condition.left_attr;
        int group_by_index = -1;
        if (left_attr.relation_name == nullptr) {
          if (selects.relation_num != 1) {
            return RC::SCHEMA_FIELD_MISSING;
          }
          group_by_index = schema_all.index_of_field(selects.relations[0], left_attr.attribute_name);
        } else {
          group_by_index = schema_all.index_of_field(left_attr.relation_name, left_attr.attribute_name);
        }
        if (group_by_index == -1) {
          return RC::SCHEMA_FIELD_MISSING;
        }
        group_by_list.push_back(group_by_index);
        int pos = -1;
        for (int j=selects.attr_num-1; j>=0; j--) {
          RelAttr attr = selects.attributes[j];
          if (attr.relation_name == nullptr && selects.relation_num != 1) {
            return RC::SCHEMA_FIELD_MISSING;
          }
          // char *table_name;
          Table * table = nullptr;
          if (attr.relation_name == nullptr) {
            table = DefaultHandler::get_default().find_table(db, selects.relations[0]);
          } else {
            table = DefaultHandler::get_default().find_table(db, attr.relation_name);
          }
          char *real_attribute_name;
          AGGREGATION_TYPE aggreation = is_aggregation_select(attr.attribute_name, real_attribute_name);
          if (aggreation != NOT_KNOWN) {
            continue;
          }
          const FieldMeta *field_meta = table->table_meta().field(attr.attribute_name);
          if (nullptr == field_meta) {
            LOG_WARN("No such field. %s.%s", table->name(), attr.attribute_name);
            return RC::SCHEMA_FIELD_MISSING;
          }
          if ((selects.relation_num == 1) 
            && (left_attr.relation_name == nullptr)
            && (attr.relation_name == nullptr) 
            && (strcmp(left_attr.attribute_name, attr.attribute_name) == 0)) {
            pos = selects.attr_num-1-j;
            break;
          } else if (((left_attr.relation_name != nullptr) 
            && (attr.relation_name != nullptr) 
            && (strcmp(left_attr.relation_name, attr.relation_name) == 0))
            && (strcmp(left_attr.attribute_name, attr.attribute_name) == 0)) {
            pos = selects.attr_num-1-j;
            break;
          }
        }
        group_by_list_pos.push_back(pos);
      }
    }
    std::vector<TupleSet> split_tuple_sets;
    split_tuple_set(tuple_set, group_by_list, split_tuple_sets);
    // std::ostringstream os;
    // tuple_set.print_tuple(os);
    // os << std::endl;
    // for (TupleSet &tuple_set_:split_tuple_sets) {
    //   tuple_set_.print_tuple(os);
    //   os << std::endl;
    // }

    // std::cout << os.str() << std::endl;
    // group_by_list_pos
    int schema_num = indexs.size();
    for (int i=0; i<group_by_list.size(); i++) {
      const TupleField &field = schema_all.field(group_by_list[i]);
      if (group_by_list_pos[i] != -1) {
        schema.add_if_not_exists(field.type(), field.table_name(), field.field_name());
        schema_num++;
      }
    }
    std::vector<int> schema_map;
    for (int i=0; i<schema_num; i++) {
      schema_map.push_back(-1);
    }
    int offset = indexs.size(); // 第一个非聚合项的列在schema中的位置
    for (int i=0; i<group_by_list_pos.size(); i++) {
      int pos = group_by_list_pos[i];
      if (pos != -1) {
        schema_map[pos] = offset++;
      }
    }
    offset = 0;
    for (int i=0; i<schema_num; i++) {
      if (schema_map[i] == -1) {
        schema_map[i] = offset++;
      }
    }
    TupleSchema schema_;
    for (int i=0; i<schema_num; i++) {
      int pos = schema_map[i];
      const TupleField &field = schema.field(pos);
      schema_.add(field.type(), field.table_name(), field.field_name());
    }

    re_tuple_set.set_schema(schema_);
    for (int i=0; i<split_tuple_sets.size(); i++) {
      Tuple tuple_, tuple__;
      for (size_t j=0; j<indexs.size(); j++) {
        int index = indexs[j];
        AGGREGATION_TYPE type = aggreations[j];
        get_tuple_value(split_tuple_sets[i], tuple_, type, index, group_by_list, j==indexs.size()-1);
      }
      for (int j=0; j<schema_num; j++) {
        int pos = schema_map[j];
        tuple__.add(tuple_.get_pointer(pos));
      }
      re_tuple_set.add(std::move(tuple__));
    }
    
    return RC::SUCCESS;
  }
  re_tuple_set.set_schema(schema);
  const std::vector<Tuple> &tuples = tuple_set.tuples();
  for (const Tuple &tuple: tuples) {
    Tuple tuple_;
    for (int index:indexs) {
      tuple_.add(tuple.get_pointer(index));
    }
    re_tuple_set.add(std::move(tuple_));
  }
  return RC::SUCCESS;
}

bool has_order_by(const Selects &selects) {
  for (size_t i = 0; i < selects.condition_num; i++) {
    const Condition &condition = selects.conditions[i];
    CompOp compop = condition.comp;

    if (compop == ORDER_BY_ASC || compop == ORDER_BY_DESC) {
      return true;
    } 

  }
  return false;

}

// 这里没有对输入的某些信息做合法性校验，比如查询的列名、where条件中的列名等，没有做必要的合法性校验
// 需要补充上这一部分. 校验部分也可以放在resolve，不过跟execution放一起也没有关系
RC ExecuteStage::do_select(const char *db, Query *sql, SessionEvent *session_event) {

  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();
  const Selects &selects = sql->sstr.selection;
  // 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
  std::vector<SelectExeNode *> select_nodes;
  std::unordered_map<std::string,int> table_map;
  for (size_t i = 0; i < selects.relation_num; i++) {
    const char *table_name = selects.relations[i];
    Table * table = DefaultHandler::get_default().find_table(db, table_name);
    if (nullptr == table) {
      LOG_WARN("No such table [%s] in db [%s]", table_name, db);
      end_trx_if_need(session, trx, false);
      char response[256];
      snprintf(response, sizeof(response), "%s\n", "FAILURE");
      session_event->set_response(response);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    SelectExeNode *select_node = new SelectExeNode;
    rc = create_selection_executor(trx, selects, db, table_name, *select_node);
    if (rc != RC::SUCCESS) {
      delete select_node;
      for (SelectExeNode *& tmp_node: select_nodes) {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      char response[256];
      snprintf(response, sizeof(response), "%s\n", "FAILURE");
      session_event->set_response(response);
      return rc;
    }
    select_nodes.push_back(select_node);
    table_map[table_name] = selects.relation_num - 1 - i;
  }
  if (table_map.empty()) {
    LOG_ERROR("No table given");
    end_trx_if_need(session, trx, false);
    return RC::SQL_SYNTAX;
  }

  std::vector<TupleSet> tuple_sets;
  for (std::vector<SelectExeNode *>::reverse_iterator iter=select_nodes.rbegin(); iter!=select_nodes.rend(); iter++) {
    SelectExeNode *&node = *iter;
    TupleSet tuple_set;
    rc = node->execute(tuple_set);
    if (rc != RC::SUCCESS) {
      for (SelectExeNode *& tmp_node: select_nodes) {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      char response[256];
      snprintf(response, sizeof(response), "%s\n", "FAILURE");
      session_event->set_response(response);
      return rc;
    } else {
      tuple_sets.push_back(std::move(tuple_set));
    }
  }

  std::stringstream ss;
  if (tuple_sets.size() > 1) {
    // 本次查询了多张表，需要做join操作
    TupleSet join_tuple_set;
    TupleSet re_tuple_set, re_tuple_set_;
    // dfs2(tuple_sets, join_tuple_set);
    JoinSelectExeNode join_select_node;
    rc = create_join_selection_executor(trx, selects, db, &tuple_sets, &table_map, join_select_node);
    join_select_node.execute(re_tuple_set);
    if (has_order_by(selects)) {
      rc = re_tuple_set.sort(selects);
      if (rc != RC::SUCCESS) {
        LOG_ERROR("sort error");
        char response[256];
        snprintf(response, sizeof(response), "%s\n", "FAILURE");
        session_event->set_response(response);
        return RC::GENERIC_ERROR;
      }
    }
    // re_tuple_set.sort(selects);
    rc = projection(db, re_tuple_set, selects, re_tuple_set_);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("projection error");
      // end_trx_if_need(session, trx, false);
      char response[256];
      snprintf(response, sizeof(response), "%s\n", "FAILURE");
      session_event->set_response(response);
      return RC::GENERIC_ERROR;
    }
    re_tuple_set_.print(ss, true);
  } else {
    // 当前只查询一张表，直接返回结果即可
    TupleSet re_tuple_set;
    TupleSet &tuple_set = tuple_sets.front();
    if (has_order_by(selects)) {
      rc = tuple_set.sort(selects);
      if (rc != RC::SUCCESS) {
        session_event->set_response(session_event->get_request_buf());
        return RC::GENERIC_ERROR;
        LOG_ERROR("sort error");
        char response[256];
        snprintf(response, sizeof(response), "%s\n", "FAILURE");
        session_event->set_response(response);
        return RC::GENERIC_ERROR;
      }
    }
    
    rc = projection(db, tuple_set, selects, re_tuple_set);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("projection error");
      char response[256];
      snprintf(response, sizeof(response), "%s\n", "FAILURE");
      session_event->set_response(response);
      return RC::GENERIC_ERROR;
    }
    re_tuple_set.print(ss, false);
  }

  for (SelectExeNode *& tmp_node: select_nodes) {
    delete tmp_node;
  }
  session_event->set_response(ss.str());
  end_trx_if_need(session, trx, true);
  return rc;
}

RC do_select_by_selects(const char *db, Trx *trx, const Selects &selects, TupleSet &re_tuple_set) {
  RC rc = RC::SUCCESS;
  // 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
  std::vector<SelectExeNode *> select_nodes;
  std::unordered_map<std::string,int> table_map;
  for (size_t i = 0; i < selects.relation_num; i++) {
    const char *table_name = selects.relations[i];
    Table * table = DefaultHandler::get_default().find_table(db, table_name);
    if (nullptr == table) {
      LOG_WARN("No such table [%s] in db [%s]", table_name, db);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    SelectExeNode *select_node = new SelectExeNode;
    rc = create_selection_executor(trx, selects, db, table_name, *select_node);
    if (rc != RC::SUCCESS) {
      delete select_node;
      for (SelectExeNode *& tmp_node: select_nodes) {
        delete tmp_node;
      }
      return rc;
    }
    select_nodes.push_back(select_node);
    table_map[table_name] = selects.relation_num - 1 - i;
  }
  if (table_map.empty()) {
    LOG_ERROR("No table given");
    return RC::SQL_SYNTAX;
  }

  std::vector<TupleSet> tuple_sets;
  for (std::vector<SelectExeNode *>::reverse_iterator iter=select_nodes.rbegin(); iter!=select_nodes.rend(); iter++) {
    SelectExeNode *&node = *iter;
    TupleSet tuple_set;
    rc = node->execute(tuple_set);
    if (rc != RC::SUCCESS) {
      for (SelectExeNode *& tmp_node: select_nodes) {
        delete tmp_node;
      }
      return rc;
    } else {
      tuple_sets.push_back(std::move(tuple_set));
    }
  }

  std::stringstream ss;
  if (tuple_sets.size() > 1) {
    // 本次查询了多张表，需要做join操作
    TupleSet join_tuple_set;
    JoinSelectExeNode join_select_node;
    rc = create_join_selection_executor(trx, selects, db, &tuple_sets, &table_map, join_select_node);
    join_select_node.execute(join_tuple_set);
    if (has_order_by(selects)) {
      rc = join_tuple_set.sort(selects);
      if (rc != RC::SUCCESS) {
        LOG_ERROR("sort error");
        return RC::GENERIC_ERROR;
      }
    }
    // re_tuple_set.sort(selects);
    rc = projection(db, join_tuple_set, selects, re_tuple_set);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("projection error");
      return RC::GENERIC_ERROR;
    }
    // re_tuple_set.print(ss, true);
  } else {
    // 当前只查询一张表，直接返回结果即可
    // TupleSet re_tuple_set;
    TupleSet &tuple_set = tuple_sets.front();
    if (has_order_by(selects)) {
      rc = tuple_set.sort(selects);
      if (rc != RC::SUCCESS) {    
        LOG_ERROR("sort error");
        return RC::GENERIC_ERROR;
      }
    }
    
    rc = projection(db, tuple_set, selects, re_tuple_set);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("projection error");
      return RC::GENERIC_ERROR;
    }
    // re_tuple_set.print(ss, false);
  }

  for (SelectExeNode *& tmp_node: select_nodes) {
    delete tmp_node;
  }
  return rc;
}

bool match_table(const Selects &selects, const char *table_name_in_condition, const char *table_name_to_match) {
  if (table_name_in_condition != nullptr) {
    return 0 == strcmp(table_name_in_condition, table_name_to_match);
  }

  return selects.relation_num == 1;
}

static RC schema_add_field(Table *table, const char *field_name, TupleSchema &schema) {
  const FieldMeta *field_meta = table->table_meta().field(field_name);
  if (nullptr == field_meta) {
    LOG_WARN("No such field. %s.%s", table->name(), field_name);
    return RC::SCHEMA_FIELD_MISSING;
  }

  schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name());
  return RC::SUCCESS;
}

bool has_no_sub_query(const Selects *selects){
  if (selects->condition_num == 0) {
    return true;
  }
  for (int i=0; i<selects->condition_num; i++) {
    if (selects->conditions[i].is_select) {
      return false;
    }
  }
  return true;
}

bool is_aggregation_schema_(const char *attribute_name, char *&aggregation_filed, char *&field_name) {
  
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

// RC select_condition_to_normal_condition(const char *db, Trx *trx, const Condition &select_condition, Condition &re_condition) {
//   Selects *selects = select_condition.selects;
//   Selects *selects_left = select_condition.selects_left;
//   TupleSet re_tuple_set;
//   RC rc;
//   if (selects_left != nullptr) {
//     Condition normal_condition_left;
//     Condition normal_condition_right;
//     if (has_no_sub_query(selects)) {

//     }
//   }
//   if (has_no_sub_query(selects)) {
//     if (selects_left != nullptr) {
//       if (selects->attr_num != 1 || selects_left->attr_num != 1) {
//         return RC::GENERIC_ERROR;
//       }
//       rc = do_select_by_selects(db, trx, *selects, re_tuple_set);
//       if (rc != RC::SUCCESS) {
//         return rc;
//       }
//       re_condition = select_condition;
//       re_condition.tuple_set_ = new TupleSet(std::move(re_tuple_set));
//       rc = do_select_by_selects(db, trx, *selects_left, re_tuple_set);
//       if (rc != RC::SUCCESS) {
//         return rc;
//       }
//       // re_condition = select_condition;
//       re_condition.tuple_set_left_ = new TupleSet(std::move(re_tuple_set));
//     } else {
//       if (selects->attr_num != 1) {
//         return RC::GENERIC_ERROR;
//       }
//       rc = do_select_by_selects(db, trx, *selects, re_tuple_set);
//       if (rc != RC::SUCCESS) {
//         return rc;
//       }
//       re_condition = select_condition;
//       re_condition.tuple_set_ = new TupleSet(std::move(re_tuple_set));
//       return RC::SUCCESS;
//     }
    
//     // re_condition
//   } else {
//     char *aggregation_filed, *field_name;
//     bool flag = is_aggregation_schema_(selects->attributes[0].attribute_name, aggregation_filed, field_name);
//     if ((!flag && select_condition.comp != IN) &&
//         (!flag && select_condition.comp != NOT_IN)) {
//       return RC::GENERIC_ERROR;
//     }
//     for (int i=0; i<selects->condition_num; i++) {
//       if (selects->conditions[i].is_select) {
//         Condition normal_condition;
//         RC rc = select_condition_to_normal_condition(db, trx, selects->conditions[i], normal_condition);
        
//         if (rc != RC::SUCCESS) {
//           return RC::GENERIC_ERROR;
//         }
//         selects->conditions[i] = normal_condition;
//       }
//     }
//     if (selects->attr_num != 1) {
//       return RC::GENERIC_ERROR;
//     }
//     rc = do_select_by_selects(db, trx, *selects, re_tuple_set);
//     if (rc != RC::SUCCESS) {
//       return rc;
//     }
//     re_condition = select_condition;
//     re_condition.tuple_set_ = new TupleSet(std::move(re_tuple_set));
//     return RC::SUCCESS;
//   }
// }

RC select_condition_to_normal_condition(const char *db, Trx *trx, const Condition &select_condition, Condition &re_condition) {
  Selects *selects = select_condition.selects;
  Selects *selects_left = select_condition.selects_left;
  TupleSet re_tuple_set;
  RC rc;
  if (has_no_sub_query(selects)) {
    if (selects_left != nullptr) {
      if (selects->attr_num != 1 || selects_left->attr_num != 1) {
        return RC::GENERIC_ERROR;
      }
      rc = do_select_by_selects(db, trx, *selects, re_tuple_set);
      if (rc != RC::SUCCESS) {
        return rc;
      }
      re_condition = select_condition;
      re_condition.tuple_set_ = new TupleSet(std::move(re_tuple_set));
      rc = do_select_by_selects(db, trx, *selects_left, re_tuple_set);
      if (rc != RC::SUCCESS) {
        return rc;
      }
      // re_condition = select_condition;
      re_condition.tuple_set_left_ = new TupleSet(std::move(re_tuple_set));
    } else {
      if (selects->attr_num != 1) {
        return RC::GENERIC_ERROR;
      }
      rc = do_select_by_selects(db, trx, *selects, re_tuple_set);
      if (rc != RC::SUCCESS) {
        return rc;
      }
      re_condition = select_condition;
      re_condition.tuple_set_ = new TupleSet(std::move(re_tuple_set));
      return RC::SUCCESS;
    }
    
    // re_condition
  } else {
    char *aggregation_filed, *field_name;
    bool flag = is_aggregation_schema_(selects->attributes[0].attribute_name, aggregation_filed, field_name);
    if ((!flag && select_condition.comp != IN) &&
        (!flag && select_condition.comp != NOT_IN)) {
      return RC::GENERIC_ERROR;
    }
    for (int i=0; i<selects->condition_num; i++) {
      if (selects->conditions[i].is_select) {
        Condition normal_condition;
        RC rc = select_condition_to_normal_condition(db, trx, selects->conditions[i], normal_condition);
        
        if (rc != RC::SUCCESS) {
          return RC::GENERIC_ERROR;
        }
        selects->conditions[i] = normal_condition;
      }
    }
    if (selects->attr_num != 1) {
      return RC::GENERIC_ERROR;
    }
    rc = do_select_by_selects(db, trx, *selects, re_tuple_set);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    re_condition = select_condition;
    re_condition.tuple_set_ = new TupleSet(std::move(re_tuple_set));
    return RC::SUCCESS;
  }
}

// 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node) {
  // 列出跟这张表关联的Attr
  TupleSchema schema;
  Table * table = DefaultHandler::get_default().find_table(db, table_name);
  if (nullptr == table) {
    LOG_WARN("No such table [%s] in db [%s]", table_name, db);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  TupleSchema::from_table(table, schema);

  // 找出仅与此表相关的过滤条件, 或者都是值的过滤条件
  std::vector<DefaultConditionFilter *> condition_filters;
  for (size_t i = 0; i < selects.condition_num; i++) {
    const Condition &condition = selects.conditions[i];
    if ((condition.left_is_attr == 0 && condition.right_is_attr == 0) || // 两边都是值
        (condition.left_is_attr == 1 && condition.right_is_attr == 0 && match_table(selects, condition.left_attr.relation_name, table_name)) ||  // 左边是属性右边是值
        (condition.left_is_attr == 0 && condition.right_is_attr == 1 && match_table(selects, condition.right_attr.relation_name, table_name)) ||  // 左边是值，右边是属性名
        (condition.left_is_attr == 1 && condition.right_is_attr == 1 &&
            match_table(selects, condition.left_attr.relation_name, table_name) && match_table(selects, condition.right_attr.relation_name, table_name)
            && condition.comp != ORDER_BY_ASC &&  condition.comp != ORDER_BY_DESC && condition.comp != GROUP_BY) || // 左右都是属性名，并且表名都符合
        (condition.is_select)
        ) {
      DefaultConditionFilter *condition_filter = new DefaultConditionFilter();
      if (condition.is_select && condition.selects_left != nullptr) {
        char *aggregation_filed, *field_name;
        // condition.selects
        bool flag1 = is_aggregation_schema_(condition.selects->attributes[0].attribute_name, aggregation_filed, field_name);
        bool flag2 = is_aggregation_schema_(condition.selects_left->attributes[0].attribute_name, aggregation_filed, field_name);
        // if ((!flag1 || !flag2) ||
        //     (condition.selects->attr_num != 1 || condition.selects_left->attr_num != 1)) {
        //       return RC::GENERIC_ERROR;
        // }
        Condition normal_condition;
        // Condition normal_condition_;
        RC rc = select_condition_to_normal_condition(db, trx, condition, normal_condition);
        if (rc != RC::SUCCESS) {
          for (DefaultConditionFilter * &filter : condition_filters) {
            delete filter;
          }
          return rc;
        }
        rc = condition_filter->init(*table, normal_condition);
        if (rc != RC::SUCCESS) {
          delete condition_filter;
          for (DefaultConditionFilter * &filter : condition_filters) {
            delete filter;
          }
          return rc;
        }
        condition_filters.push_back(condition_filter);
      } else if (condition.is_select) {
        char *aggregation_filed, *field_name;
        // condition.selects
        bool flag = is_aggregation_schema_(condition.selects->attributes[0].attribute_name, aggregation_filed, field_name);
        if ((!flag && condition.comp != IN) &&
            (!flag && condition.comp != NOT_IN)) {
          return RC::GENERIC_ERROR;
        }
        Condition normal_condition;
        RC rc = select_condition_to_normal_condition(db, trx, condition, normal_condition);
        if (rc != RC::SUCCESS) {
          for (DefaultConditionFilter * &filter : condition_filters) {
            delete filter;
          }
          return rc;
        }
        rc = condition_filter->init(*table, normal_condition);
        if (rc != RC::SUCCESS) {
          delete condition_filter;
          for (DefaultConditionFilter * &filter : condition_filters) {
            delete filter;
          }
          return rc;
        }
        condition_filters.push_back(condition_filter);
      } else {
        RC rc = condition_filter->init(*table, condition);
        if (rc != RC::SUCCESS) {
          delete condition_filter;
          for (DefaultConditionFilter * &filter : condition_filters) {
            delete filter;
          }
          return rc;
        }
        condition_filters.push_back(condition_filter);
      }
      
    }
  }

  return select_node.init(trx, table, std::move(schema), std::move(condition_filters));
}

bool same_table(char *left_table_name, char * right_table_name) {
  if (left_table_name != nullptr && right_table_name != nullptr) {
    return 0 == strcmp(left_table_name, right_table_name);
  }

  return false;
}

// 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
RC create_join_selection_executor(Trx *trx, const Selects &selects, const char *db, std::vector<TupleSet> *tuple_sets, std::unordered_map<std::string,int> *table_map, JoinSelectExeNode &join_select_node) {
  // 列出跟这张表关联的Attr
  TupleSchema schema;

  // 找出与两个表相关的过滤条件
  std::vector<DefaultConditionFilter *> condition_filters;
  for (size_t i = 0; i < selects.condition_num; i++) {
    const Condition &condition = selects.conditions[i];
    if (
        (condition.left_is_attr == 1 && condition.right_is_attr == 1 &&
            !same_table(condition.left_attr.relation_name, condition.right_attr.relation_name)) // 左右都是属性名，并且表名都符合
        ) {
      Table * table_left = DefaultHandler::get_default().find_table(db, condition.left_attr.relation_name);
      const TableMeta &table_meta_left = table_left->table_meta();
      Table * table_right = DefaultHandler::get_default().find_table(db, condition.right_attr.relation_name);
      const TableMeta &table_meta_right = table_right->table_meta();
      ConDesc left;
      ConDesc right;
      AttrType type_left = UNDEFINED;
      AttrType type_right = UNDEFINED;
      const FieldMeta *field_left = table_meta_left.field(condition.left_attr.attribute_name);
      if (nullptr == field_left) {
        LOG_WARN("No such field in condition. %s.%s", table_left->name(), condition.left_attr.attribute_name);
        return RC::SCHEMA_FIELD_MISSING;
      }
      left.attr_length = field_left->len();
      left.attr_offset = field_left->offset();
      left.is_attr = true;
      type_left = field_left->type();
      left.value = nullptr;
      left.table_name = new char[strlen(condition.left_attr.relation_name) + 1];
      strcpy(left.table_name, condition.left_attr.relation_name);
      left.attr_name = new char[strlen(condition.left_attr.attribute_name) + 1];
      strcpy(left.attr_name, condition.left_attr.attribute_name);
      
      const FieldMeta *field_right = table_meta_right.field(condition.right_attr.attribute_name);
      if (nullptr == field_right)
      {
        LOG_WARN("No such field in condition. %s.%s", table_right->name(), condition.right_attr.attribute_name);
        return RC::SCHEMA_FIELD_MISSING;
      }
      right.attr_length = field_right->len();
      right.attr_offset = field_right->offset();
      right.is_attr = true;
      type_right = field_right->type();
      right.value = nullptr;
      right.table_name = new char[strlen(condition.right_attr.relation_name) + 1];
      strcpy(right.table_name, condition.right_attr.relation_name);
      right.attr_name = new char[strlen(condition.right_attr.attribute_name) + 1];
      strcpy(right.attr_name, condition.right_attr.attribute_name);
      if (type_left != type_right)
      {
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }

      DefaultConditionFilter *condition_filter = new DefaultConditionFilter();
      

      RC rc = condition_filter->init(left, right, type_left, condition.comp, nullptr, nullptr);
      if (rc != RC::SUCCESS) {
        delete condition_filter;
        for (DefaultConditionFilter * &filter : condition_filters) {
          delete filter;
        }
        return rc;
      }
      condition_filters.push_back(condition_filter);
    }
  }

  return join_select_node.init(trx, tuple_sets, table_map, std::move(condition_filters));
}
