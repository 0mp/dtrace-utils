/*
 * Oracle Linux DTrace.
 * Copyright (c) 2023, Oracle and/or its affiliates. All rights reserved.
 * Licensed under the Universal Permissive License v 1.0 as shown at
 * http://oss.oracle.com/licenses/upl.
 */

/*
 * ASSERTION: uregs[] not supported on old kernels.
 *
 * SECTION: User Process Tracing/uregs Array
 */

BEGIN
{
	trace(uregs[R_SP]);
	exit(1);
}

