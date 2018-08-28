/**

* @file       ECDSAKey.h

* @brief      AES.

* @details	AES

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


#ifndef RIPPLE_ECDSAKEY_H
#define RIPPLE_ECDSAKEY_H

#include <ripple/basics/base_uint.h>
#include <ripple/basics/Blob.h>
#include <ripple/crypto/impl/ec_key.h>

namespace ripple {

openssl::ec_key ECDSAPrivateKey (uint256 const& serialized);
openssl::ec_key ECDSAPublicKey  (Blob    const& serialized);

openssl::ec_key ECDSAPublicKey (std::uint8_t const* data, std::size_t size);

} // ripple

#endif
