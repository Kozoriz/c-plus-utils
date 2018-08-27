#pragma once

#include <assert.h>
#include "utils/logger.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName)  \
  TypeName(const TypeName& other) = delete; \
  void operator=(const TypeName& other) = delete;

#if defined(DEBUG)
#define ASSERT(condition) assert(condition);
#else
#define ASSERT(condition)
#endif

#define DCHECK(condition)                        \
  if (!(condition)) {                            \
    CREATE_LOGGER("Utils")                       \
    LOG_FATAL("DCHECK failed : " << #condition); \
    ASSERT(condition)                            \
  }
