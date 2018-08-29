/**
* @file       TxSingleTransPrepare.h
* @brief      事务交易中单个交易信息json准备类
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

#ifndef RIPPLE_RPC_TX_SINGLE_TRANS_PREPARE_H_INCLUDED
#define RIPPLE_RPC_TX_SINGLE_TRANS_PREPARE_H_INCLUDED

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
	class SecretKey;
	class TxTransactionPrepare;

	class TxSingleTransPrepare : public TxPrepareBase
	{
	public:
		/**
		* 构造函数
		* @param app 当前运行节点程序
		* @param trans 事务交易信息准备
		* @param secret 私钥
		* @param publickey 公钥
		* @param tx_json 引用类型的json数据
		* @param func 获取校验hash回调函数 
		* @param ws 是否为websocket
		*/
		TxSingleTransPrepare(Application& app, TxTransactionPrepare* trans, 
			const std::string& secret, const std::string& publickey, Json::Value& tx_json, getCheckHashFunc func,bool ws);

	private:
		/**
		*     @brief 检查是否为加密模式
		*
		*     @param owner 账户ID
		*     @param tableName 表名
		*/
		virtual bool checkConfidential(const AccountID& owner, const std::string& tableName) override;
		/**
		*     获取校验hash
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*     @return 返回校验hash
		*/
		virtual uint256 getCheckHashOld(const std::string& sAccount, const std::string& sTableName) override;

		/**
		*     @brief 更新校验hash
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*	  @param checkHash 校验hash
		*/
		virtual void updateCheckHash(const std::string& sAccount, const std::string& sTableName,const uint256& checkHash) override;

		/**
		*     @brief 获取针对raw字段进行加解密用的密码
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*	  @return 返回加密密码
		*/
		virtual Blob getPassblobExtra(const std::string& sAccount, const std::string& sTableName) override;

		/**
		*     @brief 更新针对raw字段进行加解密用的密码
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*	  @param passblob 针对raw字段进行加解密用的密码
		*/
		virtual void updatePassblob(const std::string& sAccount, const std::string& sTableName, const Blob& passblob) override;

		/**
		*     @brief 更新实际表名
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*	  @param sNameInDB 对应底层数据库的实际表名
		*/
		void updateNameInDB(const std::string& sAccount, const std::string& sTableName, const std::string& sNameInDB) override;
		/**
		*     @brief 获取对应底层数据库的实际表名
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*/
		virtual std::string getNameInDB(const std::string& sAccount, const std::string& sTableName) override;

	private:
		TxTransactionPrepare*								m_pTransaction;
	};
} // ripple

#endif
