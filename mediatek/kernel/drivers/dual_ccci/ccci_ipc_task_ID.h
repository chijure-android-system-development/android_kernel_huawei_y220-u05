/*
 * The MTK prebuild drops this next to the bionic UAPI headers. This tree
 * does not have that copy. The macro shape matches the table in ccci_ipc.c:
 * X_IPC_MODULE_CONF(priority, local_id, extq_id, max_sent) -> {extq_id, local_id}.
 * No task rows yet: the modem IPC map for this project was not in the drop.
 */
#ifndef __CCCI_IPC_TASK_ID_H__
#define __CCCI_IPC_TASK_ID_H__

#ifdef __IPC_ID_TABLE
#define X_IPC_MODULE_CONF(a, b, c, d) {c, b},
#else
#define X_IPC_MODULE_CONF(a, b, c, d)
#endif

/* High bit marks an AP-side unify id. Task rows for this modem are not in the drop. */
#define AP_UNIFY_ID_FLAG (1U << 31)
#define MD_UNIFY_ID_FLAG (0U << 31)

#endif
