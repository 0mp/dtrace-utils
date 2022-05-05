/*
 * Oracle Linux DTrace.
 * Copyright (c) 2020, 2022, Oracle and/or its affiliates. All rights reserved.
 * Licensed under the Universal Permissive License v 1.0 as shown at
 * http://oss.oracle.com/licenses/upl.
 */

/*
 * ASSERTION: The 'arg3' variable can be accessed.  (This implementation
 * sets the arg to 0, so check this undocumented behavior as well.)
 *
 * SECTION: Variables/Built-in Variables/arg3
 */

#pragma D option quiet

BEGIN {
	trace(arg3);
	exit(arg3 == 0 ? 0 : 1);
}

ERROR {
	exit(1);
}
