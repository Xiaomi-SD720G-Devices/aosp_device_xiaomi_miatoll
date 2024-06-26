#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2023 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

DEVICE=miatoll
VENDOR=xiaomi

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "${MY_DIR}" ]]; then MY_DIR="${PWD}"; fi

ANDROID_ROOT="${MY_DIR}/../../.."

HELPER="${ANDROID_ROOT}/tools/extract-utils/extract_utils.sh"
if [ ! -f "${HELPER}" ]; then
    echo "Unable to find helper script at ${HELPER}"
    exit 1
fi
source "${HELPER}"

# Default to sanitizing the vendor folder before extraction
CLEAN_VENDOR=true

ONLY_FIRMWARE=
KANG=
SECTION=

while [ "${#}" -gt 0 ]; do
    case "${1}" in
        -n | --no-cleanup )
                CLEAN_VENDOR=false
                ;;
        -k | --kang )
                KANG="--kang"
                ;;
        -s | --section )
                SECTION="${2}"; shift
                CLEAN_VENDOR=false
                ;;
        * )
                SRC="${1}"
                ;;
    esac
    shift
done

if [ -z "${SRC}" ]; then
    SRC="adb"
fi

function blob_fixup() {
    case "${1}" in
    system_ext/lib64/lib-imsvideocodec.so )
    "${PATCHELF}" --add-needed "lib-imsvt.so" "${2}"
    ;;
    vendor/etc/camera/camxoverridesettings.txt)
    sed -i "s/0x10082/0/g" "${2}"
    sed -i "s/0x1F/0x0/g" "${2}"
    ;;
    vendor/lib64/camera/components/com.qti.node.watermark.so)
    grep -q "libpiex_shim.so" "${2}" || "${PATCHELF}" --add-needed "libpiex_shim.so" "${2}"
    ;;
    vendor/lib64/hw/fingerprint.fpc.default.so)
    # NOP out report_input_event()
    "${SIGSCAN}" -p "30 00 00 90 11 3a 42 f9" -P "30 00 00 90 1f 20 03 d5" -f "${2}"
    ;;
    vendor/bin/hw/dolbycodec2)
    patchelf --replace-needed libavservices_minijail_vendor.so libavservices_minijail.so "${2}"
    patchelf --replace-needed libcodec2_hidl@1.0.so libcodec2_hidl@1.0.stock.so "${2}"
    ;;
    vendor/lib/libcodec2_hidl@1.0.stock.so)
    patchelf --set-soname libcodec2_hidl@1.0.stock.so "${2}"
    patchelf --replace-needed libcodec2_vndk.so libcodec2_vndk.stock.so "${2}"
    ;;
    vendor/lib/libcodec2_vndk.stock.so)
    patchelf --set-soname libcodec2_vndk.stock.so "${2}"
    ;;
    esac
}

# Initialize the helper
setup_vendor "${DEVICE}" "${VENDOR}" "${ANDROID_ROOT}" false "${CLEAN_VENDOR}"

if [ -z "${ONLY_FIRMWARE}" ]; then
    extract "${MY_DIR}/proprietary-files.txt" "${SRC}" "${KANG}" --section "${SECTION}"
fi

"${MY_DIR}/setup-makefiles.sh"