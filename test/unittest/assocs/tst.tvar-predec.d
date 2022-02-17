/*
 * Oracle Linux DTrace.
 * Copyright (c) 2022, Oracle and/or its affiliates. All rights reserved.
 * Licensed under the Universal Permissive License v 1.0 as shown at
 * http://oss.oracle.com/licenses/upl.
 */

/*
 * ASSERTION: Pre-decrement should work for TLS associative arrays.
 *
 * SECTION: Variables/Associative Arrays
 */

#pragma D option quiet

BEGIN
{
	old = self->a["a"] = 42;
	val = --self->a["a"];

	exit(val != old - 1 || self->a["a"] != val);
}
