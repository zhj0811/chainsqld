/**

* @file       TableStatusDB.h

* @brief      数据库表状态基类.

* @details	 数据库表状态基类

* @author     peersafe

* @date       2017/12/09

* @version v1.0

* @par Copyright (c):

*      Copyright (c) 2016-2018 Peersafe Technology Co., Ltd.

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

#ifndef RIPPLE_APP_TABLE_TABLESTATUSDB_H_INCLUDED
#define RIPPLE_APP_TABLE_TABLESTATUSDB_H_INCLUDED

#include <ripple/app/main/Application.h>
#include <ripple/core/DatabaseCon.h>
#include <ripple/protocol/AccountID.h>
#include <ripple/protocol/Protocol.h>

namespace ripple {
	/**
	*
	* 数据操作返回值
	*
	* 忽略
	*/
	enum soci_ret {
		soci_success = 1,		///< 数据操作成功
		soci_failed = 0,		///< 数据操作失败
		soci_exception = -1		///< 数据操作异常
	};

/**

* 数据库表状态基类态

* 数据库表状态基类

*/
class TableStatusDB {
public:
    TableStatusDB(DatabaseCon* dbconn, Application*  app,beast::Journal& journal);
    virtual ~TableStatusDB();

    virtual bool InitDB(DatabaseCon::Setup setup) = 0;

    virtual bool GetAutoListFromDB(uint256 chainId, std::list<std::tuple<std::string, std::string, std::string, bool> > &list) = 0;

    virtual bool ReadSyncDB(std::string nameInDB, LedgerIndex &txnseq,
        uint256 &txnhash, LedgerIndex &seq, uint256 &hash, uint256 &TxnUpdateHash) = 0;

    virtual bool GetMaxTxnInfo(std::string TableName, std::string Owner, LedgerIndex &TxnLedgerSeq, uint256 &TxnLedgerHash) = 0;

    virtual bool InsertSnycDB(std::string TableName, std::string TableNameInDB, std::string Owner,LedgerIndex LedgerSeq, uint256 LedgerHash, bool IsAutoSync,std::string TxnLedgerTime, uint256 chainId) = 0;

    virtual bool CreateSnycDB(DatabaseCon::Setup setup) = 0;

    virtual bool isNameInDBExist(std::string TableName, std::string Owner, bool delCheck, std::string &TableNameInDB) = 0;

	/**  更新表名
	*
	*    根据表 id 更新某账户下的表名
	*	@param accountID 账户 id
	*	@param TableNameInDB 表 id
	*	@param TableName 表名
	*	@return true 更新成功，否则更新除失败
	*/
    virtual bool RenameRecord(AccountID accountID, std::string TableNameInDB, std::string TableName) = 0;

	/**  更新表 id
	*
	*    更新某账号下已有表的id
	*	@param accountID 账户 id
	*	@param tablename 表名
	*	@param TableNameInDB 表 id
	*	@return true 更新成功，否则更新除失败
	*/
    virtual soci_ret UpdateSyncDB(AccountID accountID, std::string TableName, std::string TableNameInDB) = 0;

	/**  更新表状态
	*
	*    更新某账户下的表状态
	*	@param owner 表所有者账号
	*	@param tablename 表名
	*	@param isAutoSync 是否自动同步
	*	@return true 更新成功，否则更新除失败
	*/
    virtual bool UpdateStateDB(const std::string & owner, const std::string & tablename, const bool &isAutoSync) = 0;
    
	/**  删除表记录
	*
	*    删除某账户下的表数据
	*	@param accountID 账户id
	*	@param TableName 表名
	*	@return true 删除成功，否则删除失败
	*/
    virtual bool DeleteRecord(AccountID accountID, std::string TableName) =0;

	/**  判断表是否存在
	*
	*    判断某账号下是否拥有某表
	*	@param accountID 账户id
	*	@param TableNameInDB 表id
	*	@return true 表存在，否则表不存在
	*/
    virtual bool IsExist(AccountID accountID, std::string TableNameInDB) = 0;

    virtual soci_ret UpdateSyncDB(const std::string &Owner, const std::string &TableNameInDB,
        const std::string &TxnLedgerHash, const std::string &TxnLedgerSeq, const std::string &LedgerHash,
        const std::string &LedgerSeq, const std::string &TxHash, const std::string &TxnLedgerTime, const std::string &PreviousCommit) = 0;

    virtual soci_ret UpdateSyncDB(const std::string &Owner, const std::string &TableNameInDB,
            const std::string &LedgerHash, const std::string &LedgerSeq, const std::string &PreviousCommit) = 0;

    virtual soci_ret UpdateSyncDB(const std::string &Owner, const std::string &TableNameInDB,
        const std::string &TxnUpdateHash, const std::string &PreviousCommit) = 0;

    virtual soci_ret UpdateSyncDB(const std::string &Owner, const std::string &TableNameInDB,
        bool bDel, const std::string &PreviousCommit) = 0;

protected:
    DatabaseCon*                                                 databasecon_;
    Application*                                                 app_;
    beast::Journal&                                              journal_;
}; // class TxStoreStatus

}

#endif
