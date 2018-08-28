/**

* @file       ECDSACanonical.h

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

#ifndef RIPPLE_CRYPTO_ECDSACANONICAL_H_INCLUDED
#define RIPPLE_CRYPTO_ECDSACANONICAL_H_INCLUDED

#include <ripple/basics/Blob.h>

namespace ripple {

enum class ECDSA
{
    not_strict = 0,
    strict
};

/** isCanonicalECDSASig
*     Checks whether a secp256k1 ECDSA signature is canonical.
    Return value is true if the signature is canonical.
    If mustBeStrict is specified, the signature must be
    strictly canonical (one and only one valid form).
    The return value for something that is not an ECDSA
    signature is unspecified. (But the function will not crash.)

*     @param signature
*     @param sigLen
*     @param mustBeStrict
*     @return true/false
*/
bool isCanonicalECDSASig (void const* signature,
    std::size_t sigLen, ECDSA mustBeStrict);

inline bool isCanonicalECDSASig (Blob const& signature,
    ECDSA mustBeStrict)
{
    return signature.empty() ? false :
        isCanonicalECDSASig (&signature[0], signature.size(), mustBeStrict);
}

/** isCanonicalECDSASig
*     Converts a canonical secp256k1 ECDSA signature to a
    fully-canonical one. Returns true if the original signature
    was already fully-canonical. The behavior if something
    that is not a canonical secp256k1 ECDSA signature is
    passed is unspecified. The signature buffer must be large
    enough to accommodate the largest valid fully-canonical
    secp256k1 ECDSA signature (72 bytes).

*     @param signature
*     @param sigLen
*     @return true/false
*/
bool makeCanonicalECDSASig (void *signature, std::size_t& sigLen);

}

#endif
