// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022, Oracle and/or its affiliates. All rights reserved.
 */
#include <linux/bpf.h>
#include <stdint.h>
#include <bpf-helpers.h>
#include <dt_dctx.h>

#ifndef noinline
# define noinline	__attribute__((noinline))
#endif

/*
 * Get a pointer to the data storage for an aggregation.  Regular aggregations
 * are stored as an indexed aggregation with an empty key.  The 'id' parameter
 * is the aggregtion id; 'ival' is the initial value for min/max aggregations
 * (and otherwise 0).  The 'dflt' parameter is a pointer to a zero-filled area
 * of memory that is at least the size of the largest aggregation.
 */
noinline uint64_t *dt_get_agg(const dt_dctx_t *dctx, uint32_t id,
			      const char *key, uint64_t ival, const char *dflt)
{
	uint64_t	*valp;

	/* place the variable ID at the beginning of the key */
	*(uint32_t *)key = id;

	/* try to look up the key */
	valp = bpf_map_lookup_elem(dctx->agg, key);

	/* if not found, create it */
	if (valp == 0) {
		/* start with all zeroes */
		if (bpf_map_update_elem(dctx->agg, key, dflt, BPF_ANY) < 0)
			return 0;

		valp = bpf_map_lookup_elem(dctx->agg, key);
		if (valp == 0)
			return 0;

		/* ival is nonzero only for min() and max() */
		if (ival)
			valp[1] = ival;
	}

	/* increment the data counter */
	valp[0] += 1;

	/* advance past the data counter */
	valp += 1;

	return valp;
}
