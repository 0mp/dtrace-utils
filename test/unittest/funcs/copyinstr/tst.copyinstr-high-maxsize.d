/*
 * Oracle Linux DTrace.
 * Copyright (c) 2006, 2023, Oracle and/or its affiliates. All rights reserved.
 * Licensed under the Universal Permissive License v 1.0 as shown at
 * http://oss.oracle.com/licenses/upl.
 */

/*
 * ASSERTION: It is possible to read a string from userspace addresses.
 *
 * SECTION: Actions and Subroutines/copyinstr()
 *	    User Process Tracing/copyin() and copyinstr() Subroutines
 */
/* @@trigger: delaydie */

#pragma D option quiet
#pragma D option strsize=12

syscall::write:entry
/pid == $target/
{
	printf("%s char match\n", (s = copyinstr(arg1, 96))[4] == 'y' ? "good" : "BAD");
	printf("'%s'", s);
	exit(0);
}

ERROR
{
	exit(1);
}
