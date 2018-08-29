/**
* @file       TxTransactionPrepare.h
* @brief      事务交易信息json准备类
* @details	  为事务中每一个单独的交易准备json
* @date       20171209
* @version	  V1.0
* @par Copyright (c):
*      2016-2018 Peersafe Technology Co., Ltd.
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

#ifndef RIPPLE_RPC_TX_TRANSACTION_PREPARE_H_INCLUDED
#define RIPPLE_RPC_TX_TRANSACTION_PREPARE_H_INCLUDED

#include <ripple/json/json_value.h>
#include <ripple/basics/base_uint.h>
#include <ripple/protocol/Protocol.h>
#include <ripple/protocol/AccountID.h>
#include <ripple/beast/utility/Journal.h>
#include <peersafe/rpc/impl/TxPrepareBase.h>
#include <string>
#include <map>

namespace ripple {

	class Application;
	class TxPrepareBase;

	class TxTransactionPrepare
	{
		//transaction related
		/**
		*	transInfo：交易涉及的信息
		*/
		typedef struct
		{
			uint256														 uTxCheckHash;	///<检验hash
			Blob														 pass;			///<加密密码
			std::string													 sNameInDB;		///<对应底层数据库的实际表名
		}transInfo;

	public:
		/**
		*     @brief 构造函数
		*
		*     @param app 当前运行的节点程序
		*     @param secret 私钥
		*     @param publickey 公钥
		*	  @param tx_json input/output json数据
		*	  @param func 获取校验hash回调函数 
		*     @param ws 是否为websocket
		*/
		TxTransactionPrepare(Application& app, const std::string& secret,const std::string& publickey, Json::Value& tx_json, getCheckHashFunc func,bool ws);
		virtual ~TxTransactionPrepare();

		/**
		*     @brief 准备事务中每一个交易json数据
		*
		*     @retval null  顺利准备数据
		*	  @retval !null 有错误，返回的error_message字段为错误信息
		*/
		Json::Value prepare();

		/**
		*     @brief updateNameInDB：更新实际表名
		*
		*     @param accountId 账户ID
		*     @param tableName 表名
		*	  @param sNameInDB 对应底层数据库的实际表名
		*/
		void updateNameInDB(const std::string& accountId,const std::string& tableName,const std::string& sNameInDB);
		/**
		*     @brief 获取对应底层数据库的实际表名
		*
		*     @param accountId 账户ID
		*     @param tableName 表名
		*     @return 返回对应底层数据库的实际表名
		*/
		std::string getNameInDB(const std::string& accountId, const std::string& tableName);

		/**
		*     @brief 更新校验hash
		*
		*     @param accountId 账户ID
		*     @param tableName 表名
		*	  @param checkHash 校验hash
		*/
		void updateCheckHash(const std::string& accountId, const std::string& tableName, const uint256& checkHash);
		/**
		*     @brief 获取校验hash
		*
		*     @param accountId 账户ID
		*     @param tableName 表名
		*     @return 本函数返回执行结果，返回校验hash
		*/
		uint256 getCheckHash(const std::string& accountId, const std::string& tableName);

		/**
		*     @brief 更新针对raw字段进行加解密用的密码
		*
		*     @param accountId 账户ID
		*     @param tableName 表名
		*	  @param pass 针对raw字段进行加解密用的密码
		*/
		void updatePassblob(const std::string& accountId, const std::string& tableName, const Blob& pass);
		/**
		*     @brief 获取针对raw字段进行加解密用的密码
		*
		*     @param accountId 账户ID
		*     @param tableName 表名
		*	  @return 返回加密密码
		*/
		Blob getPassblob(const std::string& accountId, const std::string& tableName);

	private:
		//key: accountId,value:map
		//key: table_name,value:transInfo
		std::shared_ptr<std::map<std::string,std::map<std::string, transInfo>>>	m_pMap;

		Application&										app_;
		const std::string&									secret_;
        const std::string&                                  public_;
		Json::Value&										tx_json_;
		getCheckHashFunc									getCheckHashFunc_;
		bool												ws_;
	};
} // ripple

#endif
