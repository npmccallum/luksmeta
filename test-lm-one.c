/* vim: set tabstop=8 shiftwidth=4 softtabstop=4 expandtab smarttab colorcolumn=80: */
/*
 * Copyright (c) 2016 Red Hat, Inc.
 * Author: Nathaniel McCallum <npmccallum@redhat.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "test.h"
#include <error.h>
#include <stdlib.h>
#include <string.h>

static const uint8_t UUID[] = {
    0xb0, 0xcc, 0xe0, 0x48, 0x89, 0x32, 0x90, 0x7e, 0x10, 0xd9, 0x62, 0xa2,
    0x09, 0x3c, 0x05, 0x7d, 0x86, 0x17, 0x03, 0xf2, 0x69, 0xe2, 0x11, 0x2b,
    0x57, 0xf1, 0x8c, 0x31, 0x41, 0x96, 0x6f, 0x9d
};

int
main(int argc, char *argv[])
{
    uint8_t uuid[sizeof(UUID)] = {};
    uint8_t data[sizeof(UUID)] = {};
    struct crypt_device *cd = NULL;
    uint32_t offset = 0;
    uint32_t length = 0;
    int r;

    crypt_free(test_format());
    cd = test_init();
    test_hole(cd, &offset, &length);

    r = luksmeta_set(cd, CRYPT_ANY_SLOT, UUID, UUID, sizeof(UUID));
    if (r < 0)
        error(EXIT_FAILURE, -r, "%s:%d", __FILE__, __LINE__);

    /* Test the layout state. */
    assert(test_layout((range_t[]) {
        { 0, 1024 },                   /* LUKS header */
        { 1024, offset - 1024, true }, /* Keyslot Area */
        { offset, 4096 },              /* luksmeta header */
        { offset + 4096, 4096 },       /* luksmeta slot 0 */
        END(offset + 8192),            /* Rest of the file */
    }));

    assert(luksmeta_get(cd, r, uuid, data, sizeof(data)) == sizeof(data));
    assert(memcmp(uuid, UUID, sizeof(UUID)) == 0);
    assert(memcmp(data, UUID, sizeof(UUID)) == 0);
    assert(luksmeta_del(cd, r) == 0);

    /* Test the layout state. */
    assert(test_layout((range_t[]) {
        { 0, 1024 },                   /* LUKS header */
        { 1024, offset - 1024, true }, /* Keyslot Area */
        { offset, 4096 },              /* luksmeta header */
        END(offset + 4096),            /* Rest of the file */
    }));

    crypt_free(cd);
    unlink(filename);
    return 0;
}
