/**
* @file       TxPrepareBase.h
* @brief      交易json数据准备基础类
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

#ifndef RIPPLE_RPC_TX_PREPARE_BASE_H_INCLUDED
#define RIPPLE_RPC_TX_PREPARE_BASE_H_INCLUDED

#include <ripple/json/json_value.h>
#include <ripple/basics/base_uint.h>
#include <ripple/protocol/Protocol.h>
#include <ripple/protocol/AccountID.h>
#include <ripple/beast/utility/Journal.h>
#include <string>
#include <map>

namespace ripple {

	class Application;
	class Config;
	class SecretKey;

	using getCheckHashFunc = std::function<uint256(uint160)>;

	class TxPrepareBase
	{
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
		TxPrepareBase(Application& app, const std::string& secret, const std::string& publickey, Json::Value& tx_json, getCheckHashFunc func, bool ws);
		virtual ~TxPrepareBase();
		/// 准备最终需要的json数据
		virtual Json::Value prepare();	
		/**
		* 判断FutureTxHash是否正确
		*
		* @param tx_json json数据
		* @param app 当前运行的节点程序
		* @param bWs 是否为websocket
		* @retval 返回值包含error_message字段，则FutureTxHash不正确；否则，FutureTxHash正确。
		*/
        static Json::Value prepareFutureHash(const Json::Value& tx_json, Application& app, bool bWs);     //just check future hash is right.
		/**
		* 判断是否为加密模式
		* @return 返回是否为加密模式
		*/
		bool isConfidential();
	protected:
		/**
		*     @brief 获取交易的json数据
		*
		*     @retval null  没有错误
		*	  @retval !null 有错误，返回的error_message字段为错误信息
		*/
		Json::Value& getTxJson();
		/**
		*     @brief 准备基础数据
		*
		*     @retval null  没有错误
		*	  @retval !null 有错误，返回的error_message字段为错误信息
		*/
		Json::Value prepareBase();
		/**
		*     @brief 为T_RECREATE交易类型准备raw字段
		*
		*     @retval null  没有错误
		*	  @retval !null 有错误，返回的error_message字段为错误信息
		*/
        Json::Value prepareGetRaw();  
		/**
		*     @brief 检查是否为加密模式
		*
		*     @retval true  加密模式
		*	  @retval false 非加密模式
		*/
		bool checkConfidentialBase(const AccountID& owner, const std::string& tableName);
		//get decrypted pass_blob
		/**
		*     @brief 获取加密密码
		*
		*     @param ownerId 拥有者的accountID
		*     @param userId 操作者accountID
		*     @param secret_key 解密加密密码所使用的私钥
		*	  @return std::pair<Blob, Json::Value>(Blob, jsonValue)
		*	  jsonValue:null 顺利获取加密密码
		*	  jsonValue:!null 获取加密密码出错，错误信息见jsonValue的error_message字段
		*/
		std::pair<Blob, Json::Value> getPassBlobBase(AccountID& ownerId, AccountID& userId, boost::optional<SecretKey> secret_key);
		/// 准备校验hash字段
		Json::Value prepareCheckHash(const std::string& sRaw, const uint256& checkHash, uint256& checkHashNew);
		/// 解析表名
		Json::Value parseTableName();
	private:
		/**
		*     @brief 更新校验hash
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*	  @param checkHash 校验hash
		*/
		virtual void updateCheckHash(const std::string& sAccount, const std::string& sTableName, const uint256& checkHash) {};
		/**
		*     获取校验hash
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*     @return 返回校验hash
		*/
		virtual uint256 getCheckHashOld(const std::string& sAccount, const std::string& sTableName);

		/**
		*     @brief 获取针对raw字段进行加解密用的密码
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*	  @return 返回加密密码
		*/
		virtual Blob getPassblobExtra(const std::string& sAccount, const std::string& sTableName);
		/**
		*     @brief 更新针对raw字段进行加解密用的密码
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*	  @param passblob 针对raw字段进行加解密用的密码
		*/
		virtual void updatePassblob(const std::string& sAccount, const std::string& sTableName, const Blob& passblob) {};
		/**
		*     @brief 更新实际表名
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*	  @param sNameInDB 对应底层数据库的实际表名
		*/
        virtual void updateNameInDB(const std::string& sAccount, const std::string& sTableName, const std::string& sNameInDB) {};
		/**
		*     @brief 获取对应底层数据库的实际表名
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*/
		virtual std::string getNameInDB(const std::string& sAccount, const std::string& sTableName);

		/**
		*     @brief 检查是否为加密模式
		*
		*     @param owner 账户ID
		*     @param tableName 表名
		*/
		virtual bool checkConfidential(const AccountID& owner, const std::string& tableName);

		/**
		*     @brief 更新表信息
		*
		*     @param sAccount 账户ID
		*     @param sTableName 表名
		*     @param sNameInDB 对应底层数据库的实际表名
		*/
        void updateInfo(const std::string& sAccount, const std::string& sTableName, const std::string& sNameInDB);
		/**
		* @brief 准备“对应底层数据库的实际表名（nameInDB）”字段数据
		* @return
		*/
		Json::Value prepareDBName();        
		/**
		*	  @brief 使用基础json数据，生成最终的json数据
		*     @retval null  没有错误
		*	  @retval !null 有错误，返回的error_message字段为错误信息
		*/
		Json::Value prepareVL(Json::Value& json);

		/**
		* @brief 准备加密模式下json数据
		* @details 加密模式下:
		*	       T_CREATE:准备Raw和Token字段；
		*		   T_ASSERT/R_INSERT/R_UPDATE/R_DELETE：准备Raw字段；
		*		   T_GRANT/T_ASSIGN：准备Token字段
		* @retval null  没有错误
		* @retval !null 有错误，返回的error_message字段为错误信息
		*/
		Json::Value prepareRawEncode();
		/**
		* @brief 准备严格模式下的json数据
		* @details 严格模式下，更新TxCheckHash字段
		* @retval null  没有错误
		* @retval !null 有错误，返回的error_message字段为错误信息
		*/
		Json::Value prepareStrictMode();
		/**
		* @brief 准备PressData
		* @details 更新Flags字段
		* @retval null  没有错误
		* @retval !null 有错误，返回的error_message字段为错误信息
		*/
		void preparePressData();

		/**
		* @brief 准备加密模式下json数据
		* @details 加密模式下，T_ASSERT/R_INSERT/R_UPDATE/R_DELETE：准备Raw字段；
		* @retval null  没有错误
		* @retval !null 有错误，返回的error_message字段为错误信息
		*/
		Json::Value prepareForOperating();
		/**
		* @brief 准备加密模式下json数据
		* @details 加密模式下，T_CREATE:准备Raw和Token字段
		* @retval null  没有错误
		* @retval !null 有错误，返回的error_message字段为错误信息
		*/
		Json::Value prepareForCreate();
		/**
		* @brief 准备加密模式下json数据
		* @details 加密模式下，T_GRANT/T_ASSIGN：准备Token字段
		* @retval null  没有错误
		* @retval !null 有错误，返回的error_message字段为错误信息
		*/
		Json::Value prepareForAssign();

		//get decrypted pass_blob
		/**
		*     @brief 获取校验hash
		*
		*     @param sAccount 账户ID
		*	  @param sTableName 表名
		*	  @return std::pair<uint256, Json::Value>(checkHash, jsonValue)
		*	  jsonValue:null 顺利获取校验hash
		*	  jsonValue:!null 获取校验hash出错，错误信息见jsonValue的error_message字段
		*/
		std::pair<uint256,Json::Value> getCheckHash(const std::string& sAccount,const std::string& sTableName);

		/**
		*     @brief 获取加密密码
		*
		*     @param ownerId 拥有者的accountID
		*     @param userId 操作者accountID
		*     @param secret_key 解密加密密码所使用的私钥
		*	  @return std::pair<Blob, Json::Value>(Blob, jsonValue)
		*	  jsonValue:null 顺利获取加密密码
		*	  jsonValue:!null 获取加密密码出错，错误信息见jsonValue的error_message字段
		*/
		std::pair<Blob, Json::Value> getPassBlob(
			AccountID& ownerId, AccountID& userId, boost::optional<SecretKey> secret_key);
	protected:
		Application&										app_;
		const std::string&									secret_;
        const std::string&                                  public_;
		Json::Value&										tx_json_;
		getCheckHashFunc									getCheckHashFunc_;
		bool												ws_;

	private:
		std::string                                         sTableName_;
		uint160                                             u160NameInDB_;		
		AccountID                                           ownerID_;

		//is table raw encoded
		bool												m_bConfidential;
	};
} // ripple

#endif
