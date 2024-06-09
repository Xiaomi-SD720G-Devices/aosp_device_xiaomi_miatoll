# clang v17.0.4
git clone -b clang-r498229b https://github.com/Xiaomi-SD720G-Devices/AOSP-clang.git prebuilts/clang/host/linux-x86/clang-r498229b --depth=1

# hardware/Motorola/Dolby
git clone -b v1.1-daxappui https://github.com/userariii/hardware_Motorola_Dolby.git hardware/Motorola/Dolby

# hardware/google/pixel
git clone -b lineage-21.0 https://github.com/LineageOS/android_hardware_google_pixel.git hardware/google/pixel --depth=1

# hardware/sony/timekeep
git clone -b lineage-21 https://github.com/LineageOS/android_hardware_sony_timekeep.git hardware/sony/timekeep --depth=1

# hardware/xiaomi
git clone -b lineage-21 https://github.com/LineageOS/android_hardware_xiaomi.git hardware/xiaomi --depth=1

# kernel/xiaomi
git clone -b 14-OpenELA https://github.com/ProjectElixir-Devices/kernel_xiaomi_sm6250.git kernel/xiaomi/sm6250 --depth=1

# vendor/xiaomi
git clone -b Fourteen https://github.com/ProjectElixir-Devices/vendor_xiaomi_miatoll.git vendor/xiaomi/miatoll --depth=1
