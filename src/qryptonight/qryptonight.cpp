/*
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * Additional permission under GNU GPL version 3 section 7
  *
  * If you modify this Program, or any covered work, by linking or combining
  * it with OpenSSL (or a modified version of that library), containing parts
  * covered by the terms of OpenSSL License and SSLeay License, the licensors
  * of this Program grant you additional permission to convey the resulting work.
  *
  */

#include <xmrstak/backend/cpu/crypto/cryptonight.h>
#include <xmrstak/backend/cpu/crypto/cryptonight_aesni.h>
#include <iostream>
#include "qryptonight.h"

Qryptonight::Qryptonight()
{
    size_t init_res;

    // First try fast mem
    init_res = cryptonight_init(1, 1, &_last_msg);

    if (init_res)
    {
        // get context
        _context = cryptonight_alloc_ctx(1, 1, &_last_msg);
        if (_context!= nullptr)
            return;
    }

    // If something failed.. go for basic settings
    init_res = cryptonight_init(0, 1, &_last_msg);
    _context = cryptonight_alloc_ctx(0, 1, &_last_msg);
}

Qryptonight::~Qryptonight()
{
    if (_context!= nullptr)
    {
        cryptonight_free_ctx(_context);
    }
}

std::vector<uint8_t> Qryptonight::hash(std::vector<uint8_t> input)
{
    std::vector<uint8_t> output(32);

    cryptonight_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, false, false>(input.data(), input.size(),
                                                                            output.data(),
                                                                            _context);

    return output;
};
