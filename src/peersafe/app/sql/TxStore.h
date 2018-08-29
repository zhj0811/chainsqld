/**

* @file       TxStore.h

* @brief      将 chainSQL 的交易转换成 native SQL.

* @details	此类会根据 Raw 协议生成 native SQL,并且执行 SQL 语句

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

#ifndef RIPPLE_APP_MISC_TXSTORE_H_INCLUDED
#define RIPPLE_APP_MISC_TXSTORE_H_INCLUDED

#include <memory>
#include <string>
#include <utility>

#include <ripple/app/tx/impl/ApplyContext.h>
#include <ripple/core/DatabaseCon.h>
#include <ripple/rpc/Context.h>
#include <ripple/json/json_value.h>
#include <ripple/basics/Log.h>

namespace ripple {

class TxStoreDBConn {
public:
	TxStoreDBConn(const Config& cfg);
	~TxStoreDBConn();

	DatabaseCon* GetDBConn() {
		if (databasecon_)
			return databasecon_.get();
		return nullptr;
	}

private:
	std::shared_ptr<DatabaseCon> databasecon_;
};

class TxStoreTransaction {
public:
	TxStoreTransaction(TxStoreDBConn* storeDBConn);
	~TxStoreTransaction();

	soci::transaction* GetTransaction() {
		if (tr_)
			return tr_.get();
		return nullptr;
	}

	void commit() {
		tr_->commit();
	}

    void rollback() {
        tr_->rollback();
    }

private:
	std::shared_ptr<soci::transaction> tr_;
    std::shared_ptr<LockedSociSession> lockSession_;
};

/**

* 转换 chainSQL 交易为 native SQL.

* 将 chainSQL 交易转换为 native SQL 语句，并且执行 native SQL。这个操作会更改 chainSQL 后端数据库实体数据

*/
class TxStore {
public:
	//TxStore(const Config& cfg);

	/** TxStore 构造函数
	*
	*    根据数据库连接对象、配置等构造 txStore 对象
	*	@param dbconn 数据连接对象
	*	@param cfg 数据库连接配置
	*	@param journal 日志对象
	*	@return void
	*/
	TxStore(DatabaseCon* dbconn, const Config& cfg, const beast::Journal& journal);
	~TxStore();

	/** 处理 chainSQL 交易
	*
	*    根据 chainSQL 交易的类型生成相关的 native SQL,并且执行 SQL
	*	@param tx chainSQL 交易对象
	*	@param operationRule  行级控制规则
	*	@param verifyAffectedRows  这个交易是否会影响到行
	*	@return std::pair 的第一个元素为 0 表示处置交易是否成功，否则处置交易失败；第二个元素为失败后的错误提示
	*/
	std::pair<bool,std::string> Dispose(const STTx& tx,const std::string& operationRule = "", bool verifyAffectedRows = false);

	/** 从数据库里直接删除指定的表
	*
	*    略
	*	@param tablename 表名
	*	@return std::pair 的第一个元素为 0 表示处置交易是否成功，否则处置交易失败；第二个元素为失败后的错误提示
	*/
	std::pair<bool, std::string> DropTable(const std::string& tablename);

	/** 从数据库里直接查询数据
	*
	*    更加查询对象直接从数据库里查询数据
	*	@param context tx context 查询对象
	*	@return Json::Value
	*/
	Json::Value txHistory(RPC::Context& context);
    Json::Value txHistory(Json::Value& tx_json);
    Json::Value txHistory(std::string sql);

	DatabaseCon* getDatabaseCon();
private:
	const Config& cfg_;
	std::string db_type_;
	DatabaseCon* databasecon_;
	beast::Journal journal_;
};	// class TxStore

}	// namespace ripple
#endif // RIPPLE_APP_MISC_TXSTORE_H_INCLUDED
