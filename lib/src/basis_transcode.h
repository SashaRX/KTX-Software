// Copyright 2026 The Khronos Group Inc.
// SPDX-License-Identifier: Apache-2.0

#ifndef _BASIS_TRANSCODE_H_
#define _BASIS_TRANSCODE_H_

/*
 * Internal interface shared between the Basis transcode paths:
 * ktxTexture2_TranscodeBasis (basis_transcode.cpp) and the streaming
 * ktxLevelProcessor (level_processor.cpp).
 */

#include <ktx.h>
#include "basis_sgd.h"
#include "vkformat_enum.h"

/*
 * Resolve a requested transcode target against a Basis-compressed source
 * texture: detect the source's alpha content, map the automatic selection
 * formats (KTX_TTF_ETC, KTX_TTF_BC1_OR_3, PVRTC RGBA variants) to their
 * concrete targets, resolve the target VkFormat (applying the source's
 * transfer function), validate PVRTC1 power-of-two dimensions and check
 * that the transcoder for the source/target pair is included in the build.
 *
 * On success *pOutputFormat holds the concrete target, *pVkFormat the
 * resolved VkFormat and *pAlphaContent the source's alpha content.
 */
KTX_error_code
ktxBasis_ResolveTargetFormat(const ktxTexture2* This,
                             ktx_transcode_fmt_e* pOutputFormat,
                             VkFormat* pVkFormat,
                             alpha_content_e* pAlphaContent);

#endif /* _BASIS_TRANSCODE_H_ */
