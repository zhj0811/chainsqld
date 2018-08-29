/**
* @file       TableAssistant.h
* @brief      表助手类
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

#ifndef RIPPLE_RPC_TABLE_ASSISTANT_H_INCLUDED
#define RIPPLE_RPC_TABLE_ASSISTANT_H_INCLUDED

#include <ripple/json/json_value.h>
#include <ripple/basics/base_uint.h>
#include <ripple/protocol/Protocol.h>
#include <ripple/protocol/AccountID.h>
#include <ripple/beast/utility/Journal.h>
#include <utility>
#include <string>
#include <map>
#include <list>
#include <chrono>

namespace ripple {

	class Application;
	class Config;
	class SecretKey;
	class STTx;

	using clock_type = std::chrono::steady_clock;
	using time_point_type = clock_type::time_point;

	class TableAssistant
	{
		/**
		* 交易信息
		*/
		typedef struct
		{
			LedgerIndex                                                  uTxLedgerVersion;		///< 最后可以加入的区块高度
			uint256                                                      uTxHash;				///< 交易hash
			uint256                                                      uTxCheckHash;			///< 校验hash
			bool                                                         bStrictMode;			///< 是否为严格模式
		}txInfo;
		/**
		* 校验信息
		*/
		typedef struct
		{
			uint256                                                      uTxCheckHash;			///< 校验hash
			uint256                                                      uTxBackupHash;			///< 备份校验hash，和raw字段一起创建新的校验hash
			std::string													 sTableName;			///< 
			AccountID													 accountID;
			time_point_type												 timer;
			std::list<std::shared_ptr<txInfo>>							 listTx;
		}checkInfo;

	public:
		TableAssistant(Application& app, Config& cfg, beast::Journal journal);
		~TableAssistant() {}

		//
		//	t_create:generate token & crypt raw
		//	t_assign:generate token
		//	r_insert&r_delete&r_update:crypt raw
		/**
		*     准备普通交易/事务所需的json数据
		*
		*		t_create:生成Token（对Raw字段加解密密码使用用户公钥加密后的密文），加密raw字段
		*
		*		r_insert&r_delete&r_update:加密raw字段
		*
		*     @param secret 私钥
		*     @param publickey 公钥
		*	  @param tx_json input/output json数据
		*	  @param ws 是否为websocket
		*     @return 有错误时，返回的error_message或error
		*/
		Json::Value prepare(const std::string& secret,const std::string& publickey,Json::Value& tx_json,bool ws = false);
		/**
		*     获取对应底层数据库的实际表名
		*
		*     @param accountIdStr 账户ID
		*     @param tableNameStr 表名
		*/
		Json::Value getDBName(const std::string& accountIdStr, const std::string& tableNameStr);
		/// 存储普通交易/事务中各个交易的校验信息到map中
		bool Put(STTx const& tx);
		/// 开启表校验hash线程
		void TryTableCheckHash();

		/// 获取表校验hash
		uint256 getCheckHash(uint160 nameInDB);
	private:
		/// 存储交易的校验信息到map中
		bool PutOne(STTx const& tx, const uint256 &uHash);
		/**
		* 表校验hash线程
		*
		*	将已进入验证区块的交易，从m_map的校验信息中的交易信息列表中移除
		*/
		void TableCheckHashThread();
        Config& GetConfig() {return cfg_;}

	private:
		Application&										app_;
		beast::Journal										journal_;
		Config&												cfg_;

		//key:nameInDB
		std::map<uint160, std::shared_ptr<checkInfo>>		m_map;

		std::mutex											mutexMap_;
		bool												bTableCheckHashThread_;

	};
} // ripple

#endif
