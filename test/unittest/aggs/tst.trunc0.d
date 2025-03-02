/*
 * Oracle Linux DTrace.
 * Copyright (c) 2006, 2020, Oracle and/or its affiliates. All rights reserved.
 * Licensed under the Universal Permissive License v 1.0 as shown at
 * http://oss.oracle.com/licenses/upl.
 */
/* @@xfail: dtv2 - requires trunc() */

#pragma D option quiet
#pragma D option aggrate=1ms
#pragma D option switchrate=50ms

int i;

tick-100ms
/i < 10/
{
	@[i] = sum(10 - i);
	i++;
}

tick-100ms
/i == 5/
{
	trunc(@);
}

tick-100ms
/i == 10/
{
	exit(0);
}

END
{
	printa(@);
}
