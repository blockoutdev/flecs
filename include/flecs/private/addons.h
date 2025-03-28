/**
 * @file addons.h
 * @brief Include enabled addons.
 *
 * This file should only be included by the main flecs.h header.
 */

#ifndef FLECS_ADDONS_H
#define FLECS_ADDONS_H

/* Blacklist macros */
#ifdef FLECS_NO_CPP
#undef FLECS_CPP
#endif
#ifdef FLECS_NO_MODULE
#undef FLECS_MODULE
#endif
#ifdef FLECS_NO_SCRIPT
#undef FLECS_SCRIPT
#endif
#ifdef FLECS_NO_PARSER
#undef FLECS_PARSER
#endif
#ifdef FLECS_NO_QUERY_DSL
#undef FLECS_QUERY_DSL
#endif
#ifdef FLECS_NO_SCRIPT_MATH
#undef FLECS_SCRIPT_MATH
#endif
#ifdef FLECS_NO_STATS
#undef FLECS_STATS
#endif
#ifdef FLECS_NO_SYSTEM
#undef FLECS_SYSTEM
#endif
#ifdef FLECS_NO_ALERTS
#undef FLECS_ALERTS
#endif
#ifdef FLECS_NO_PIPELINE
#undef FLECS_PIPELINE
#endif
#ifdef FLECS_NO_TIMER
#undef FLECS_TIMER
#endif
#ifdef FLECS_NO_META
#undef FLECS_META
#endif
#ifdef FLECS_NO_UNITS
#undef FLECS_UNITS
#endif
#ifdef FLECS_NO_JSON
#undef FLECS_JSON
#endif
#ifdef FLECS_NO_DOC
#undef FLECS_DOC
#endif
#ifdef FLECS_NO_LOG
#undef FLECS_LOG
#endif
#ifdef FLECS_NO_APP
#undef FLECS_APP
#endif
#ifdef FLECS_NO_OS_API_IMPL
#undef FLECS_OS_API_IMPL
#endif
#ifdef FLECS_NO_HTTP
#undef FLECS_HTTP
#endif
#ifdef FLECS_NO_REST
#undef FLECS_REST
#endif
#ifdef FLECS_NO_JOURNAL
#undef FLECS_JOURNAL
#endif

/* Always included, if disabled functions are replaced with dummy macros */
#include "flecs/private/journal.h"
#include "flecs/addons/log.h"

/* Handle addon dependencies that need declarations to be visible in header */
#ifdef FLECS_STATS
#ifndef FLECS_PIPELINE
#define FLECS_PIPELINE
#endif
#ifndef FLECS_TIMER
#define FLECS_TIMER
#endif
#endif

#ifdef FLECS_REST
#ifndef FLECS_HTTP
#define FLECS_HTTP
#endif
#endif

#ifdef FLECS_APP
#ifdef FLECS_NO_APP
#error "FLECS_NO_APP failed: APP is required by other addons"
#endif
#include "../addons/app.h"
#endif

#ifdef FLECS_HTTP
#ifdef FLECS_NO_HTTP
#error "FLECS_NO_HTTP failed: HTTP is required by other addons"
#endif
#include "../addons/http.h"
#endif

#ifdef FLECS_REST
#ifdef FLECS_NO_REST
#error "FLECS_NO_REST failed: REST is required by other addons"
#endif
#include "../addons/rest.h"
#endif

#ifdef FLECS_TIMER
#ifdef FLECS_NO_TIMER
#error "FLECS_NO_TIMER failed: TIMER is required by other addons"
#endif
#include "../addons/timer.h"
#endif

#ifdef FLECS_PIPELINE
#ifdef FLECS_NO_PIPELINE
#error "FLECS_NO_PIPELINE failed: PIPELINE is required by other addons"
#endif
#include "../addons/pipeline.h"
#endif

#ifdef FLECS_SYSTEM
#ifdef FLECS_NO_SYSTEM
#error "FLECS_NO_SYSTEM failed: SYSTEM is required by other addons"
#endif
#include "../addons/system.h"
#endif

#ifdef FLECS_STATS
#ifdef FLECS_NO_STATS
#error "FLECS_NO_STATS failed: STATS is required by other addons"
#endif
#include "../addons/stats.h"
#endif

#ifdef FLECS_METRICS
#ifdef FLECS_NO_METRICS
#error "FLECS_NO_METRICS failed: METRICS is required by other addons"
#endif
#include "../addons/metrics.h"
#endif

#ifdef FLECS_ALERTS
#ifdef FLECS_NO_ALERTS
#error "FLECS_NO_ALERTS failed: ALERTS is required by other addons"
#endif
#include "../addons/alerts.h"
#endif

#ifdef FLECS_JSON
#ifdef FLECS_NO_JSON
#error "FLECS_NO_JSON failed: JSON is required by other addons"
#endif
#include "../addons/json.h"
#endif

#ifdef FLECS_UNITS
#ifdef FLECS_NO_UNITS
#error "FLECS_NO_UNITS failed: UNITS is required by other addons"
#endif
#include "../addons/units.h"
#endif

#ifdef FLECS_SCRIPT_MATH
#ifdef FLECS_NO_SCRIPT_MATH
#error "FLECS_NO_SCRIPT_MATH failed: SCRIPT_MATH is required by other addons"
#endif
#include "../addons/script_math.h"
#endif

#ifdef FLECS_PARSER
#ifdef FLECS_NO_PARSER
#error "FLECS_NO_PARSER failed: PARSER is required by other addons"
#endif
#endif

#ifdef FLECS_QUERY_DSL
#ifdef FLECS_NO_QUERY_DSL
#error "FLECS_NO_QUERY_DSL failed: QUERY_DSL is required by other addons"
#endif
#ifndef FLECS_PARSER
#define FLECS_PARSER
#endif
#endif

#ifdef FLECS_SCRIPT
#ifdef FLECS_NO_SCRIPT
#error "FLECS_NO_SCRIPT failed: SCRIPT is required by other addons"
#endif
#include "../addons/script.h"
#endif

#ifdef FLECS_DOC
#ifdef FLECS_NO_DOC
#error "FLECS_NO_DOC failed: DOC is required by other addons"
#endif
#include "../addons/doc.h"
#endif

#ifdef FLECS_META
#ifdef FLECS_NO_META
#error "FLECS_NO_META failed: META is required by other addons"
#endif
#include "../addons/meta.h"
#endif

#ifdef FLECS_OS_API_IMPL
#ifdef FLECS_NO_OS_API_IMPL
#error "FLECS_NO_OS_API_IMPL failed: OS_API_IMPL is required by other addons"
#endif
#include "../private/os_api_impl.h"
#endif

#ifdef FLECS_MODULE
#ifdef FLECS_NO_MODULE
#error "FLECS_NO_MODULE failed: MODULE is required by other addons"
#endif
#include "../addons/module.h"
#endif

#ifdef FLECS_CPP
#ifdef FLECS_NO_CPP
#error "FLECS_NO_CPP failed: CPP is required by other addons"
#endif
#include "../addons/flecs_cpp.h"

#ifdef __cplusplus
#include "../addons/cpp/flecs.hpp"
#endif // __cplusplus

#endif // FLECS_CPP

#endif
