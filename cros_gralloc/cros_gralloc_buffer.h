/*
 * Copyright 2017 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef CROS_GRALLOC_BUFFER_H
#define CROS_GRALLOC_BUFFER_H

#include "cros_gralloc_helpers.h"

class cros_gralloc_buffer
{
      public:
	static cros_gralloc_buffer *create(struct bo *acquire_bo,
					   const struct cros_gralloc_handle *borrowed_handle);
	~cros_gralloc_buffer();

	uint32_t get_id() const;

	/* The new reference count is returned by both these functions. */
	int32_t increase_refcount();
	int32_t decrease_refcount();

	int32_t lock(const struct rectangle *rect, uint32_t map_flags,
		     uint8_t *addr[DRV_MAX_PLANES]);
	int32_t unlock();
	int32_t resource_info(uint32_t strides[DRV_MAX_PLANES], uint32_t offsets[DRV_MAX_PLANES],
			      uint64_t *format_modifier);

	int32_t invalidate();
	int32_t flush();

	int32_t get_reserved_region(void **reserved_region_addr, uint64_t *reserved_region_size);

      private:
	// Temporary access to hnd_.
	friend class cros_gralloc_driver;

	cros_gralloc_buffer(struct bo *acquire_bo, struct cros_gralloc_handle *acquire_handle);

	cros_gralloc_buffer(cros_gralloc_buffer const &);
	cros_gralloc_buffer operator=(cros_gralloc_buffer const &);

	struct bo *bo_;

	/* Note: this will be nullptr for imported/retained buffers. */
	struct cros_gralloc_handle *hnd_;

	int32_t refcount_ = 1;
	int32_t lockcount_ = 0;

	struct mapping *lock_data_[DRV_MAX_PLANES];

	/* Optional additional shared memory region attached to some gralloc buffers. */
	void *reserved_region_addr_ = nullptr;
};

#endif
