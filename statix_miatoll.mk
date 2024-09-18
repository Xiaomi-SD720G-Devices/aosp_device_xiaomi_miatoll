# Inherit from miatoll  device
$(call inherit-product, device/xiaomi/miatoll/atoll.mk)

# Inherit some common statix stuff.
$(call inherit-product, vendor/statix/config/common.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/non_ab_device.mk)

# Bootanimation Resolution
TARGET_BOOT_ANIMATION_RES := 1080

# NGA
TARGET_SUPPORTS_NEXT_GEN_ASSISTANT := true

# Device identifier
PRODUCT_NAME := statix_miatoll
PRODUCT_DEVICE := miatoll
PRODUCT_BRAND := Xiaomi
PRODUCT_MODEL := SM6250
PRODUCT_MANUFACTURER := Xiaomi

PRODUCT_GMS_CLIENTID_BASE := android-xiaomi
WITH_GMS := true

IS_PHONE := true
TARGET_SUPPORTS_GOOGLE_RECORDER := true
TARGET_INCLUDE_STOCK_ACORE := true
TARGET_INCLUDE_LIVE_WALLPAPERS := true
TARGET_FACE_UNLOCK_SUPPORTED := true
TARGET_SUPPORTS_QUICK_TAP := true
ENABLE_GAMETOOLS := true
INCLUDE_PIXEL_LAUNCHER := true

BUILD_USERNAME := userariii
BUILD_HOSTNAME := statix
