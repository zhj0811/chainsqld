/**

* @file       STTx2SQL.h

* @brief      将 chainSQL 的交易转换成 native SQL.

* @details	真正实现将 chainSQL 交易转换成 native SQL 并且执行 SQL 的类

* @author     dbliu

* @date       2017/12/09

* @version v1.0

* @par Copyright (c):

*      Copyright (c) 2016-2018 Peersafe Technology Co., Ltd

* @par History:

*   v1.0: dbliu, 2017/12/09, originator\n

*/
//------------------------------------------------------------------------------
/*
 This file is part of chainsqld: https://github.com/chainsql/chainsqld
 Copyright (c) 2016-2018 Peersafe Technology Co., Ltd.
 
	chainsqld is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
 
	chainsqld is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
 */
//==============================================================================

#ifndef RIPPLE_APP_MISC_JSON2SQL_H_INCLUDED
#define RIPPLE_APP_MISC_JSON2SQL_H_INCLUDED

#include <memory>
#include <string>
#include <utility>

#include <ripple/json/json_value.h>
#include <ripple/json/Object.h>
#include <ripple/json/json_writer.h>
#include <ripple/protocol/STTx.h>
#include <ripple/core/DatabaseCon.h>

namespace ripple {

class BuildSQL;
class DatabaseCon;

/**

* 转换 chainSQL 交易为 native SQL.

* 真正将将 chainSQL 交易转换为 native SQL 语句，并且执行 native SQL。这个操作会更改 chainSQL 后端数据库实体数据

*/
class STTx2SQL {
public:
	STTx2SQL(const std::string& db_type);
	STTx2SQL(const std::string& db_type, DatabaseCon* dbconn);
	~STTx2SQL();

    static bool IsTableExistBySelect(DatabaseCon* dbconn, std::string sTable);

	using MapRule = std::map<std::string, Json::Value>;
	/** 处理 chainSQL 交易
	*
	*    根据 chainSQL 交易的类型生成相关的 native SQL,并且执行 SQL
	*	@param tx chainSQL 交易对象
	*	@param operationRule  行级控制规则
	*	@param verifyAffectedRows  这个交易是否会影响到行
	*	@return std::pair 的第一个元素为 0 表示处置交易是否成功，否则处置交易失败；第二个元素成功时为 native SQL 语句，否则为失败信息
	*/
	std::pair<int /*retcode*/, std::string /*sql*/> ExecuteSQL(const ripple::STTx& tx, 
		const std::string& operationRule = "",
		bool verifyAffectedRows = false);

private:
	STTx2SQL() {};
	int GenerateCreateTableSql(const Json::Value& raw, BuildSQL *buildsql);
	//int GenerateRenameTableSql(const Json::Value& tx_json, std::string& sql);
	int GenerateInsertSql(const Json::Value& raw, BuildSQL *buildsql);
	int GenerateUpdateSql(const Json::Value& raw, BuildSQL *buildsql);
	int GenerateDeleteSql(const Json::Value& raw, BuildSQL *buildsql);
	int GenerateSelectSql(const Json::Value& raw, BuildSQL *buildsql);

	std::pair<bool, std::string> handle_assert_statement(const Json::Value& raw, BuildSQL *buildsql);
	bool assert_result(const soci::rowset<soci::row>& records, const Json::Value& expect);

	bool check_raw(const Json::Value& raw, const uint16_t optype);
	std::pair<bool, std::string> check_optionalRule(const std::string& optionalRule);

	std::string db_type_;
	DatabaseCon* db_conn_;
}; // STTx2SQL

}
#endif // RIPPLE_APP_MISC_JSON2SQL_H_INCLUDED
