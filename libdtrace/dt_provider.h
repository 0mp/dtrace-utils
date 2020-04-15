/*
 * Oracle Linux DTrace.
 * Copyright (c) 2006, 2020, Oracle and/or its affiliates. All rights reserved.
 * Licensed under the Universal Permissive License v 1.0 as shown at
 * http://oss.oracle.com/licenses/upl.
 */

#ifndef	_DT_PROVIDER_H
#define	_DT_PROVIDER_H

#include <dt_impl.h>
#include <dt_ident.h>
#include <dt_list.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define TRACEFS		"/sys/kernel/debug/tracing/"
#define EVENTSFS	TRACEFS "events/"

#define DTRACE_PAT	"dtrace_%d"

struct dt_probe;

/*
 * Because it would waste both space and time, argument types are not recorded
 * with the probe by default.  In order to determine argument types for args[X]
 * variables, the D compiler queries for argument types on a probe-by-probe
 * basis.  (This optimizes for the common case that arguments are either not
 * used or used in an untyped fashion.)  Typed arguments are specified with a
 * string of the type name in the 'native' member of the argument description
 * structure.  Typed arguments may be further translated to types of greater
 * stability; the provider indicates such a translated argument by filling in
 * the 'xlate' member with the string of the translated type.
 * Finally, the provider may indicate which argument value a given argument
 * maps to by setting the 'mapping' member -- allowing a single argument to map
 * to multiple args[X] variables.
 */
typedef struct dt_argdesc {
#ifdef FIXME
	dtrace_id_t id;
	int ndx;
#endif
	int mapping;
	char *native;
	char *xlate;
} dt_argdesc_t;

typedef struct dt_provimpl {
	const char *name;			/* provider generic name */
	int prog_type;				/* BPF program type */
	int (*populate)(dtrace_hdl_t *dtp);	/* register probes */
	int (*probe_info)(dtrace_hdl_t *dtp,	/* get probe info */
			  const struct dt_probe *prp,
			  int *idp, int *argcp, dt_argdesc_t **argvp);
	void (*trampoline)(dt_pcb_t *pcb,	/* generate BPF trampoline */
			   int haspred);
	int (*probe_fini)(dtrace_hdl_t *dtp,	/* probe cleanup */
			  struct dt_probe *prb);
} dt_provimpl_t;

extern int tp_event_info(dtrace_hdl_t *dtp, FILE *f, int skip, int *idp,
			 int *argcp, dt_argdesc_t **argvp);

extern dt_provimpl_t dt_dtrace;
extern dt_provimpl_t dt_fbt;
extern dt_provimpl_t dt_sdt;
extern dt_provimpl_t dt_syscall;

typedef struct dt_provider {
	dt_list_t pv_list;		/* list forward/back pointers */
	struct dt_provider *pv_next;	/* pointer to next provider in hash */
	dtrace_providerdesc_t desc;	/* provider name and attributes */
	const dt_provimpl_t *impl;	/* provider implementation */
	dt_idhash_t *pv_probes;		/* probe defs (if user-declared) */
	dt_node_t *pv_nodes;		/* parse node allocation list */
	ulong_t *pv_xrefs;		/* translator reference bitmap */
	ulong_t pv_xrmax;		/* number of valid bits in pv_xrefs */
	ulong_t pv_gen;			/* generation # that created me */
	dtrace_hdl_t *pv_hdl;		/* pointer to containing dtrace_hdl */
	uint_t pv_flags;		/* flags (see below) */
} dt_provider_t;

#define	DT_PROVIDER_INTF	0x1	/* provider interface declaration */
#define	DT_PROVIDER_IMPL	0x2	/* provider implementation is loaded */

extern dt_provider_t *dt_provider_lookup(dtrace_hdl_t *, const char *);
extern dt_provider_t *dt_provider_create(dtrace_hdl_t *, const char *,
					 const dt_provimpl_t *,
					 const dtrace_pattr_t *);
extern void dt_provider_destroy(dtrace_hdl_t *, dt_provider_t *);
extern int dt_provider_xref(dtrace_hdl_t *, dt_provider_t *, id_t);

#ifdef	__cplusplus
}
#endif

#endif	/* _DT_PROVIDER_H */
